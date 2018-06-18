#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#	define AHEUIPLUSPLUS_MACRO_IS_WINDOWS true
#else
#	define AHEUIPLUSPLUS_MACRO_IS_WINDOWS false
#endif

static_assert((sizeof(wchar_t) != sizeof(char32_t) && AHEUIPLUSPLUS_MACRO_IS_WINDOWS) ||
	(sizeof(wchar_t) == sizeof(char32_t)),
	"The size of wchar_t and char32_t can be different from each other on Windows.");

#if AHEUIPLUSPLUS_TARGET == 1
#include <Aheuiplusplus/code.hpp>
#include <Aheuiplusplus/debugger.hpp>
#include <Aheuiplusplus/interpreter.hpp>
#include <Aheuiplusplus/version.hpp>

#include <clocale>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#	include <fcntl.h>
#	include <io.h>
#endif

namespace
{
	bool parse_command_line(int argc, char** argv,
		bool& command_aheui, bool& command_interpreting, app::version& version, std::string& path,
		bool& command_loud_mode)
	{
		if (argc == 1)
		{
			std::printf("오류: 입력이 없습니다. --help 옵션을 이용해 사용법을 확인하실 수 있습니다.\n");
			return false;
		}

		static const char* duplicate_message = " 옵션이 두번 이상 사용되었습니다.";
		static const char* invalid_argument_message = " 옵션의 인수가 올바르지 않습니다. --help 옵션을 이용해 올바른 인수 형태를 확인하실 수 있습니다.";

		bool command_help = false;
		bool command_version = false;
		command_aheui = false;
		command_interpreting = false;
		path.clear();
		version = app::version::none;
		command_loud_mode = false;

		for (int i = 1; i < argc; ++i)
		{
			std::string argument = argv[i];

			if (argument == "--help")
			{
				if (command_help)
				{
					std::printf("오류: %s%s\n", "--help", duplicate_message);
					return false;
				}

				command_help = true;
			}
			else if (argument == "--version")
			{
				if (command_version)
				{
					std::printf("오류: %s%s\n", "--version", duplicate_message);
					return false;
				}

				command_version = true;
			}
			else if (argument == "-A")
			{
				if (command_aheui)
				{
					std::printf("오류: %s%s\n", "-A", duplicate_message);
					return false;
				}

				command_aheui = true;
			}
			else if (argument == "-i")
			{
				if (command_interpreting)
				{
					std::printf("오류: %s%s\n", "-i", duplicate_message);
					return false;
				}

				command_interpreting = true;
			}
			else if (argument.substr(0, 4) == "-std")
			{
				if (argument.length() <= 5)
				{
					std::printf("오류: %s%s\n", "-std", invalid_argument_message);
					return false;
				}
				else if (argument[4] != '=')
				{
					std::printf("오류: %s%s\n", "-std", invalid_argument_message);
					return false;
				}
				
				std::string argument_of_argument = argument.substr(5);
				
				if (argument_of_argument.find('.') == std::string::npos)
				{
					try
					{
						int major = std::stoi(argument_of_argument);

						version = app::get_version(major);

						if (version == app::version::none)
						{
							std::printf("오류: %s%s\n", "-std", invalid_argument_message);
							return false;
						}
					}
					catch (...)
					{
						std::printf("오류: %s%s\n", "-std", invalid_argument_message);
						return false;
					}
				}
				else
				{
					std::string major_raw = argument_of_argument.substr(0, argument_of_argument.find('.'));
					std::string minor_raw = argument_of_argument.substr(argument_of_argument.find('.') + 1);

					if (minor_raw.find('.') != std::string::npos)
					{
						std::printf("오류: %s%s\n", "-std", invalid_argument_message);
						return false;
					}

					int major = std::stoi(major_raw);
					int minor = std::stoi(minor_raw);

					version = app::get_version(major, minor);

					if (version == app::version::none)
					{
						std::printf("오류: %s%s\n", "-std", invalid_argument_message);
						return false;
					}
				}
			}
			else if (argument == "-l")
			{
				if (command_loud_mode)
				{
					std::printf("오류: %s%s\n", "-l", duplicate_message);
					return false;
				}

				command_loud_mode = true;
			}
			else
			{
				if (argument.front() == '-')
				{
					std::printf("오류: %s는 알 수 없는 옵션입니다. --help 옵션을 이용해 올바른 옵션을 확인하실 수 있습니다.\n", argument.c_str());
					return false;
				}
				else if (!path.empty())
				{
					std::printf("오류: 경로가 두개 이상 입력되었습니다.\n");
					return false;
				}

				path = argument;
			}
		}

		if (command_help)
		{
			std::printf("사용법: %s [option(s)...] [path]\n"
						"path는 아희++(또는 아희) 코드가 기록된 BOM이 없는 UTF-8로 인코딩 된 텍스트 파일이여야 합니다(인터프리팅 모드일 경우 필요하지 않습니다.).\n\n"
						"--help - 사용법 및 옵션 목록을 봅니다.\n"
						"--version - 프로그램의 버전을 봅니다.\n\n"
						"-A - 아희 표준대로만 작동합니다(아희++의 기능을 이용할 수 없습니다.). -std 옵션과 함께 쓰일 수 없습니다.\n"
						"-std=<version> - 어떤 버전의 아희++ 표준을 따를지 설정합니다. version은 m 또는 m.n 형태로 구성됩니다(이때 m은 주 버전, n은 부 버전입니다.). -A 옵션과 함께 쓰일 수 없습니다.\n"
						"-i - 인터프리팅 모드로 전환합니다.\n\n"
						"-l - 입력을 받아야 할 때 입력을 요청하는 메세지를 출력합니다.\n", argv[0]);
			return false;
		}
		if (command_version)
		{
			std::printf("아희++ 표준 인터프리터 %s\n\n%s\n이 프로그램은 공개 소프트웨어로, 소스 코드는 위 웹사이트에서 MIT 라이선스에 의해 배포되고 있습니다.\n",
				app::interpreter::version_string, "https://github.com/kmc7468/Aheuiplusplus");
			return false;
		}

		if (command_aheui && version != app::version::none)
		{
			std::printf("오류: -A 옵션은 -std 옵션과 함께 쓰일 수 없습니다.\n");
			return false;
		}
		if (command_interpreting && !path.empty())
		{
			std::printf("오류: 인터프리팅 모드일 경우 path는 필요하지 않습니다.\n");
			return false;
		}
		if (!command_interpreting && path.empty())
		{
			std::printf("오류: 일반 모드일 경우 path가 필요합니다.\n");
			return false;
		}

		if (version == app::version::none)
		{
			version = app::version::latest;
		}

		return true;
	}
	bool parse_command(const app::raw_code& code, app::debugger& d, bool& command_quit)
	{
		static const char* help_message =
			"!q 또는 !quit - 인터프리터를 종료합니다.\n"
			"!help - 명령어 목록을 봅니다.\n"
			"!clear - 화면을 비웁니다.\n"
			"\n"
			"!d 또는 !dump - 전체 저장공간 상태를 덤프합니다.\n"
			"!d 또는 !dump [storage] - 해당 저장공간의 상태를 덤프합니다. storage는 완성된 현대 한글이여야 하며, 종성만이 결과에 영향을 미칩니다.";
		static const char* invalid_argument_message = "오류: 올바르지 않은 인수입니다. !help 명령어를 입력해 올바른 인수 형태를 확인하실 수 있습니다.";

		command_quit = false;

		if (code == U"!q" || code == U"!quit")
		{
			command_quit = true;
			return true;
		}
		else if (code == U"!clear")
		{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
			std::system("cls");
#else
			std::system("clear");
#endif
			return true;
		}
		else if (code == U"!help")
		{
			std::printf("%s\n\n", help_message);
			return true;
		}

		else if (code == U"!d" || code == U"!dump")
		{
			d.dump_storages(1);
			return true;
		}
		else if (code.front() == U'!')
		{
			if (code.substr(0, 3) == U"!d " || code.substr(0, 6) == U"!dump ")
			{
				if ((code.length() != 4 && code.length() != 7) ||
					!app::is_complete_hangul(code.back()))
				{
					std::printf("%s\n\n", invalid_argument_message);
				}
				else
				{
					d.dump_storage(code.back());
				}
				return true;
			}
			else
			{
				std::printf("오류: 알 수 없는 명령어입니다. !help 명령어를 입력해 올바른 명령어 목록을 확인하실 수 있습니다.\n\n");
				return true;
			}
		}

		return false;
	}

	app::raw_code input_string()
	{
		app::raw_code result;

		while (true)
		{
			char32_t c = app::read_char(stdin);

			if (c == U'\n')
			{
				break;
			}

			result += c;
		}

		return result;
	}
	app::raw_code input_line(std::FILE* file)
	{
		app::raw_code result;

		while (!std::feof(file))
		{
			std::string c = app::read_u8char(file);

			if (c.length() == 1 && (c.front() == '\n' || c.front() == '\r'))
			{
				if (c.front() == '\r')
				{
					app::read_u8char(file);
				}

				break;
			}

			if (c.length() == 1)
			{
				result += app::u8char_to_char32(c[0]);
			}
			else if (c.length() == 2)
			{
				result += app::u8char_to_char32(c[0], c[1]);
			}
			else if (c.length() == 3)
			{
				result += app::u8char_to_char32(c[0], c[1], c[2]);
			}
			else
			{
				result += app::u8char_to_char32(c[0], c[1], c[2], c[3]);
			}
		}

		return result;
	}
}

int main(int argc, char** argv)
{
	bool command_aheui;
	bool command_interpreting;
	std::string path;
	app::version version;
	bool command_loud_mode;

	if (!parse_command_line(argc, argv,
		command_aheui, command_interpreting, version, path,
		command_loud_mode))
	{
		return 0;
	}
	
	app::interpreter i(version, stdin, stdout);
	app::debugger d(stdout, i);

	d.connect_debugger();

	std::setlocale(LC_ALL, "");

	if (command_interpreting)
	{
		static const char* title_message =
			"- 코드를 실행하려면 코드를 입력하고 엔터 키를 누르세요.\n"
			"- 코드를 줄바꿈 하려면 줄의 맨 끝에 \\를 입력하고 엔터 키를 누르세요.\n"
			"- 인터프리터를 종료하려면 !q 또는 !quit 명령어를 입력하세요.\n"
			"- 더 많은 명령어 목록을 보려면 !help 명령어를 입력하세요.";

		std::printf("아희++ 표준 인터프리터 %s (%s)\n%s\n\n",
			app::interpreter::version_string, "https://github.com/kmc7468/Aheuiplusplus", title_message);

		while (true)
		{
			std::printf(">>> ");
			
			app::raw_code code = input_string();

			if (code.empty())
			{
				continue;
			}

			bool command_quit;

			if (parse_command(code, d, command_quit))
			{
				if (command_quit)
				{
					return 0;
				}
				else
				{
					continue;
				}
			}
			
			if (*(code.end() - 1) == U'\\')
			{
				code.erase(code.end() - 1);

				while (true)
				{
					std::printf("    ");
					app::raw_code new_line = input_string();

					if (new_line.empty())
					{
						code += U'\n';
						break;
					}
					else
					{
						code += U'\n';
						code += new_line;

						if (*(new_line.end() - 1) != U'\\')
						{
							break;
						}
						else
						{
							code.erase(code.end() - 1);
						}
					}
				}
			}

			d.run_with_debugging(code,
				command_aheui, command_loud_mode);
			std::printf("\n");

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
			std::rewind(stdin);
#else
			while (!std::feof(stdin))
			{
				app::read_char(stdin);
			}
#endif
		}
	}
	else
	{
		std::FILE* file = std::fopen(path.c_str(), "r");

		if (file == nullptr)
		{
			std::printf("오류: '%s' 파일을 여는데에 실패했습니다. 경로 또는 권한을 확인해 보십시오.\n", path.c_str());
			return 0;
		}

		app::raw_code code;
		app::raw_code line;

		while (!std::feof(file))
		{
			line = input_line(file);

			code += line;
			code += U'\n';
		}

		code = code.substr(0, code.length() - 2);

		long long result = d.run_with_debugging(code,
			command_aheui, command_loud_mode);
		std::printf("\n");

		std::fclose(file);

		return static_cast<int>(result);
	}

	return 0;
}
#endif