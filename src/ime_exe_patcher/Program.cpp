#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>
#include <bcrypt.h>
#include <fcntl.h>
#include <io.h>

#include <algorithm>
#include <array>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace
{
constexpr std::wstring_view Version = L"1.0.0";
constexpr std::wstring_view ProductName = L"GalleyHouse IME EXE Patcher";
constexpr std::wstring_view TargetName = L"galleyhouse.exe";
constexpr std::wstring_view BackupName = L"galleyhouse.exe.backup";
constexpr std::uint64_t ExpectedSize = 104534016;
constexpr std::uint64_t PatchOffset = 0x2510F;
constexpr std::wstring_view OriginalSha256 =
    L"66A57A2033E0A7BC9418C45EE4DD32678DB62F00A2D76485172FCBD894BD37A5";
constexpr std::wstring_view PatchedSha256 =
    L"9996BF1F5C715570BD066CF91ADCE5AE6AE8DDCE5129E661E8E169D4FC06E54F";

constexpr std::array<unsigned char, 20> OriginalBytes = {
    0x31, 0xD2, 0xE8, 0x62, 0xD3, 0x1D, 0x03, 0xFF, 0x15, 0xFC,
    0xEC, 0x47, 0x06, 0xC6, 0x87, 0xB8, 0x01, 0x00, 0x00, 0x00,
};

constexpr std::array<unsigned char, 20> PatchedBytes = {
    0x31, 0xD2, 0x89, 0x97, 0xB8, 0x01, 0x00, 0x00, 0xE8, 0x5C,
    0xD3, 0x1D, 0x03, 0xFF, 0x15, 0xF6, 0xEC, 0x47, 0x06, 0x90,
};

class AppError
{
public:
    explicit AppError(std::wstring message) : message_(std::move(message))
    {
    }

    const std::wstring& message() const noexcept
    {
        return message_;
    }

private:
    std::wstring message_;
};

class FileHandle
{
public:
    FileHandle() = default;
    explicit FileHandle(HANDLE handle) : handle_(handle)
    {
    }

    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    FileHandle(FileHandle&& other) noexcept : handle_(std::exchange(other.handle_, INVALID_HANDLE_VALUE))
    {
    }

    FileHandle& operator=(FileHandle&& other) noexcept
    {
        if (this != &other)
        {
            close();
            handle_ = std::exchange(other.handle_, INVALID_HANDLE_VALUE);
        }
        return *this;
    }

    ~FileHandle()
    {
        close();
    }

    HANDLE get() const noexcept
    {
        return handle_;
    }

    bool valid() const noexcept
    {
        return handle_ != INVALID_HANDLE_VALUE;
    }

    void close() noexcept
    {
        if (valid())
        {
            CloseHandle(handle_);
            handle_ = INVALID_HANDLE_VALUE;
        }
    }

private:
    HANDLE handle_ = INVALID_HANDLE_VALUE;
};

class AlgorithmHandle
{
public:
    ~AlgorithmHandle()
    {
        if (handle_ != nullptr)
        {
            BCryptCloseAlgorithmProvider(handle_, 0);
        }
    }

    BCRYPT_ALG_HANDLE* put() noexcept
    {
        return &handle_;
    }

    BCRYPT_ALG_HANDLE get() const noexcept
    {
        return handle_;
    }

private:
    BCRYPT_ALG_HANDLE handle_ = nullptr;
};

class HashHandle
{
public:
    ~HashHandle()
    {
        if (handle_ != nullptr)
        {
            BCryptDestroyHash(handle_);
        }
    }

    BCRYPT_HASH_HANDLE* put() noexcept
    {
        return &handle_;
    }

    BCRYPT_HASH_HANDLE get() const noexcept
    {
        return handle_;
    }

private:
    BCRYPT_HASH_HANDLE handle_ = nullptr;
};

struct FileFingerprint
{
    std::uint64_t size;
    std::wstring sha256;

    bool matches(std::uint64_t other_size, std::wstring_view other_sha256) const noexcept
    {
        return size == other_size &&
            sha256.size() == other_sha256.size() &&
            _wcsnicmp(sha256.c_str(), other_sha256.data(), other_sha256.size()) == 0;
    }
};

std::wstring format_windows_error(DWORD code)
{
    LPWSTR raw_message = nullptr;
    const DWORD length = FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        code,
        0,
        reinterpret_cast<LPWSTR>(&raw_message),
        0,
        nullptr);

    if (length == 0 || raw_message == nullptr)
    {
        return L"Windows 오류 " + std::to_wstring(code);
    }

    std::wstring message(raw_message, length);
    LocalFree(raw_message);
    while (!message.empty() && (message.back() == L'\r' || message.back() == L'\n'))
    {
        message.pop_back();
    }
    return message;
}

[[noreturn]] void throw_windows_error(std::wstring_view action, DWORD code = GetLastError())
{
    throw AppError(std::wstring(action) + L": " + format_windows_error(code));
}

[[noreturn]] void throw_bcrypt_error(std::wstring_view action, NTSTATUS status)
{
    std::wostringstream message;
    message << action << L": NTSTATUS 0x"
            << std::hex << std::uppercase << static_cast<unsigned long>(status);
    throw AppError(message.str());
}

void check_bcrypt(NTSTATUS status, std::wstring_view action)
{
    if (status < 0)
    {
        throw_bcrypt_error(action, status);
    }
}

FileHandle open_file(
    const std::filesystem::path& path,
    DWORD access,
    DWORD sharing,
    DWORD creation,
    DWORD flags)
{
    FileHandle file(CreateFileW(
        path.c_str(),
        access,
        sharing,
        nullptr,
        creation,
        flags,
        nullptr));
    if (!file.valid())
    {
        throw_windows_error(path.wstring());
    }
    return file;
}

std::uint64_t get_file_size(HANDLE file)
{
    LARGE_INTEGER size = {};
    if (!GetFileSizeEx(file, &size))
    {
        throw_windows_error(L"파일 크기 확인 실패");
    }
    if (size.QuadPart < 0)
    {
        throw AppError(L"파일 크기가 올바르지 않습니다.");
    }
    return static_cast<std::uint64_t>(size.QuadPart);
}

void set_file_position(HANDLE file, std::uint64_t position)
{
    LARGE_INTEGER distance = {};
    distance.QuadPart = static_cast<LONGLONG>(position);
    if (!SetFilePointerEx(file, distance, nullptr, FILE_BEGIN))
    {
        throw_windows_error(L"파일 위치 이동 실패");
    }
}

void read_exact(HANDLE file, unsigned char* buffer, std::size_t size)
{
    std::size_t offset = 0;
    while (offset < size)
    {
        DWORD bytes_read = 0;
        const DWORD requested = static_cast<DWORD>(std::min<std::size_t>(size - offset, MAXDWORD));
        if (!ReadFile(file, buffer + offset, requested, &bytes_read, nullptr))
        {
            throw_windows_error(L"파일 읽기 실패");
        }
        if (bytes_read == 0)
        {
            throw AppError(L"패치 위치의 원본 바이트를 읽지 못했습니다.");
        }
        offset += bytes_read;
    }
}

void write_all(HANDLE file, const unsigned char* buffer, std::size_t size)
{
    std::size_t offset = 0;
    while (offset < size)
    {
        DWORD bytes_written = 0;
        const DWORD requested = static_cast<DWORD>(std::min<std::size_t>(size - offset, MAXDWORD));
        if (!WriteFile(file, buffer + offset, requested, &bytes_written, nullptr))
        {
            throw_windows_error(L"파일 쓰기 실패");
        }
        if (bytes_written == 0)
        {
            throw AppError(L"파일을 끝까지 쓰지 못했습니다.");
        }
        offset += bytes_written;
    }
}

std::wstring bytes_to_hex(const std::vector<unsigned char>& bytes)
{
    constexpr wchar_t Digits[] = L"0123456789ABCDEF";
    std::wstring result;
    result.resize(bytes.size() * 2);
    for (std::size_t index = 0; index < bytes.size(); ++index)
    {
        result[index * 2] = Digits[bytes[index] >> 4];
        result[index * 2 + 1] = Digits[bytes[index] & 0x0F];
    }
    return result;
}

FileFingerprint fingerprint(const std::filesystem::path& path)
{
    FileHandle file = open_file(
        path,
        GENERIC_READ,
        FILE_SHARE_READ,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN);
    const std::uint64_t initial_size = get_file_size(file.get());

    AlgorithmHandle algorithm;
    check_bcrypt(
        BCryptOpenAlgorithmProvider(algorithm.put(), BCRYPT_SHA256_ALGORITHM, nullptr, 0),
        L"SHA-256 초기화 실패");

    DWORD object_length = 0;
    DWORD hash_length = 0;
    DWORD result_length = 0;
    check_bcrypt(
        BCryptGetProperty(
            algorithm.get(),
            BCRYPT_OBJECT_LENGTH,
            reinterpret_cast<PUCHAR>(&object_length),
            sizeof(object_length),
            &result_length,
            0),
        L"SHA-256 객체 크기 확인 실패");
    check_bcrypt(
        BCryptGetProperty(
            algorithm.get(),
            BCRYPT_HASH_LENGTH,
            reinterpret_cast<PUCHAR>(&hash_length),
            sizeof(hash_length),
            &result_length,
            0),
        L"SHA-256 길이 확인 실패");

    std::vector<unsigned char> hash_object(object_length);
    std::vector<unsigned char> digest(hash_length);
    HashHandle hash;
    check_bcrypt(
        BCryptCreateHash(
            algorithm.get(),
            hash.put(),
            hash_object.data(),
            static_cast<ULONG>(hash_object.size()),
            nullptr,
            0,
            0),
        L"SHA-256 해시 생성 실패");

    std::vector<unsigned char> buffer(1024 * 1024);
    std::uint64_t total_read = 0;
    for (;;)
    {
        DWORD bytes_read = 0;
        if (!ReadFile(
                file.get(),
                buffer.data(),
                static_cast<DWORD>(buffer.size()),
                &bytes_read,
                nullptr))
        {
            throw_windows_error(L"파일 해시 읽기 실패");
        }
        if (bytes_read == 0)
        {
            break;
        }
        check_bcrypt(
            BCryptHashData(hash.get(), buffer.data(), bytes_read, 0),
            L"SHA-256 계산 실패");
        total_read += bytes_read;
    }

    const std::uint64_t final_size = get_file_size(file.get());
    if (total_read != initial_size || final_size != initial_size)
    {
        throw AppError(L"파일 크기가 읽는 도중 변경되었습니다.");
    }

    check_bcrypt(
        BCryptFinishHash(hash.get(), digest.data(), static_cast<ULONG>(digest.size()), 0),
        L"SHA-256 완료 실패");
    return FileFingerprint{initial_size, bytes_to_hex(digest)};
}

bool file_exists(const std::filesystem::path& path)
{
    const DWORD attributes = GetFileAttributesW(path.c_str());
    return attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
}

void verify_preimage(const std::filesystem::path& path)
{
    FileHandle file = open_file(
        path,
        GENERIC_READ,
        FILE_SHARE_READ,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL);
    set_file_position(file.get(), PatchOffset);

    std::array<unsigned char, OriginalBytes.size()> actual = {};
    read_exact(file.get(), actual.data(), actual.size());
    if (actual != OriginalBytes)
    {
        throw AppError(std::wstring(TargetName) + L"의 패치 위치 원본 바이트가 예상과 다릅니다.");
    }
}

void try_delete(const std::filesystem::path& path) noexcept
{
    DeleteFileW(path.c_str());
}

void report_existing_backup(const std::filesystem::path& path)
{
    if (!file_exists(path))
    {
        std::wcout << L"[주의] " << BackupName
                   << L"가 없습니다. 이미 패치된 EXE는 변경하지 않았습니다.\n";
        return;
    }

    try
    {
        const FileFingerprint backup = fingerprint(path);
        if (backup.matches(ExpectedSize, OriginalSha256))
        {
            std::wcout << L"[확인] 원본 백업 " << BackupName << L"가 정상입니다.\n";
        }
        else
        {
            std::wcout << L"[주의] " << BackupName << L"가 지원 원본과 일치하지 않습니다.\n";
        }
    }
    catch (const AppError& error)
    {
        std::wcout << L"[주의] " << BackupName
                   << L"를 검사하지 못했습니다: " << error.message() << L'\n';
    }
}

void ensure_backup(
    const std::filesystem::path& source,
    const std::filesystem::path& backup)
{
    if (file_exists(backup))
    {
        if (!fingerprint(backup).matches(ExpectedSize, OriginalSha256))
        {
            throw AppError(
                L"기존 " + std::wstring(BackupName) +
                L"가 지원 원본과 일치하지 않아 덮어쓰지 않습니다.");
        }
        return;
    }

    bool created = false;
    try
    {
        FileHandle input = open_file(
            source,
            GENERIC_READ,
            FILE_SHARE_READ,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN);
        FileHandle output = open_file(
            backup,
            GENERIC_WRITE,
            0,
            CREATE_NEW,
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH);
        created = true;

        std::vector<unsigned char> buffer(1024 * 1024);
        for (;;)
        {
            DWORD bytes_read = 0;
            if (!ReadFile(
                    input.get(),
                    buffer.data(),
                    static_cast<DWORD>(buffer.size()),
                    &bytes_read,
                    nullptr))
            {
                throw_windows_error(L"백업 원본 읽기 실패");
            }
            if (bytes_read == 0)
            {
                break;
            }
            write_all(output.get(), buffer.data(), bytes_read);
        }

        if (!FlushFileBuffers(output.get()))
        {
            throw_windows_error(std::wstring(BackupName) + L" 저장 실패");
        }
        output.close();
        input.close();

        if (!fingerprint(backup).matches(ExpectedSize, OriginalSha256))
        {
            throw AppError(std::wstring(BackupName) + L" 검증 결과가 원본과 다릅니다.");
        }
    }
    catch (...)
    {
        if (created)
        {
            try_delete(backup);
        }
        throw;
    }
}

std::filesystem::path create_patched_file(
    const std::filesystem::path& source,
    const std::filesystem::path& directory)
{
    std::vector<wchar_t> temporary_buffer(MAX_PATH);
    if (GetTempFileNameW(directory.c_str(), L"GHI", 0, temporary_buffer.data()) == 0)
    {
        throw_windows_error(L"임시 패치 파일 생성 실패");
    }

    const std::filesystem::path temporary(temporary_buffer.data());
    bool keep = false;
    try
    {
        FileHandle input = open_file(
            source,
            GENERIC_READ,
            FILE_SHARE_READ,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN);
        FileHandle output = open_file(
            temporary,
            GENERIC_READ | GENERIC_WRITE,
            0,
            TRUNCATE_EXISTING,
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH);

        std::vector<unsigned char> buffer(1024 * 1024);
        for (;;)
        {
            DWORD bytes_read = 0;
            if (!ReadFile(
                    input.get(),
                    buffer.data(),
                    static_cast<DWORD>(buffer.size()),
                    &bytes_read,
                    nullptr))
            {
                throw_windows_error(L"패치 원본 읽기 실패");
            }
            if (bytes_read == 0)
            {
                break;
            }
            write_all(output.get(), buffer.data(), bytes_read);
        }

        set_file_position(output.get(), PatchOffset);
        std::array<unsigned char, OriginalBytes.size()> actual = {};
        read_exact(output.get(), actual.data(), actual.size());
        if (actual != OriginalBytes)
        {
            throw AppError(std::wstring(TargetName) + L"의 패치 위치 원본 바이트가 예상과 다릅니다.");
        }

        set_file_position(output.get(), PatchOffset);
        write_all(output.get(), PatchedBytes.data(), PatchedBytes.size());
        if (!FlushFileBuffers(output.get()))
        {
            throw_windows_error(L"임시 패치 파일 저장 실패");
        }
        output.close();
        input.close();

        if (!fingerprint(temporary).matches(ExpectedSize, PatchedSha256))
        {
            throw AppError(L"임시 패치 파일이 예상 결과와 일치하지 않습니다.");
        }
        keep = true;
        return temporary;
    }
    catch (...)
    {
        if (!keep)
        {
            try_delete(temporary);
        }
        throw;
    }
}

void replace_target(
    const std::filesystem::path& replacement,
    const std::filesystem::path& target)
{
    if (!ReplaceFileW(
            target.c_str(),
            replacement.c_str(),
            nullptr,
            REPLACEFILE_IGNORE_MERGE_ERRORS,
            nullptr,
            nullptr))
    {
        throw_windows_error(
            std::wstring(TargetName) +
            L" 교체에 실패했습니다. 게임이 실행 중인지 확인해 주세요");
    }
}

std::filesystem::path executable_directory()
{
    std::vector<wchar_t> buffer(MAX_PATH);
    for (;;)
    {
        SetLastError(ERROR_SUCCESS);
        const DWORD length = GetModuleFileNameW(
            nullptr,
            buffer.data(),
            static_cast<DWORD>(buffer.size()));
        if (length == 0)
        {
            throw_windows_error(L"패처 위치를 확인하지 못했습니다");
        }
        if (length < buffer.size() - 1)
        {
            const std::filesystem::path executable(std::wstring(buffer.data(), length));
            const std::filesystem::path directory = executable.parent_path();
            if (directory.empty())
            {
                throw AppError(L"패처 위치를 확인하지 못했습니다.");
            }
            return directory;
        }
        if (buffer.size() >= 32768)
        {
            throw AppError(L"패처 경로가 너무 깁니다.");
        }
        buffer.resize(buffer.size() * 2);
    }
}

void print_help()
{
    std::wcout << ProductName << L"\n\n";
    std::wcout << L"이 파일을 " << TargetName << L"와 같은 게임 폴더에 놓고 실행합니다.\n";
    std::wcout << L"원본은 " << BackupName << L"으로 백업하고 "
               << TargetName << L"만 패치합니다.\n";
    std::wcout << L"PCK 파일은 읽거나 수정하지 않습니다.\n";
}

void run(int argument_count, wchar_t* arguments[])
{
    if (argument_count > 1)
    {
        const std::wstring_view argument(arguments[1]);
        if (argument == L"-h" || argument == L"--help")
        {
            print_help();
            return;
        }
        if (argument == L"--version")
        {
            std::wcout << ProductName << L' ' << Version << L'\n';
            return;
        }
        throw AppError(L"지원하지 않는 인수입니다: " + std::wstring(argument));
    }

    std::wcout << ProductName << L' ' << Version << L"\n\n";

    const std::filesystem::path directory = executable_directory();
    const std::filesystem::path target = directory / TargetName;
    const std::filesystem::path backup = directory / BackupName;
    if (!file_exists(target))
    {
        throw AppError(
            std::wstring(TargetName) +
            L"를 찾지 못했습니다. 패처를 게임 폴더에 넣고 실행해 주세요.");
    }

    const FileFingerprint current = fingerprint(target);
    if (current.matches(ExpectedSize, PatchedSha256))
    {
        std::wcout << L"[완료] 이미 IME EXE 패치가 적용되어 있습니다.\n";
        report_existing_backup(backup);
        return;
    }
    if (!current.matches(ExpectedSize, OriginalSha256))
    {
        std::wostringstream message;
        message << L"지원하지 않는 " << TargetName
                << L"입니다 (크기 " << current.size
                << L" bytes, SHA-256 " << current.sha256 << L')';
        throw AppError(message.str());
    }

    verify_preimage(target);
    ensure_backup(target, backup);
    std::wcout << L"[백업] " << BackupName << L'\n';

    const std::filesystem::path temporary = create_patched_file(target, directory);
    try
    {
        replace_target(temporary, target);
    }
    catch (...)
    {
        try_delete(temporary);
        throw;
    }
    try_delete(temporary);

    const FileFingerprint final = fingerprint(target);
    if (!final.matches(ExpectedSize, PatchedSha256))
    {
        throw AppError(
            L"패치 결과가 예상과 다릅니다. 원본은 " +
            std::wstring(BackupName) + L"에 보존되어 있습니다.");
    }

    std::wcout << L"[완료] IME EXE 패치를 적용했습니다.\n";
    std::wcout << L"[확인] SHA-256 " << final.sha256 << L'\n';
    std::wcout << L"[안내] PCK 파일은 변경하지 않았습니다.\n";
}

void enable_utf8_console()
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    _setmode(_fileno(stdin), _O_U8TEXT);
    _setmode(_fileno(stdout), _O_U8TEXT);
    _setmode(_fileno(stderr), _O_U8TEXT);
}
}

int wmain(int argument_count, wchar_t* arguments[])
{
    enable_utf8_console();

    int result = 0;
    try
    {
        run(argument_count, arguments);
    }
    catch (const AppError& error)
    {
        std::wcout << L"\n[오류] " << error.message() << L'\n';
        result = 1;
    }
    catch (const std::exception& error)
    {
        std::wcout << L"\n[오류] 예기치 않은 C++ 오류: " << error.what() << L'\n';
        result = 1;
    }
    catch (...)
    {
        std::wcout << L"\n[오류] 알 수 없는 오류가 발생했습니다.\n";
        result = 1;
    }

    if (argument_count == 1)
    {
        std::wcout << L"\nEnter 키를 누르면 종료합니다.";
        std::wstring line;
        std::getline(std::wcin, line);
    }
    return result;
}
