#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#	define AHEUIPLUSPLUS_MACRO_IS_WINDOWS true
#else
#	define AHEUIPLUSPLUS_MACRO_IS_WINDOWS false
#endif

static_assert((sizeof(wchar_t) != sizeof(char32_t) && AHEUIPLUSPLUS_MACRO_IS_WINDOWS) ||
	(sizeof(wchar_t) == sizeof(char32_t)),
	"The size of wchar_t and char32_t can be different from each other on Windows.");

#include <Aheuiplusplus/code.hpp>
#include <Aheuiplusplus/interpreter.hpp>

#include <cstdio>
#include <string>
#include <vector>

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#	include <fcntl.h>
#	include <io.h>
#endif

namespace
{
	app::raw_code input_string()
	{
		std::rewind(stdin);

		app::raw_code result;

		if constexpr (sizeof(wchar_t) == sizeof(char32_t))
		{
			while (!std::feof(stdin))
			{
				char32_t value = std::fgetwc(stdin);

				if (value == U'\n')
				{
					break;
				}

				result.push_back(value);
			}
		}
		else
		{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
			_setmode(_fileno(stdin), _O_U16TEXT);
#endif
			while (!std::feof(stdin))
			{
				wchar_t high_surrogate = std::fgetwc(stdin);

				if (high_surrogate >= 0xD800 && high_surrogate <= 0xDBFF)
				{
					wchar_t low_surrogate = std::fgetwc(stdin);

					result.push_back(app::wchar_to_char32(high_surrogate, low_surrogate));
				}
				else
				{
					if (high_surrogate == L'\n')
					{
						break;
					}

					result.push_back(high_surrogate);
				}
			}
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
			_setmode(_fileno(stdin), _O_TEXT);
#endif
		}

		return result;
	}
	app::raw_code input_line(std::FILE* file)
	{
		app::raw_code result;

		while (!std::feof(file))
		{
			unsigned char first_byte = static_cast<unsigned char>(std::fgetc(file));

			if (first_byte < 0x80)
			{
				if (first_byte == '\r')
				{
					break;
				}
				if (first_byte == '\n')
				{
					break;
				}

				result.push_back(first_byte);
			}
			else if ((first_byte & 0xF0) == 0xF0)
			{
				result.push_back(
					((first_byte & 0x07) << 18) + ((static_cast<unsigned char>(std::fgetc(file)) & 0x3F) << 12) + ((static_cast<unsigned char>(std::fgetc(file)) & 0x3F) << 6) + (static_cast<unsigned char>(std::fgetc(file)) & 0x3F));
			}
			else if ((first_byte & 0xE0) == 0xE0)
			{
				result.push_back(
					((first_byte & 0x0F) << 12) + ((static_cast<unsigned char>(std::fgetc(file)) & 0x3F) << 6) + (static_cast<unsigned char>(std::fgetc(file)) & 0x3F));
			}
			else if ((first_byte & 0xC0) == 0xC0)
			{
				result.push_back(
					((first_byte & 0x1F) << 6) + (static_cast<unsigned char>(std::fgetc(file)) & 0x3F));
			}
		}

		return result;
	}
}

int main(int argc, char** argv)
{
	if (argc == 1)
	{
		std::printf("사용법: %s [-i] [path]\n"
					"(실행할 파일은 BOM이 없는 UTF-8로 인코딩 되어 있어야 합니다.)\n", argv[0]);
		return 0;
	}

	bool interpreting_mode = false;
	std::vector<std::string> paths;

	for (int i = 1; i < argc; ++i)
	{
		std::string argument = argv[i];

		if (argument == "-i")
		{
			interpreting_mode = true;
		}
		else
		{
			paths.push_back(argument);
		}
	}
	
	if (interpreting_mode && paths.size() != 0)
	{
		std::printf("오류: 인터프리팅 모드일 경우에는 실행할 파일의 경로가 필요하지 않습니다.\n");
		return 0;
	}
	else if (!interpreting_mode && paths.size() == 0)
	{
		std::printf("오류: 실행할 파일의 경로가 필요합니다.\n");
		return 0;
	}
	else if (!interpreting_mode && paths.size() != 1)
	{
		std::printf("오류: 실행할 파일의 경로는 하나만 필요합니다.\n");
		return 0;
	}

	app::interpreter i(stdin, stdout);

	if (interpreting_mode)
	{
		std::printf("코드를 실행하려면 코드를 입력하고 엔터 키를 누르세요.\n"
					"코드를 줄바꿈 하려면 줄의 맨 끝에 \\를 입력하고 엔터 키를 누르세요.\n"
					"종료하려면 !q 또는 !quit를 입력하세요.\n\n");

		while (true)
		{
			std::printf(">>> ");
			
			app::raw_code code = input_string();

			if (code.empty())
			{
				continue;
			}

			if (code == U"!q" || code == U"!quit")
			{
				return 0;
			}

			if (*(code.end() - 1) == U'\\')
			{
				*(code.end() - 1) = U'\n';

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
					}
				}
			}

			i.run(code);
			std::printf("\n");
		}
	}
	else
	{
		std::FILE* file = std::fopen(paths[0].c_str(), "r");

		app::raw_code code;
		app::raw_code line;

		while (!std::feof(file))
		{
			line = input_line(file);

			code += line;
			code += U'\n';
		}

		code = code.substr(0, code.length() - 2);

		i.run(code);
	}

	return 0;
}