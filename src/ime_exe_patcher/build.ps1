$ErrorActionPreference = "Stop"

$vswhere = Join-Path ${env:ProgramFiles(x86)} "Microsoft Visual Studio\Installer\vswhere.exe"
if (-not (Test-Path -LiteralPath $vswhere -PathType Leaf)) {
    throw "Visual Studio Installer의 vswhere.exe를 찾지 못했습니다."
}

$installation = & $vswhere `
    -latest `
    -products * `
    -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
    -property installationPath
if ($LASTEXITCODE -ne 0 -or [String]::IsNullOrWhiteSpace($installation)) {
    throw "x64 C++ 빌드 도구를 찾지 못했습니다."
}

$vsDevCmd = Join-Path $installation.Trim() "Common7\Tools\VsDevCmd.bat"
if (-not (Test-Path -LiteralPath $vsDevCmd -PathType Leaf)) {
    throw "VsDevCmd.bat를 찾지 못했습니다: $vsDevCmd"
}

$environmentCommand = "`"$vsDevCmd`" -no_logo -arch=x64 -host_arch=x64 >nul && set"
$environmentLines = & $env:ComSpec /d /s /c $environmentCommand
if ($LASTEXITCODE -ne 0) {
    throw "x64 C++ 빌드 환경 초기화에 실패했습니다."
}

foreach ($line in $environmentLines) {
    $separator = $line.IndexOf("=")
    if ($separator -gt 0) {
        $name = $line.Substring(0, $separator)
        $value = $line.Substring($separator + 1)
        [Environment]::SetEnvironmentVariable($name, $value, "Process")
    }
}

$compiler = (Get-Command "cl.exe" -ErrorAction Stop).Source
$resourceCompiler = (Get-Command "rc.exe" -ErrorAction Stop).Source
$build = Join-Path $PSScriptRoot "build"
$dist = Join-Path $PSScriptRoot "dist"
$source = Join-Path $PSScriptRoot "Program.cpp"
$resourceSource = Join-Path $PSScriptRoot "version.rc"
$resource = Join-Path $build "version.res"
$object = Join-Path $build "Program.obj"
$output = Join-Path $dist "GalleyHouse_IME_EXE_Patcher.exe"

New-Item -Path $build -ItemType Directory -Force | Out-Null
New-Item -Path $dist -ItemType Directory -Force | Out-Null

& $resourceCompiler `
    /nologo `
    "/fo$resource" `
    $resourceSource
if ($LASTEXITCODE -ne 0) {
    throw "리소스 빌드에 실패했습니다 (종료 코드 $LASTEXITCODE)."
}

$compilerArguments = @(
    "/nologo",
    "/std:c++20",
    "/O2",
    "/GL",
    "/EHsc",
    "/W4",
    "/WX",
    "/permissive-",
    "/utf-8",
    "/MT",
    "/GS",
    "/guard:cf",
    "/DUNICODE",
    "/D_UNICODE",
    "/Fo$object",
    "/Fe$output",
    $source,
    $resource,
    "/link",
    "bcrypt.lib",
    "/LTCG",
    "/OPT:REF",
    "/OPT:ICF",
    "/DYNAMICBASE",
    "/NXCOMPAT",
    "/HIGHENTROPYVA",
    "/MANIFEST:EMBED",
    "/MANIFESTUAC:level='asInvoker' uiAccess='false'",
    "/SUBSYSTEM:CONSOLE",
    "/INCREMENTAL:NO"
)

& $compiler @compilerArguments
if ($LASTEXITCODE -ne 0) {
    throw "C++ 빌드에 실패했습니다 (종료 코드 $LASTEXITCODE)."
}

Copy-Item -LiteralPath (Join-Path $PSScriptRoot "README.md") -Destination $dist -Force

$artifact = Get-Item -LiteralPath $output
$hash = Get-FileHash -LiteralPath $output -Algorithm SHA256
Write-Output "Built: $($artifact.FullName)"
Write-Output "Size: $($artifact.Length) bytes"
Write-Output "SHA-256: $($hash.Hash)"
