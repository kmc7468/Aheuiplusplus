#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#	define AHEUIPLUSPLUS_MACRO_IS_WINDOWS true
#else
#	define AHEUIPLUSPLUS_MACRO_IS_WINDOWS false
#endif

static_assert((sizeof(wchar_t) != sizeof(char32_t) && AHEUIPLUSPLUS_MACRO_IS_WINDOWS) ||
	(sizeof(wchar_t) == sizeof(char32_t)),
	"The size of wchar_t and char32_t can be different from each other on Windows.");

#if AHEUIPLUSPLUS_TARGET == 1
#include <Aheuiplusplus/Aheuiplusplus.hpp>

#include <algorithm>
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
	bool parse_command(const app::raw_code& code, app::debugger& d, app::command_line& command_line,
		bool& command_quit)
	{
		static const char* help_message =
			"!q 또는 !quit - 인터프리터를 종료합니다.\n"
			"!help - 명령어 목록을 봅니다.\n"
			"!clear - 화면을 비웁니다.\n"
			"\n"
			"!d 또는 !dump - 전체 저장공간 상태를 덤프합니다.\n"
			"!d 또는 !dump [storage] - 해당 저장공간의 상태를 덤프합니다. storage는 완성된 현대 한글이여야 하며, 종성만이 결과에 영향을 미칩니다.\n"
			"\n"
			"!enable <option> - 해당 명령줄 옵션을 활성화 합니다. option은 A, l 중 하나입니다.\n"
			"!disable <option> - 해당 명령줄 옵션을 비활성화 합니다. option은 A, l 중 하나입니다.";
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
			else if (code.substr(0, 7) == U"!enable")
			{
				if (code.length() < 9)
				{
					std::printf("%s\n\n", invalid_argument_message);
					return true;
				}
				else if (code[7] != U' ')
				{
					std::printf("%s\n\n", invalid_argument_message);
					return true;
				}

				app::raw_code argument = code.substr(8);

				if (argument == U"A")
				{
					command_line.option_aheui(true);
				}
				else if (argument == U"l")
				{
					command_line.option_loud_mode(true);
				}
				else
				{
					std::printf("%s\n\n", invalid_argument_message);
				}
				return true;
			}
			else if (code.substr(0, 8) == U"!disable")
			{
				if (code.length() < 10)
				{
					std::printf("%s\n\n", invalid_argument_message);
					return true;
				}
				else if (code[8] != U' ')
				{
					std::printf("%s\n\n", invalid_argument_message);
					return true;
				}

				app::raw_code argument = code.substr(9);

				if (argument == U"A")
				{
					command_line.option_aheui(true);
				}
				else if (argument == U"l")
				{
					command_line.option_loud_mode(true);
				}
				else
				{
					std::printf("%s\n\n", invalid_argument_message);
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
	app::raw_code input_line(std::FILE* file, const app::command_line& command_line)
	{
		app::raw_code result;
		
		if (command_line.option_utf8())
		{
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
					result += app::encoding::utf8::decode(c[0]);
				}
				else if (c.length() == 2)
				{
					result += app::encoding::utf8::decode(c[0], c[1]);
				}
				else if (c.length() == 3)
				{
					if (std::equal(reinterpret_cast<std::uint8_t*>(&c[0]), reinterpret_cast<std::uint8_t*>(&c[2]), app::encoding::utf8::byte_order_mark))
					{
						continue;
					}

					result += app::encoding::utf8::decode(c[0], c[1], c[2]);
				}
				else
				{
					result += app::encoding::utf8::decode(c[0], c[1], c[2], c[3]);
				}
			}
		}
		else if (command_line.option_utf16())
		{
			while (!std::feof(file))
			{
				unsigned char first_byte = std::fgetc(file);
				unsigned char second_byte = std::fgetc(file);

				char16_t first = (first_byte << 8) + second_byte;
				if (app::is_little_endian())
				{
					std::reverse(reinterpret_cast<std::uint8_t*>(&first), reinterpret_cast<std::uint8_t*>(&first) + 2);
				}
				int length = app::encoding::utf16::encoded_length(first);
				
				if (length == 2 && (first == u'\n' || first == u'\r'))
				{
					if (first == '\r')
					{
						std::fgetc(file);
						std::fgetc(file);
					}

					break;
				}

				if (length == 2)
				{
					if ((first == 0xFEFF && app::is_little_endian()) ||
						(first == 0xFFEE && !app::is_little_endian()))
					{
						continue;
					}

					result += first;
				}
				else
				{
					first_byte = std::fgetc(file);
					second_byte = std::fgetc(file);

					char16_t second = (first_byte << 8) + second_byte;
					
					result += app::encoding::utf16::decode(first, second);
				}
			}
		}
		else
			throw std::invalid_argument("인수 command_line에 명시된 인코딩이 없습니다.");
		
		return result;
	}
}

int main(int argc, char** argv)
{
	app::command_line command_line;

	if (!command_line.parse(argc, argv))
	{
		return 0;
	}
	
	app::interpreter i(command_line.option_version());
	app::debugger d(stdout, i);

	d.connect_debugger();

	std::setlocale(LC_ALL, "");

	if (command_line.option_interpreting_mode())
	{
		static const char* title_message =
			"- 코드를 실행하려면 코드를 입력하고 엔터 키를 누르세요.\n"
			"- 코드를 줄바꿈 하려면 줄의 맨 끝에 \\를 입력하고 엔터 키를 누르세요.\n"
			"- 인터프리터를 종료하려면 !q 또는 !quit 명령어를 입력하세요.\n"
			"- 더 많은 명령어 목록을 보려면 !help 명령어를 입력하세요.";

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
		_setmode(_fileno(stdout), _O_TEXT);
#endif
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

			if (parse_command(code, d, command_line, command_quit))
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

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
			_setmode(_fileno(stdin), _O_U16TEXT);
			_setmode(_fileno(stdout), _O_U16TEXT);
#endif
			d.run_with_debugging(code, command_line);
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
			_setmode(_fileno(stdout), _O_TEXT);
#endif
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
		std::FILE* file = std::fopen(command_line.option_code_path().c_str(), "r");

		if (file == nullptr)
		{
			std::printf("오류: '%s' 파일을 여는데에 실패했습니다. 경로 또는 권한을 확인해 보십시오.\n", command_line.option_code_path().c_str());
			return 0;
		}

		app::raw_code code;
		app::raw_code line;

		while (!std::feof(file))
		{
			line = input_line(file, command_line);

			code += line;
			code += U'\n';
		}

		code = code.substr(0, code.length() - 2);

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
		_setmode(_fileno(stdout), _O_U16TEXT);
#endif
		long long result = d.run_with_debugging(code, command_line);
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
		_setmode(_fileno(stdout), _O_TEXT);
#endif
		std::printf("\n");

		std::fclose(file);

		return static_cast<int>(result);
	}

	return 0;
}
#endif