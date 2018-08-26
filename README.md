[![license](https://img.shields.io/badge/license-MIT-brightgreen.svg)](https://shields.io/) [![language](https://img.shields.io/badge/language-C%2B%2B17-blue.svg)](https://shields.io/)
# 아희++
아희와 호환성이 있는 난해한 객체지향 한글 프로그래밍 언어
- 인터프리터 버전: 2.0.0 (불안정한 버전)
	- 다른 버전: [1.2.1](https://github.com/kmc7468/Aheuiplusplus/tree/version/1.2.1)
- 아직 개발중인 버전입니다.
## [레퍼런스](https://github.com/kmc7468/Aheuiplusplus/wiki)
아희++의 표준안과 예제를 수록하고 있습니다. 표준안에 애매한 내용이나 질문이 있다면 이슈 등을 통해 알려주시면 감사하겠습니다.
## 아희++ 표준 인터프리터의 특징
- **강력한 유니코드 지원**<br>
코드에 이모지 등의 2개 이상의 코드 포인트로 구성되는 다양한 유니코드 문자를 사용해도 1글자로 정상적으로 인식합니다.
- **범용성**<br>
아희++은 물론이며, 아희도 인터프리팅 할 수 있습니다.
- **통로 지원**<br>
아희 구현체 최초로 통로를 *제대로* 지원합니다. C++로 작성된 '아희++ 표준 인터프리터 확장'을 연결하면 통로를 통해 확장과 통신할 수 있습니다. C++을 사용할 수 있다면 누구나 확장을 만들 수 있습니다.
## 컴파일
### 필요한 소프트웨어
- CMake 3.8.0 이상
- C++17 이상을 지원하는 C++ 컴파일러
### 컴파일 방법
1. 이 레포지토리를 로컬에 복제합니다.
2. 복제된 디렉토리 내부에 있는 CMakeLists.txt 파일을 CMake로 실행합니다.
3. CMake가 생성한 빌드 스크립트를 적절한 소프트웨어로 실행합니다.
### Git+Makefile
```
$ git clone -b stable https://github.com/kmc7468/Aheuiplusplus.git
$ cd ./Aheuiplusplus
$ cmake CMakeLists.txt
$ make
```
`-b stable` 옵션은 릴리즈 중 *정식 버전만* 보았을 때, 가장 최신의 릴리즈의 소스 코드를 복제하도록 하는 옵션입니다. `-b pre-release` 옵션으로 수정할 경우 모든 릴리즈 중 가장 최신의 릴리즈의 소스 코드를 복제하게 되며, 옵션을 삭제할 경우 여기에 릴리즈 되지 않은 소스 코드도 포함해 가장 최신의 소스 코드를 복제하게 됩니다. 옵션을 삭제하는 것은 권장되지 않습니다.
### CMake 옵션
- `COMPILE_TARGET`:<br>
아희++ 표준 인터프리터를 실행 파일의 형태로 컴파일 할지, 정적 라이브러리의 형태로 컴파일 할지 설정하는 옵션입니다.
	- 값은 `Executable`, `Library` 중 하나이며, 대소문자는 구분되지 않습니다. 전자는 실행 파일, 후자는 정적 라이브러리의 형태를 의미합니다.
## 예제
더 많은 예제는 레퍼런스에서 확인하실 수 있습니다.
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
## 외부 라이브러리
아래에 열거된 외부 라이브러리들은 아희++ 표준 인터프리터를 컴파일 할 때 같이 컴파일 되므로 추가적인 작업이 필요하지 않습니다.
- [u5e](https://github.com/ruoso/u5e)의 커밋 [3b970d5](https://github.com/ruoso/u5e/tree/3b970d5bc251fdef341d039d66c84ec5eaf4cb6a) - 2-clause BSD license
	- include/u5e/basic_grapheme_iterator.hpp 파일의 159번 줄 및 170번 줄이 수정되었습니다. (커밋 [abca129](https://github.com/kmc7468/Aheuiplusplus/commit/abca1292fe6c421d835516e00b33d62ae5710200))
- [UTF8-CPP](https://github.com/nemtrif/utfcpp) 2.3.5
## 라이선스
아희++ 표준 인터프리터의 모든 소스 코드는 MIT 라이선스가 적용됩니다. 단, 외부 라이브러리에는 적용되지 않습니다.
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