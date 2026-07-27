# GalleyHouse IME EXE Patcher

수첩을 닫은 직후 첫 입력의 KEYUP이 누락되는 문제를 수정하는 Windows x64 네이티브 EXE 패처입니다.

## 사용법

1. 게임을 종료합니다.
2. `GalleyHouse_IME_EXE_Patcher.exe`를 `galleyhouse.exe`와 같은 게임 폴더에 둡니다.
3. 패처를 실행합니다.
4. 성공하면 원본은 `galleyhouse.exe.backup`으로 남고 `galleyhouse.exe`에 패치가 적용됩니다.

기존 백업 파일이 지원 원본과 일치하지 않으면 덮어쓰지 않고 중단합니다.

이 패처는 `galleyhouse.pck`를 읽거나 수정하지 않습니다. 이미 패치된 EXE에는 다시 패치하지 않습니다.

지원 원본:

- 크기: 104,534,016 bytes
- SHA-256: `66A57A2033E0A7BC9418C45EE4DD32678DB62F00A2D76485172FCBD894BD37A5`

패치 결과:

- 크기: 104,534,016 bytes
- SHA-256: `9996BF1F5C715570BD066CF91ADCE5AE6AE8DDCE5129E661E8E169D4FC06E54F`

압축, 난독화, 관리형 런타임 번들링을 사용하지 않은 Windows x64 네이티브 실행 파일입니다. 검증된 원본 백업을 만든 뒤 같은 폴더에 패치된 임시 EXE를 만들고 전체 결과 hash를 확인한 후 `ReplaceFileW`로 원본 EXE를 교체합니다.

## 빌드

Visual Studio의 C++ 데스크톱 빌드 도구가 필요합니다.

```powershell
.\build.ps1
```

배포 파일은 `dist/`에 생성됩니다.
