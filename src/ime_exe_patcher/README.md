# GalleyHouse IME EXE Patcher

수첩을 닫은 직후 첫 입력의 KEYUP이 누락되는 문제를 수정하는 Windows x64 네이티브 EXE 패처입니다.

## 사용법

1. 게임을 종료합니다.
2. `GalleyHouse_IME_EXE_Patcher_v1.0.1.0.exe`를 `galleyhouse.exe`와 같은 게임 폴더에 둡니다.
3. 패처를 실행합니다.
4. 성공하면 원본은 `galleyhouse.exe.backup`으로 남고 `galleyhouse.exe`에 패치가 적용됩니다.

기존 백업 파일이 현재 또는 알려진 이전 지원 원본과 일치하지 않으면 덮어쓰지 않고 중단합니다. 이전 지원 빌드의 백업은 `galleyhouse.exe.backup.24297351`로 보존하고 새 원본 백업을 만듭니다.

이 패처는 `galleyhouse.pck`를 읽거나 수정하지 않습니다. 이미 패치된 EXE에는 다시 패치하지 않습니다.

지원 원본:

- 크기: 104,549,664 bytes
- SHA-256: `5B24095A3B83005FD8C114E628EA7377D0D288010534C9F64EDF8E9375176C37`

패치 결과:

- 크기: 104,549,664 bytes
- SHA-256: `499A37B212DB7EC9ED4FB793DC5E529E211E73FDC1FB5D9E2D8F125FC2C6889B`

압축, 난독화, 관리형 런타임 번들링을 사용하지 않은 Windows x64 네이티브 실행 파일입니다. 검증된 원본 백업을 만든 뒤 `galleyhouse.exe`의 패치 위치를 다시 확인하고 20바이트만 직접 수정합니다.

## 빌드

Visual Studio의 C++ 데스크톱 빌드 도구가 필요합니다.

```powershell
.\build.ps1
```

배포 파일은 `dist/`에 생성됩니다.
