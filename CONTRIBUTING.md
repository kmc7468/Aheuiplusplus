# 아희++ 표준 인터프리터에 대한 기여에 관한 작은 규칙
최신 버전이 아닐 수 있습니다. 최신 버전은 [이곳](https://github.com/kmc7468/Aheuiplusplus/blob/master/CONTRIBUTING.md)에서 확인할 수 있습니다.
## 기본적인 예의
- 커밋 제목 및 메세지, 커밋에 포함된 기여 내용 등 모든 부분에서 기본적인 예의를 준수하여 주십시오.
	- 예를 들어, 비존대어를 사용하거나, 비속어를 사용하지 마십시오.
- 커밋 제목 및 메세지, 커밋에 포함된 기여 내용 등 모든 부분에서 인권 및 기여자의 소속 국가의 법률을 준수하여 주십시오.
	- 예를 들어, 라이선스 문제가 있는 소스 코드를 사용하거나, 커밋 메세지에 차별적 표현을 사용하지 마십시오.
## 파일
- 모든 텍스트 파일은 **BOM이 있는 UTF-8**로 인코딩 해 주십시오.
- 모든 헤더 파일 및 소스 파일의 줄바꿈 형식은 **CRLF**(`"\r\n"`)로 해주십시오.
## 커밋
- 커밋 제목 및 메세지는 반드시 **한국어의 표준어**로 작성해 주십시오.
- 커밋 제목은 동사로 끝나는 명사형(예: ~ 수정, ~ 업데이트, ~ 개선 등)을 사용해 주십시오.
- 반드시 master 브랜치에만 커밋하여 주십시오.
## PR과 이슈
- 반드시 master 브랜치에 대하여 열어 주십시오.
- PR은 반드시 수락되는 것은 아님에 유의하십시오.
## 프로그래밍
- 소스 코드는 반드시 크로스 플랫폼이 가능하도록 프로그래밍 하여 주십시오.
## 브랜치
- **master 브랜치**<br>
주 브랜치로, 모든 커밋은 반드시 이 브랜치에만 해야 합니다.
- **stable 브랜치**<br>
master 브랜치에서 버전의 개발이 완료되어 릴리즈를 할 준비가 완료되었으며, 정식 출시가 가능한 안정화 된 버전일 경우 master 브랜치에서 stable 브랜치로 커밋을 병합합니다.
- **pre-release 브랜치**<br>
master 브랜치에서 버전의 개발이 완료되어 릴리즈를 할 준비가 완료되었으나, 정식 출시 전 프리릴리즈일 경우 master 브랜치에서 pre-release 브랜치로 커밋을 병합합니다.
### 브랜칭 전략
- 신규 기능을 구현할 때, 작업이 오래 걸릴 것으로 보이는 기능을 구현한다면 `"feature/(기능 이름)"`의 이름을 가진 브랜치를 master 브랜치에서 분기시킬 수 있습니다. 작업이 끝난 후에는 다시 master 브랜치로 병합시켜야 합니다. 병합을 한 후에는 브랜치를 삭제합니다.
	- 기능 이름은 알파벳 소문자, 언더바(`'_'`)로만 이루어진 명령문 형태의 영어 문장으로 되어 있어야 합니다. 문장은 최대한 간결하게 만듭니다. 예를 들어, `encoding` 클래스를 추가하는 작업을 할 예정이라면, 브랜치 이름을 `"feature/add_encoding_class"`로 지으면 됩니다.
	- 해당 브랜치에는 해당 기능 구현과 관련 없는 작업은 하지 마십시오.
	- 해당 브랜치에는 예외적으로 이슈와 PR을 넣을 수 있습니다.
- 버그를 수정할 때, 작업이 오래 걸릴 것으로 보이는 기능을 구현한다면 `"bugfix/(버그 이름)"`의 이름을 가진 브랜치를 master 브랜치에서 분기시킬 수 있습니다. 작업이 끝난 후에는 다시 master 브랜치로 병합시켜야 합니다. 병합을 한 후에는 브랜치를 삭제합니다.
	- 버그 이름은 알파벳 소문자, 언더바(`'_'`)로만 이루어진 명령문 형태의 영어 문장으로 되어 있어야 합니다. 문장은 최대한 간결하게 만듭니다. 예를 들어, 리눅스에서 문자 입력이 되지 않는 버그를 수정할 예정이라면, 브랜치 이름을 `"bugfix/cannot_read_character"`로 지으면 됩니다.
	- 해당 브랜치에는 예외적으로 이슈와 PR을 넣을 수 있습니다.
- 이미 출시된 릴리즈에 심각한 버그가 있을 경우 master 브랜치에서 stable 또는 pre-release 브랜치로 병합하는 커밋(안정된 릴리즈일 경우 stable 브랜치로, 프리릴리즈일 경우 pre-release 브랜치로 병합하는 커밋에서 분기합니다.)에서 `"bugfix/(버그 이름)"`의 이름을 가진 브랜치를 master 브랜치에서 분기한 후, 버그를 수정한 후 master 브랜치 및 stable 또는 pre-release 브랜치(안정된 릴리즈일 경우 stable 브랜치로, 프리릴리즈일 경우 pre-release 브랜치로 병합합니다.)로 병합합니다. 이때, master 브랜치에 먼저 병합을 한 후에 stable 또는 pre-release 브랜치에 병합해야 합니다. 병합을 한 후에는 브랜치를 삭제합니다.
	- 단, 프리릴리즈를 포함하여 가장 최신 릴리즈에서 심각한 버그가 발견되었을 경우, master 브랜치에 부 버전을 올려야 하는 기능 구현을 하지 않았다면 브랜치를 분기하지 않고 master 브랜치에서 작업합니다(단, 이 경우에도 버그의 수정이 오래 걸릴 것으로 보이면 브랜치를 분기할 수 있습니다.).
	- 해당 브랜치에는 예외적으로 이슈와 PR을 넣을 수 있습니다.
## 릴리즈 절차
1. master 브랜치에서 특정 버전에 대한 개발을 완료합니다.
2. 만약 정식 출시가 가능한 안정화 된 버전일 경우 stable 브랜치로 커밋을 병합합니다. 만약 정식 출시 전 프리릴리즈일 경우 pre-release 브랜치로 커밋을 병합합니다.
3. 만약 stable 브랜치로 병합했다면, stable 브랜치를 pre-release 브랜치에 병합합니다. master에서 pre-release로 병합하면 안됩니다.
4. Release를 작성합니다. 반드시 기존에 작성된 Release 게시글의 형식을 따라 주십시오.