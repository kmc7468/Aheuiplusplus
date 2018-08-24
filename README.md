[![license](https://img.shields.io/badge/license-MIT-brightgreen.svg)](https://shields.io/) [![language](https://img.shields.io/badge/language-C%2B%2B17-blue.svg)](https://shields.io/)
# 아희++
아희와 호환성이 있는 난해한 객체지향 한글 프로그래밍 언어
- 인터프리터 버전: 1.2.1
- **이 브랜치에는 추가적인 커밋이 없을 예정입니다. [이곳](https://github.com/kmc7468/Aheuiplusplus)에서 최신 소스 코드를 확인하실 수 있습니다.**<br>
## 사용 방법
아희++ 표준 구현체(이하 "인터프리터")는 작동 모드를 크게 인터프리팅 모드 및 일반 모드로 나눌 수 있습니다.
### 인터프리팅 모드
```
$ ./Aheuiplusplus -i
```
위 명령을 입력하면 인터프리팅 모드로 인터프리터를 시작할 수 있습니다.
#### 명령어
- `!q`, `!quit`<br>
인터프리터를 종료합니다.
- `!help`<br>
명령어 목록을 봅니다.
- `!clear`<br>
화면을 비웁니다.
<br><br>
- `!d`, `!dump`<br>
전체 저장공간 상태를 덤프합니다.
<br><br>
- 이 외의 명령어는 인터프리터에서 `!help` 명령어를 사용하여 확인하실 수 있습니다.
### 일반 모드
```
$ ./Aheuiplusplus path
```
위 명령을 입력하면 일반 모드로 인터프리터를 시작할 수 있습니다. path는 BOM이 없는 UTF-8로 인코딩 된 아희++ 코드가 담긴 파일의 경로입니다. 그 파일의 줄바꿈 형식은 CRLF(`"\r\n"`) 또는 LF(`'\n'`)로 통일되어 있어야 합니다.
### 명령줄 옵션
- `--help`<br>
명령줄 옵션 목록을 봅니다.
- `--vesion`<br>
인터프리터의 버전을 봅니다.
<br><br>
- `-A`<br>
아희 표준대로만 작동하도록 합니다(아희++의 기능을 이용할 수 없습니다.).
<br><br>
- 이 외의 명령줄 옵션은 인터프리터 명령줄에서 `--help` 옵션을 사용하여 확인하실 수 있습니다.
## 컴파일 방법
### 필요한 도구
- CMake 3.8.0 이상
- C++17 이상을 지원하는 C++ 컴파일러
### 레포지토리 복제
```
$ git clone -b stable https://github.com/kmc7468/Aheuiplusplus.git
```
위 명령을 입력하면 가장 최신의 안정된 인터프리터 소스 파일을 복제할 수 있습니다. 만약 가장 최신의 프리릴리즈를 포함한 안정된 소스 파일을 복제하고 싶으시면 `-b stable` 옵션을 `-b pre-release`로 변경하시고, 항상 가장 최신의 소스 파일을 복제하고 싶다면 위 명령에서 `-b stable` 옵션을 삭제하십시오.
### 컴파일(Makefile 이용)
```
$ cmake CMakeLists.txt
$ make
```
위 명령들을 입력하면 인터프리터 소스 파일을 실행 파일로 컴파일 할 수 있습니다.
### CMake 옵션 목록
- `COMPILE_TARGET`<br>
이 인터프리터를 실행 파일로 컴파일 할 지, 라이브러리로 컴파일 할 지 설정합니다.
	- 값은 `"실행 파일"`, `"정적 라이브러리"` 중 하나입니다.
## [레퍼런스](https://github.com/kmc7468/Aheuiplusplus/wiki)
아희++ 표준 및 예제를 수록하고 있습니다. 만약 구현체 개발을 진행하시다가 레퍼런스만 보고 동작을 결정하기 어려운 사안이 있다면 개발자에게 알려주시면 감사하겠습니다.
## 예제
더 많은 예제는 레퍼런스를 참조해 주십시오.
### [개발자 수다방](https://gist.github.com/RanolP/6ecb4b1030fccad19dc05f3716d6c2c7) by [RanolP](https://gist.github.com/RanolP)
```
개반뭉반붓밪두빥붖빠뭏밠뭉박누망뭏따뿌삭뿌밪붅파투밣뚜타댜뎌뭏뷺다두타두밢두밙뚜빥푸다뿑빠뿌빥분받뚜삽쑤밪불빥두받투밧누
발꾔바몽나몽망봀타뽀바몽맣본빠몽밤봃싹뫃빠소따뽅빥볼타빠쑺봃밠뽅소두봎뭏또두볻두봃쑵봃붖뽀뿌토붅또투도수소뚜도푸토뭏본뭉
자두변번뻕떠벌벚멓더떠벓벐더머퍼뻕더뻕벒뻕더벇뻕떠벐번멓서볻퍼두뫃불포두봀뭏뽅뭏뽅투뫃불속뭏볾뚜쏩뭏뽅투뫃뿑노투도분소붋
수뺝리밪밤따다맣밪타빥밠빥파타반밧나타타삭맣사맣밢타빥맣발다뽅맣속타뽅빥본밦토밦도밞토따도사뫃빠뽀밦도맣속반봇밠뽅삭뫃뿌
다총통각하만세삼창해멓북번붏멓뚜벖두뻕숙멓붊번붇썩투퍼투너뚜벓수멓두번푸뻕푸터두번불벚두벘뿑벐뿑더뿑벑숮멓투떠붍번뿌떠붐
방망희됴아하는난로당도너또범토더봆벌토벌토더토너뽀퍼뽅터봇번볻뻐속멓토머볾터포뻕뽅떠뫃더토더토퍼본더뫃뻐속멓봆더도뻕또더
```
출력: `2018.07.11. 개발자 수다방: 텔레그램 에디션 제 1회 개천절 경축!`
## 라이선스
MIT 라이선스를 채택하고 있습니다.
```
MIT License

Copyright (c) 2018 kmc7468

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```