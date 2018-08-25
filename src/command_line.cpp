#include <Aheuiplusplus/command_line.hpp>

#include <Aheuiplusplus/interpreter.hpp>

#include <cstdlib>
#include <utility>

namespace app
{
	command_line::command_line(const command_line& data)
		: option_aheui_(data.option_aheui_),
		  option_interpreting_mode_(data.option_interpreting_mode_),
		  option_version_(data.option_version_),
		  option_utf8_(data.option_utf8_),
		  option_utf16_(data.option_utf16_),
		  option_utf16be_(data.option_utf16be_),
		
		  option_loud_mode_(data.option_loud_mode_),

		  option_code_path_(data.option_code_path_)
	{}
	command_line::command_line(command_line&& data) noexcept
		: option_aheui_(data.option_aheui_),
		  option_interpreting_mode_(data.option_interpreting_mode_),
		  option_version_(data.option_version_),
		  option_utf8_(data.option_utf8_),
		  option_utf16_(data.option_utf16_),
		  option_utf16be_(data.option_utf16be_),

		  option_loud_mode_(data.option_loud_mode_),

		  option_code_path_(std::move(data.option_code_path_))
	{}

	command_line& command_line::operator=(const command_line& data)
	{
		option_aheui_ = data.option_aheui_;
		option_interpreting_mode_ = data.option_interpreting_mode_;
		option_version_ = data.option_version_;
		option_utf8_ = data.option_utf8_;
		option_utf16_ = data.option_utf16_;
		option_utf16be_ = data.option_utf16be_;

		option_loud_mode_ = data.option_loud_mode_;

		option_code_path_ = data.option_code_path_;

		return *this;
	}
	command_line& command_line::operator=(command_line&& data) noexcept
	{
		option_aheui_ = data.option_aheui_;
		option_interpreting_mode_ = data.option_interpreting_mode_;
		option_version_ = data.option_version_;
		option_utf8_ = data.option_utf8_;
		option_utf16_ = data.option_utf16_;
		option_utf16be_ = data.option_utf16be_;

		option_loud_mode_ = data.option_loud_mode_;

		option_code_path_ = std::move(data.option_code_path_);

		return *this;
	}

	bool command_line::parse(int argc, char** argv)
	{
		return parse(stderr, argc, argv);
	}
	bool command_line::parse(std::FILE* error_stream, int argc, char** argv)
	{
		if (argc <= 1)
		{
			std::fprintf(error_stream, "오류: 입력이 없습니다. --help 옵션을 이용해 사용법을 확인하실 수 있습니다.\n");
			return false;
		}

		bool option_help = false;
		bool option_version = false;

		option_aheui_ = false;
		option_interpreting_mode_ = false;
		option_version_ = version::none;
		option_utf8_ = false;
		option_utf16_ = false;
		option_utf16be_ = false;

		option_loud_mode_ = false;

		option_code_path_.clear();

		static const char* duplicate_message = " 옵션이 두번 이상 사용되었습니다.";
		static const char* invalid_argument_message = " 옵션의 인수가 올바르지 않습니다.";

		for (int i = 1; i < argc; ++i)
		{
			const std::string_view argument = argv[i];

			if (argument == "--help")
			{
				if (option_help)
				{
					std::fprintf(error_stream, "오류: %s%s\n", "--help", duplicate_message);
					return false;
				}

				option_help = true;
			}
			else if (argument == "--version")
			{
				if (option_version)
				{
					std::fprintf(error_stream, "오류: %s%s\n", "--version", duplicate_message);
					return false;
				}

				option_version = true;
			}

			else if (argument == "-A")
			{
				if (option_aheui_)
				{
					std::fprintf(error_stream, "오류: %s%s\n", "-A", duplicate_message);
					return false;
				}

				option_aheui_ = true;
			}
			else if (argument == "-i")
			{
				if (option_interpreting_mode_)
				{
					std::fprintf(error_stream, "오류: %s%s\n", "-i", duplicate_message);
					return false;
				}

				option_interpreting_mode_ = true;
			}
			else if (argument.substr(0, 4) == "-std")
			{
				if (argument.length() <= 5)
				{
					std::fprintf(error_stream, "오류: %s%s\n", "-std", invalid_argument_message);
					return false;
				}
				else if (argument[4] != '=')
				{
					std::fprintf(error_stream, "오류: %s%s\n", "-std", invalid_argument_message);
					return false;
				}

				const std::string_view argument_of_argument = argument.substr(5);
				const std::size_t dot_pos = argument_of_argument.find('.');

				if (dot_pos == std::string_view::npos)
				{
					char* dummy;
					const int major = static_cast<int>(strtol(argument_of_argument.data(), &dummy, 10));

					option_version_ = get_version(major);

					if (option_version_ == version::none)
					{
						std::fprintf(error_stream, "오류: %s%s\n", "-std", invalid_argument_message);
						return false;
					}
				}
				else
				{
					const std::string_view major_string = argument_of_argument.substr(0, dot_pos);
					const std::string_view minor_string = argument_of_argument.substr(dot_pos + 1);

					char* dummy;
					const int major = static_cast<int>(strtol(major_string.data(), &dummy, 10));
					const int minor = static_cast<int>(strtol(minor_string.data(), &dummy, 10));

					option_version_ = get_version(major, minor);

					if (option_version_ == version::none)
					{
						std::fprintf(error_stream, "오류: %s%s\n", "-std", invalid_argument_message);
						return false;
					}
				}
			}
			else if (argument == "-utf8")
			{
				if (option_utf8_)
				{
					std::fprintf(error_stream, "오류: %s%s\n", "-utf8", duplicate_message);
					return false;
				}

				option_utf8_ = true;
			}
			else if (argument == "-utf16")
			{
				if (option_utf16_)
				{
					std::fprintf(error_stream, "오류: %s%s\n", "-utf16", duplicate_message);
					return false;
				}

				option_utf16_ = true;
			}
			else if (argument == "-utf16be")
			{
				if (option_utf16be_)
				{
					std::fprintf(error_stream, "오류: %s%s\n", "-utf16be", duplicate_message);
					return false;
				}

				option_utf16be_ = true;
			}

			else if (argument == "-l")
			{
				if (option_loud_mode_)
				{
					std::fprintf(error_stream, "오류: %s%s\n", "-l", duplicate_message);
					return false;
				}

				option_loud_mode_ = true;
			}

			else
			{
				if (argument.front() == '-')
				{
					std::fprintf(error_stream, "오류: %s는 알 수 없는 옵션입니다.\n", argument.data());
					return false;
				}
				else if (!option_code_path_.empty())
				{
					std::fprintf(error_stream, "오류: 경로가 두개 이상 입력되었습니다.\n");
					return false;
				}

				option_code_path_ = argument;
			}
		}

		if (option_help)
		{

			std::fprintf(error_stream,
				"사용법: %s [option(s)...] [path]\n"
				"path는 아희++(또는 아희) 코드가 기록된 텍스트 파일에 대한 올바른 경로여야 합니다(인터프리팅 모드일 경우 필요하지 않습니다.).\n"
				"\n"
				"--help - 사용법 및 명령줄 옵션 목록을 봅니다.\n"
				"--version - 이 소프트웨어의 버전을 봅니다.\n"
				"\n"
				"-A - 아희 전용 모드로 실행합니다. -std 옵션과 함께 쓰일 수 없습니다.\n"
				"-i - 인터프리팅 모드로 실행합니다. -utf8, -utf16, -utf16be 옵션과 함께 쓰일 수 없습니다.\n"
				"-std=<version> - 이 소프트웨어가 따를 아희++ 표준의 버전을 설정합니다. version은 m 또는 m.n의 형태로 구성됩니다(이때 m은 주버전, n은 부버전입니다.). -A 옵션과 함께 쓰일 수 없습니다.\n"
				"-utf8 - path가 나타내는 텍스트 파일의 인코딩이 UTF-8임을 명시합니다. 기본적으로 이 옵션이 적용됩니다. -i, -utf16, -utf16be 옵션과 함께 쓰일 수 없습니다.\n"
				"-utf16 - path가 나타내는 텍스트 파일의 인코딩이 UTF-16LE임을 명시합니다. -i, -utf8, -utf16be 옵션과 함께 쓰일 수 없습니다.\n"
				"-utf16be - path가 나타내는 텍스트 파일의 인코딩이 UTF-16BE임을 명시합니다. -i, -utf8, -utf16 옵션과 함께 쓰일 수 없습니다.\n"
				"\n"
				"-l - 아희++(또는 아희) 코드가 입력을 요청했을 때 요청됐음을 나타내는 메세지를 출력합니다.\n",
				argv[0]);

			return false;
		}
		else if (option_version)
		{
			std::fprintf(error_stream,
				"아희++ 표준 인터프리터 %s\n%s\n\n이 소프트웨어는 공개 소프트웨어로, 소스 코드는 아래 웹사이트에서 MIT 라이선스로 배포되고 있습니다.\n\n%s\n",
				version_string, "(C) 2018. kmc7468 All rights reserved.", "https://github.com/kmc7468/Aheuiplusplus");

			return false;
		}

		if (option_aheui_ && option_version_ != version::none)
		{
			std::fprintf(error_stream, "오류: -A 옵션과 -std 옵션은 함께 쓰일 수 없습니다.\n");
			return false;
		}
		else if (option_interpreting_mode_ && !option_code_path_.empty())
		{
			std::fprintf(error_stream, "오류: 인터프리팅 모드일 경우 path가 필요하지 않습니다.\n");
			return false;
		}
		else if (!option_interpreting_mode_ && option_code_path_.empty())
		{
			std::fprintf(error_stream, "오류: 일반 모드일 경우 path가 필요합니다.\n");
			return false;
		}
		else if (option_interpreting_mode_ && option_utf8_)
		{
			std::fprintf(error_stream, "오류: -i 옵션과 -utf8 옵션은 함께 쓰일 수 없습니다.\n");
			return false;
		}
		else if (option_interpreting_mode_ && option_utf16_)
		{
			std::fprintf(error_stream, "오류: -i 옵션과 -utf16 옵션은 함께 쓰일 수 없습니다.\n");
			return false;
		}
		else if (option_interpreting_mode_ && option_utf16be_)
		{
			std::fprintf(error_stream, "오류: -i 옵션과 -utf16be 옵션은 함께 쓰일 수 없습니다.\n");
			return false;
		}
		else if (option_utf8_ && option_utf16_)
		{
			std::fprintf(error_stream, "오류: -utf8 옵션과 -utf16 옵션은 함께 쓰일 수 없습니다.\n");
			return false;
		}
		else if (option_utf8_ && option_utf16be_)
		{
			std::fprintf(error_stream, "오류: -utf8 옵션과 -utf16be 옵션은 함께 쓰일 수 없습니다.\n");
			return false;
		}
		else if (option_utf16_ && option_utf16be_)
		{
			std::fprintf(error_stream, "오류: -utf16 옵션과 -utf16be 옵션은 함께 쓰일 수 없습니다.\n");
			return false;
		}

		if (!option_utf8_ && !option_utf16_ && !option_utf16be_)
		{
			option_utf8_ = true;
		}
		if (option_version_ == version::none)
		{
			option_version_ = version::latest;
		}

		return true;
	}

	bool command_line::option_aheui() const noexcept
	{
		return option_aheui_;
	}
	void command_line::option_aheui(bool new_option_aheui) noexcept
	{
		option_aheui_ = new_option_aheui;
	}
	bool command_line::option_interpreting_mode() const noexcept
	{
		return option_interpreting_mode_;
	}
	void command_line::option_interpreting_mode(bool new_option_interpreting_mode) noexcept
	{
		option_interpreting_mode_ = new_option_interpreting_mode;
	}
	bool command_line::option_utf8() const noexcept
	{
		return option_utf8_;
	}
	void command_line::option_utf8(bool new_option_utf8) noexcept
	{
		option_utf8_ = new_option_utf8;
	}
	bool command_line::option_utf16() const noexcept
	{
		return option_utf16_;
	}
	void command_line::option_utf16(bool new_option_utf16) noexcept
	{
		option_utf16_ = new_option_utf16;
	}
	bool command_line::option_utf16be() const noexcept
	{
		return option_utf16be_;
	}
	void command_line::option_utf16be(bool new_option_utf16be) noexcept
	{
		option_utf16be_ = new_option_utf16be;
	}

	bool command_line::option_loud_mode() const noexcept
	{
		return option_loud_mode_;
	}
	void command_line::option_loud_mode(bool new_option_loud_mode) noexcept
	{
		option_loud_mode_ = new_option_loud_mode;
	}

	std::string command_line::option_code_path() const
	{
		return option_code_path_;
	}
	void command_line::option_code_path(const std::string_view& new_option_code_path)
	{
		option_code_path_ = new_option_code_path;
	}
	void command_line::option_code_path(std::string&& new_option_code_path)
	{
		option_code_path_ = std::move(new_option_code_path);
	}
}