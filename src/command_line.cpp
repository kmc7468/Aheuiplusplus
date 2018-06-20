#include <Aheuiplusplus/command_line.hpp>

#include <Aheuiplusplus/interpreter.hpp>

#include <functional>

namespace app
{
	command_line::command_line(const command_line& data)
		: option_aheui_(data.option_aheui_), option_interpreting_mode_(data.option_interpreting_mode_),
		option_version_(data.option_version_), option_loud_mode_(data.option_loud_mode_), option_input_end_mode_(data.option_input_end_mode_),
		option_code_path_(data.option_code_path_)
	{}
	
	command_line& command_line::operator=(const command_line& data)
	{
		option_aheui_ = data.option_aheui_;
		option_interpreting_mode_ = data.option_interpreting_mode_;
		option_version_ = data.option_version_;

		option_loud_mode_ = data.option_loud_mode_;
		option_input_end_mode_ = data.option_input_end_mode_;

		option_code_path_ = data.option_code_path_;

		return *this;
	}

	bool command_line::parse(int argc, char** argv)
	{
		return parse(stdout, argc, argv);
	}
	bool command_line::parse(std::FILE* output_stream, int argc, char** argv)
	{
		if (argc == 1)
		{
			std::fprintf(output_stream, "오류: 입력이 없습니다. --help 옵션을 이용해 사용법을 확인하실 수 있습니다.\n");
			return false;
		}

		static const char* duplicate_message = " 옵션이 두번 이상 사용되었습니다.";
		static const char* invalid_argument_message = " 옵션의 인수가 올바르지 않습니다. --help 옵션을 이용해 올바른 인수 형태를 확인하실 수 있습니다.";

		bool option_help = false;
		bool option_version = false;

		option_aheui_ = false;
		option_interpreting_mode_ = false;
		option_version_ = version::none;
		option_utf8_ = false;
		option_utf16_ = false;

		option_loud_mode_ = false;
		option_input_end_mode_ = false;

		option_code_path_.clear();

		for (int i = 1; i < argc; ++i)
		{
			std::string argument = argv[i];

			if (argument == "--help")
			{
				if (option_help)
				{
					std::fprintf(output_stream, "오류: %s%s\n", "--help", duplicate_message);
					return false;
				}

				option_help = true;
			}
			else if (argument == "--version")
			{
				if (option_version)
				{
					std::fprintf(output_stream, "오류: %s%s\n", "--version", duplicate_message);
					return false;
				}

				option_version = true;
			}
			else if (argument == "-A")
			{
				if (option_aheui_)
				{
					std::fprintf(output_stream, "오류: %s%s\n", "-A", duplicate_message);
					return false;
				}

				option_aheui_ = true;
			}
			else if (argument == "-i")
			{
				if (option_interpreting_mode_)
				{
					std::fprintf(output_stream, "오류: %s%s\n", "-i", duplicate_message);
					return false;
				}

				option_interpreting_mode_ = true;
			}
			else if (argument == "-utf8")
			{
				if (option_utf8_)
				{
					std::fprintf(output_stream, "오류: %s%s\n", "-utf8", duplicate_message);
					return false;
				}

				option_utf8_ = true;
			}
			else if (argument == "-utf16")
			{
				if (option_utf16_)
				{
					std::fprintf(output_stream, "오류: %s%s\n", "-utf16", duplicate_message);
					return false;
				}

				option_utf16_ = true;
			}
			else if (argument.substr(0, 4) == "-std")
			{
				if (argument.length() <= 5)
				{
					std::fprintf(output_stream, "오류: %s%s\n", "-std", invalid_argument_message);
					return false;
				}
				else if (argument[4] != '=')
				{
					std::fprintf(output_stream, "오류: %s%s\n", "-std", invalid_argument_message);
					return false;
				}

				std::string argument_of_argument = argument.substr(5);

				if (argument_of_argument.find('.') == std::string::npos)
				{
					try
					{
						int major = std::stoi(argument_of_argument);

						option_version_ = get_version(major);

						if (option_version_ == version::none)
						{
							std::fprintf(output_stream, "오류: %s%s\n", "-std", invalid_argument_message);
							return false;
						}
					}
					catch (...)
					{
						std::fprintf(output_stream, "오류: %s%s\n", "-std", invalid_argument_message);
						return false;
					}
				}
				else
				{
					std::string major_raw = argument_of_argument.substr(0, argument_of_argument.find('.'));
					std::string minor_raw = argument_of_argument.substr(argument_of_argument.find('.') + 1);

					if (minor_raw.find('.') != std::string::npos)
					{
						std::fprintf(output_stream, "오류: %s%s\n", "-std", invalid_argument_message);
						return false;
					}

					int major = std::stoi(major_raw);
					int minor = std::stoi(minor_raw);

					option_version_ = get_version(major, minor);

					if (option_version_ == version::none)
					{
						std::fprintf(output_stream, "오류: %s%s\n", "-std", invalid_argument_message);
						return false;
					}
				}
			}
			else if (argument == "-l")
			{
				if (option_loud_mode_)
				{
					std::fprintf(output_stream, "오류: %s%s\n", "-l", duplicate_message);
					return false;
				}

				option_loud_mode_ = true;
			}
			else
			{
				if (argument.front() == '-')
				{
					std::fprintf(output_stream, "오류: %s는 알 수 없는 옵션입니다. --help 옵션을 이용해 올바른 옵션을 확인하실 수 있습니다.\n", argument.c_str());
					return false;
				}
				else if (!option_code_path_.empty())
				{
					std::fprintf(output_stream, "오류: 경로가 두개 이상 입력되었습니다.\n");
					return false;
				}

				option_code_path_ = argument;
			}
		}

		if (option_help)
		{
			std::fprintf(output_stream,
				"사용법: %s [option(s)...] [path]\n"
				"path는 아희++(또는 아희) 코드가 기록된 BOM이 없는 UTF-8로 인코딩 된 텍스트 파일이여야 합니다(인터프리팅 모드일 경우 필요하지 않습니다.).\n"
				"\n"
				"--help - 사용법 및 옵션 목록을 봅니다.\n"
				"--version - 프로그램의 버전을 봅니다.\n"
				"\n"
				"-A - 아희 전용 모드로 전환합니다(아희++의 기능을 이용할 수 없습니다.). -std 옵션과 함께 쓰일 수 없습니다.\n"
				"-std=<version> - 어떤 버전의 아희++ 표준을 따를지 설정합니다. version은 m 또는 m.n 형태로 구성됩니다(이때 m은 주 버전, n은 부 버전입니다.). -A 옵션과 함께 쓰일 수 없습니다.\n"
				"-i - 인터프리팅 모드로 전환합니다.\n"
				"-utf8 - path의 인코딩이 UTF-8임을 명시합니다. 기본적으로 이 옵션이 적용됩니다. -i, -utf16 옵션과 함께 쓰일 수 없습니다.\n"
				"-utf16 - path의 인코딩이 UTF-16임을 명시합니다. -i, -utf8 옵션과 함께 쓰일 수 없습니다.\n"
				"\n"
				"-l - 입력을 받아야 할 때 입력을 요청하는 메세지를 출력합니다.\n",
				argv[0]);

			return false;
		}
		else if (option_version)
		{
			std::fprintf(output_stream,
				"아희++ 표준 인터프리터 %s\n\n%s\n이 프로그램은 공개 소프트웨어로, 소스 코드는 위 웹사이트에서 MIT 라이선스에 의해 배포되고 있습니다.\n",
				interpreter::version_string, "https://github.com/kmc7468/Aheuiplusplus");

			return false;
		}

		if (option_aheui_ && option_version_ != version::none)
		{
			std::fprintf(output_stream, "오류: -A 옵션과 -std 옵션은 함께 쓰일 수 없습니다.\n");

			return false;
		}
		else if (option_interpreting_mode_ && !option_code_path_.empty())
		{
			std::fprintf(output_stream, "오류: 인터프리팅 모드일 경우 path는 필요하지 않습니다.\n");

			return false;
		}
		else if (!option_interpreting_mode_ && option_code_path_.empty())
		{
			std::fprintf(output_stream, "오류: 일반 모드일 경우 path가 필요합니다.n");

			return false;
		}
		else if (option_interpreting_mode_ && option_utf8_)
		{
			std::fprintf(output_stream, "오류: -i 옵션과 -utf8 옵션은 함께 쓰일 수 없습니다.\n");

			return false;
		}
		else if (option_interpreting_mode_ && option_utf16_)
		{
			std::fprintf(output_stream, "오류: -i 옵션과 -utf16 옵션은 함께 쓰일 수 없습니다.\n");

			return false;
		}
		else if (option_utf8_ && option_utf16_)
		{
			std::fprintf(output_stream, "오류: -utf8 옵션과 -utf16 옵션은 함께 쓰일 수 없습니다.\n");

			return false;
		}

		if (!option_utf8_ && !option_utf16_)
		{
			option_utf8_ = true;
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
	version command_line::option_version() const noexcept
	{
		return option_version_;
	}
	void command_line::option_version(version new_option_version) noexcept
	{
		option_version_ = new_option_version;
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

	bool command_line::option_loud_mode() const noexcept
	{
		return option_loud_mode_;
	}
	void command_line::option_loud_mode(bool new_option_loud_mode) noexcept
	{
		option_loud_mode_ = new_option_loud_mode;
	}
	bool command_line::option_input_end_mode() const noexcept
	{
		return option_input_end_mode_;
	}
	void command_line::option_input_end_mode(bool new_option_input_end_mode) noexcept
	{
		option_input_end_mode_ = new_option_input_end_mode;
	}

	std::string command_line::option_code_path() const
	{
		return option_code_path_;
	}
	void command_line::option_code_path(const std::string& new_option_code_path)
	{
		option_code_path_ = new_option_code_path;
	}
}