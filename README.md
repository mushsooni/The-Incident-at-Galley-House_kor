# The Incident at Galley House 한글패치 (베타 버전)
- 스팀 게임 [**The Incident at Galley House**](https://store.steampowered.com/app/3641000) (갤리 저택 사건)의 유저 한글패치입니다.  
- AI 번역본으로 우선 배포 후, 손번역으로 텍스트 및 이미지 교체 작업 중에 있습니다. 자세한 내용은 **주요 안내 사항** 및 **업데이트 내역**을 참고 바랍니다.

### 다운로드
https://github.com/mushsooni/The-Incident-at-Galley-House_kor/releases/download/v0.2/Galley-House_KO_v0.2.zip

### 패치 방법
1. 압축을 풀고, `galleyhouse.pck` 파일을 게임 폴더에 **덮어쓰기**  
   (게임 폴더: 스팀 라이브러리 - 게임명 우클릭 - `관리` - `로컬 파일 탐색`)
2. 게임 실행 후 `Settings` - `language`를 `한국어`로 변경
3. 게임의 안내에 따라 설정 적용 후 **게임 재실행**

### 주요 안내 사항
- 본 패치는 **윈도우** 버전 게임을 기준으로 제작되었습니다.
- 본 패치는 중국어 슬롯을 사용하며 다른 언어 플레이에 영향을 미치지 않습니다.  
  즉, 한글패치 후에도 영어, 일본어 등 다른 언어로 플레이할 수 있습니다.
- 게임 업데이트 시 한글패치가 풀리고 **중국어로 출력**될 수 있습니다.  
  한글패치 파일을 다시 덮어씌우면 한국어로 복원되지만, 게임의 업데이트 내역은 반영되지 않을 수 있습니다.  
- 대규모 업데이트로 인해 위 방법으로도 한글패치가 불가능한 경우, 아래 **문의, 제보** 링크로 제보 바랍니다.  
  (**게임 원본 빌드**를 참고하여 [버전을 롤백](https://blog.naver.com/toruin84/222898486431)하면 임시로 플레이가 가능합니다.)
- 현재 AI 번역이 적용되어 번역의 질이 다소 미흡할 수 있습니다. v0.1 공개일 기준 약 두 달에 걸쳐 손번역으로 교체될 예정입니다.
- 현재 이미지 에셋은 영어로 출력됩니다. 손번역 일정에 맞추어 한국어 이미지로 함께 교체될 예정입니다.

### ★ 알려진 문제 ★
1. 수첩에서 한글 조합을 완성하지 않은 채 수첩을 닫은 뒤 방향키를 누르면, 같은 키를 다시 누를 때까지 **이동 입력이 유지**되는 버그가 있습니다.  
  같은 방향키를 **다시 누르면** 문제 증상을 멈출 수 있습니다.
2. 수첩에서 한글 조합을 완성하지 않은 채 다른 행을 클릭하면, 입력 중인 글자가 클릭한 행으로 이동하는 버그가 있습니다.
3. 드래그 도중 엔터키를 누르거나 허공에 엔터키를 누를 시 **드래그가 되지 않는 버그**가 있습니다.  
  **엔터키**를 다시 한 번 누르면 문제 증상을 멈출 수 있습니다.
4. '은/는', '이/가'와 같은 조사가 앞에 오는 단어와 대응하지 않는 문제가 있습니다.

- **1, 2번** 이슈는 한글 입력에서만 발생하는 고도 엔진의 IME(입력 방식 편집기) 자체의 버그로 추정됩니다. 다른 분께 제공받은 수정 코드를 바탕으로 플레이 테스트 중입니다.
- **3번** 이슈는 게임 자체의 버그로 추정됩니다. 해당 이슈는 본 패치에서는 수정 계획이 없으니 양해 바랍니다.
- **4번** 이슈는 추후 업데이트를 통해 수정될 예정입니다.

### 게임 원본 빌드
- **기준일**: 2026-07-17 23:02:19 KST (한국시간)
- **App ID**: 3641000 / **Depot ID**: 3641001 / **Build ID**: 24261359 / **Manifest ID**: 1228548973922031401
- **SteamDB**: https://steamdb.info/depot/3641001/history/?changeid=M:1228548973922031401
- **버전 롤백 명령어** : `download_depot 3641000 3641001 1228548973922031401`

### 문의, 제보
- **배포 안내글**: https://gall.dcinside.com/indiegame/448576
- **깃허브 이슈**: https://github.com/mushsooni/The-Incident-at-Galley-House_kor/issues/new/choose

### 패치 제작
- **버섯순이**

### Special Thanks
- **Erica Noh** ([*Type Help*](https://william-rous.itch.io/type-help) 번역)  
원작의 한국어 번역을 많이 참고하였습니다. 멋진 게임을 한국어로 소개해주셔서 정말 감사합니다.
- [**Snowyegret**](https://github.com/snowyegret23)  
IME와 드래그 관련 버그의 트리거를 찾고 해결 방법과 수정 코드를 제공해주셨습니다. 진심으로 감사드립니다.

### 업데이트 내역
- 2026-07-17 [v0.1] **베타 버전 공개**: 모든 폰트에 개별 폴백 폰트 적용, 어절 단위 줄바꿈 적용, 영어 이미지 적용, 영어 이미치 출력 코드 적용
- 2026-07-18 [v0.2] 게임 업데이트 대응(Build 24261359), Dumbwaiter 번역 수정

### 사용된 폰트

**Noto Serif Korean** (SemiBold)  
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

### 기타 안내 사항
- 본 패치는 원작자의 허가 하에 제작되었으며, 제작사/유통사의 요청이 있을 경우 언제든 내려갈 수 있습니다.
- 무단 재배포를 금합니다. 수정 후 재배포의 경우 사전 문의 바랍니다.
- 기타 번역 누락, 오역, 버그 발견 시 제보를 부탁드립니다.

