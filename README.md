# The Incident at Galley House 한글패치 (베타 버전)
(최신 릴리즈: 26-08-06 18:50)
- 스팀 게임 [**The Incident at Galley House**](https://store.steampowered.com/app/3641000) (갤리 저택 사건)의 유저 한글패치입니다.  
- AI 번역으로 우선 배포 후, 손번역으로 텍스트 및 이미지 교체 작업 중에 있습니다. 자세한 내용은 **주요 안내 사항** 및 **업데이트 내역**을 참고 바랍니다.

### 다운로드
- [최신 릴리즈](https://github.com/mushsooni/The-Incident-at-Galley-House_kor/releases/latest)에서 `Galley-House_KO_{버전명}.zip`과 `GalleyHouse_IME_EXE_Patcher_{버전명}.exe` 다운로드

### 패치 방법
1. `zip` 파일의 압축을 풀고, `galleyhouse.pck` 파일을 게임 폴더에 **덮어쓰기**  
   (게임 폴더: 스팀 라이브러리 - 게임명 우클릭 - `관리` - `로컬 파일 탐색`)
2. **(선택)** `exe` 파일을 게임 폴더에 놓고 실행  
   `Windows의 PC 보호` 창이 뜨는 경우 `추가 정보` - `실행` 클릭
3. 게임 실행 후 `Settings` - `language`를 `한국어`로 변경
4. 게임의 안내에 따라 **게임 재실행**

### 주요 안내 사항
- 본 패치는 **윈도우** 버전 게임을 기준으로 제작되었습니다.
- `GalleyHouse_IME_EXE_Patcher.exe`는 **이동 입력 유지 버그** 관련 코드를 수정하는 패처입니다.  
  자세한 설명은 하단의 **알려진 문제** 및 [src/ime_exe_patcher](https://github.com/mushsooni/The-Incident-at-Galley-House_kor/tree/main/src/ime_exe_patcher)를 참고하시기 바랍니다.
- 게임 업데이트 시 한글패치가 풀리고 중국어로 출력될 수 있습니다.  
  `galleyhouse.pck` 파일을 **다시 덮어씌우면** 한국어로 복원되지만, 게임의 업데이트 내역은 반영되지 않을 수 있습니다.
- 현재 대부분 AI 번역이 적용되어 번역의 질이 다소 미흡할 수 있습니다. v0.1 공개일 기준 약 두 달에 걸쳐 **손번역**으로 교체될 예정입니다.
- 현재 이미지 에셋 중 일부는 영어로 출력됩니다. 게임 내 **오버레이 자막** 기능을 사용하시기 바랍니다.  
  손번역 일정에 맞추어 한국어 이미지로 교체 작업 중입니다.

### ★ 알려진 문제 ★
- 수첩에서 한글 입력 중에 수첩을 닫고 방향키를 누르면, **이동 입력이 유지**되는 버그가 있습니다.  
  같은 방향키를 **다시 누르면** 문제 증상을 멈출 수 있습니다.

해당 이슈는 한글 입력을 처리하는 게임 엔진 상의 버그로 추정됩니다. `GalleyHouse_IME_EXE_Patcher.exe`는 해당 문제를 수정하는 EXE 패처입니다.

### 게임 원본 빌드
- **기준일**: 한국시간 26-07-29 23:39:31
- **App ID**: 3641000 / **Depot ID**: 3641001 / **Manifest ID**: 7053694967861353130 / **Build ID**: 24454079

### 문의, 제보
- **배포 안내글**: https://gall.dcinside.com/indiegame/448576
- **깃허브 이슈**: https://github.com/mushsooni/The-Incident-at-Galley-House_kor/issues/new/choose

### 패치 제작
- **버섯순이**
- [**Snowyegret**](https://github.com/snowyegret23) (IME 버그 관련 기술 지원)

### Special Thanks
- **Erica Noh** ([*Type Help*](https://william-rous.itch.io/type-help) 번역)  
원작의 한국어 번역을 많이 참고하였습니다. 멋진 게임을 한국어로 소개해주셔서 정말 감사합니다.

### 업데이트 내역
- 2026-07-17 [v0.1] **베타 버전 공개**: 모든 폰트에 개별 폴백 폰트 적용, 어절 단위 줄바꿈 적용, 영어 이미지 적용, 영어 이미치 출력 코드 적용
- 2026-07-18 [v0.2] 게임 업데이트 대응, Dumbwaiter 번역 수정
- 2026-07-19 [v0.3] 게임 업데이트 대응, [person] 및 %s 태그 뒤 한국어 조사 자동 변환
- 2026-07-21 [v0.4] 게임 업데이트 대응, 게임 타이틀 등 일부 이미지 한글화, UI 및 튜토리얼 일부 번역 검수
- 2026-07-23 [v0.4.1] 이미지 다수 한글화, 튜토리얼 번역 검수, 일부 용어 수정, 수첩 폰트 기울기 해제
- 2026-07-25 [v0.4.2] 초반 장면 번역 검수, 용어 및 인물 간 말투 일부 수정, 이미지 다수 한글화
- 2026-07-28 [v0.5] IME 버그 수정, 번역 검수, 이미지 한글화
- 2026-07-29 [v0.6] 게임 업데이트 대응, 폰트 자간 수정, 번역 검수
- 2026-07-30 [v0.7] 게임 업데이트 대응, 번역 검수
- 2026-08-03 [v0.7.1] 용어 및 인물 간 말투 일부 수정, 장소명 이미지 중앙 정렬, 기타 번역 검수
- 2026-08-06 [v0.7.2] 용어 및 인명 수정, 이미지 한글화, 기타 번역 검수

자세한 변경 사항 및 진행도는 [Releases](https://github.com/mushsooni/The-Incident-at-Galley-House_kor/releases)를 참고 바랍니다.

### 사용된 폰트

**Noto Serif Korean** (Light, SemiBold, Bold, Black)  
Copyright 2012 Google Inc. All Rights Reserved.  
https://fonts.google.com/noto/specimen/Noto+Serif+KR

**KCC방정환체**  
https://gongu.copyright.or.kr/gongu/wrt/wrt/view.do?wrtSn=13372653&menuNo=200023

**전기안전체** (Medium)  
본 저작물은 '한국전기안전공사'에서 '2024년' 작성하여 공공누리 제1유형으로 개방한 '전기안전체'를 이용하였으며, 해당 저작물은 '전기안전 홍보센터'에서 무료로 다운받으실 수 있습니다.  
https://kesco.or.kr/bbs/pr/selectBbs.do?bbs_code=PRB00012&bbs_seq=167561

**온글잎 강동희**  
Designed by Kan*** in 2023. Published by Ownglyph  
https://www.ownglyph.com/trial/d318ba11-71ec-4c8c-a5de-55fa37373862

**DX타자** (Medium)  
https://www.dxkorea.co.kr/goods/goods_view.php?goodsNo=1000000037&pageNm=font  
https://www.dxkorea.co.kr/main/html.php?htmid=/service/freefont.html

**가평한석봉** (큰붓B)  
본 패치의 텍스처 에셋 중 일부는 가평한석봉 큰붓B를 사용하여 제작되었습니다.  
https://www.gp.go.kr/portal/contents.do?key=2410

**굵은둥근모**  
https://github.com/sawalk/Korean-Font-Change/releases/tag/Bold-v0.2

**그리운 옥수니 손글씨**  
https://www.griun.co.kr/fonts/oksooni

**에스코어 드림** (8 Heavy)  
https://s-core.co.kr/company/font2/

**영월체**  
https://www.yw.go.kr/www/contents.do?key=1500

**인천교육힘찬체**  
https://www.ice.go.kr/ice/cm/cntnts/cntntsView.do?mi=10874&cntntsId=943

**조선궁서체**  
https://event.chosun.com/100/100font.html

**카페24 PRO Slim Fit**  
https://fonts.cafe24.com/

**DX일급비밀** (Medium)  
https://www.dxkorea.co.kr/goods/goods_view.php?goodsNo=1000000245&pageNm=font  
https://www.dxkorea.co.kr/main/html.php?htmid=/service/freefont.html

**DX활판명조** (Medium, Bold)  
https://www.dxkorea.co.kr/goods/goods_view.php?goodsNo=1000000042&pageNm=font  
https://www.dxkorea.co.kr/main/html.php?htmid=/service/freefont.html

**Galmuri7**  
© 2019–2025 Lee Minseo (quiple​@​quiple​.​dev)  
https://quiple.dev/font/galmuri

**LAB디지털**  
https://fontlab.kr/notice/?uid=11&mod=document  
https://fontlab.kr/notice/?uid=9&mod=document

**Noto Sans Korean** (Bold)  
Copyright 2014-2021 Adobe (http://www.adobe.com/), with Reserved Font Name 'Source'  
https://fonts.google.com/noto/specimen/Noto+Sans+KR

### 기타 안내 사항
- 본 패치는 원작자의 허가 하에 제작되었으며, 제작사/유통사의 요청이 있을 경우 언제든 내려갈 수 있습니다.
- 무단 재배포를 금합니다. 수정 후 재배포의 경우 사전 문의 바랍니다.
- 기타 번역 누락, 오역, 버그 발견 시 제보를 부탁드립니다.
