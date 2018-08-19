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
			std::fprintf(error_stream, "����: �Է��� �����ϴ�. --help �ɼ��� �̿��� ������ Ȯ���Ͻ� �� �ֽ��ϴ�.\n");
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

		static const char* duplicate_message = " �ɼ��� �ι� �̻� ���Ǿ����ϴ�.";
		static const char* invalid_argument_message = " �ɼ��� �μ��� �ùٸ��� �ʽ��ϴ�.";

		for (int i = 1; i < argc; ++i)
		{
			const std::string_view argument = argv[i];

			if (argument == "--help")
			{
				if (option_help)
				{
					std::fprintf(error_stream, "����: %s%s\n", "--help", duplicate_message);
					return false;
				}

				option_help = true;
			}
			else if (argument == "--version")
			{
				if (option_version)
				{
					std::fprintf(error_stream, "����: %s%s\n", "--version", duplicate_message);
					return false;
				}

				option_version = true;
			}

			else if (argument == "-A")
			{
				if (option_aheui_)
				{
					std::fprintf(error_stream, "����: %s%s\n", "-A", duplicate_message);
					return false;
				}

				option_aheui_ = true;
			}
			else if (argument == "-i")
			{
				if (option_interpreting_mode_)
				{
					std::fprintf(error_stream, "����: %s%s\n", "-i", duplicate_message);
					return false;
				}

				option_interpreting_mode_ = true;
			}
			else if (argument.substr(0, 4) == "-std")
			{
				if (argument.length() <= 5)
				{
					std::fprintf(error_stream, "����: %s%s\n", "-std", invalid_argument_message);
					return false;
				}
				else if (argument[4] != '=')
				{
					std::fprintf(error_stream, "����: %s%s\n", "-std", invalid_argument_message);
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
						std::fprintf(error_stream, "����: %s%s\n", "-std", invalid_argument_message);
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
						std::fprintf(error_stream, "����: %s%s\n", "-std", invalid_argument_message);
						return false;
					}
				}
			}
			else if (argument == "-utf8")
			{
				if (option_utf8_)
				{
					std::fprintf(error_stream, "����: %s%s\n", "-utf8", duplicate_message);
					return false;
				}

				option_utf8_ = true;
			}
			else if (argument == "-utf16")
			{
				if (option_utf16_)
				{
					std::fprintf(error_stream, "����: %s%s\n", "-utf16", duplicate_message);
					return false;
				}

				option_utf16_ = true;
			}
			else if (argument == "-utf16be")
			{
				if (option_utf16be_)
				{
					std::fprintf(error_stream, "����: %s%s\n", "-utf16be", duplicate_message);
					return false;
				}

				option_utf16be_ = true;
			}

			else if (argument == "-l")
			{
				if (option_loud_mode_)
				{
					std::fprintf(error_stream, "����: %s%s\n", "-l", duplicate_message);
					return false;
				}

				option_loud_mode_ = true;
			}

			else
			{
				if (argument.front() == '-')
				{
					std::fprintf(error_stream, "����: %s�� �� �� ���� �ɼ��Դϴ�.\n", argument.data());
					return false;
				}
				else if (!option_code_path_.empty())
				{
					std::fprintf(error_stream, "����: ��ΰ� �ΰ� �̻� �ԷµǾ����ϴ�.\n");
					return false;
				}

				option_code_path_ = argument;
			}
		}

		if (option_help)
		{

			std::fprintf(error_stream,
				"����: %s [option(s)...] [path]\n"
				"path�� ����++(�Ǵ� ����) �ڵ尡 ��ϵ� �ؽ�Ʈ ���Ͽ� ���� �ùٸ� ��ο��� �մϴ�(���������� ����� ��� �ʿ����� �ʽ��ϴ�.).\n"
				"\n"
				"--help - ���� �� ����� �ɼ� ����� ���ϴ�.\n"
				"--version - �� ����Ʈ������ ������ ���ϴ�.\n"
				"\n"
				"-A - ���� ���� ���� �����մϴ�. -std �ɼǰ� �Բ� ���� �� �����ϴ�.\n"
				"-i - ���������� ���� �����մϴ�. -utf8, -utf16, -utf16be �ɼǰ� �Բ� ���� �� �����ϴ�.\n"
				"-std=<version> - �� ����Ʈ��� ���� ����++ ǥ���� ������ �����մϴ�. version�� m �Ǵ� m.n�� ���·� �����˴ϴ�(�̶� m�� �ֹ���, n�� �ι����Դϴ�.). -A �ɼǰ� �Բ� ���� �� �����ϴ�.\n"
				"-utf8 - path�� ��Ÿ���� �ؽ�Ʈ ������ ���ڵ��� UTF-8���� ����մϴ�. �⺻������ �� �ɼ��� ����˴ϴ�. -i, -utf16, -utf16be �ɼǰ� �Բ� ���� �� �����ϴ�.\n"
				"-utf16 - path�� ��Ÿ���� �ؽ�Ʈ ������ ���ڵ��� UTF-16LE���� ����մϴ�. -i, -utf8, -utf16be �ɼǰ� �Բ� ���� �� �����ϴ�.\n"
				"-utf16be - path�� ��Ÿ���� �ؽ�Ʈ ������ ���ڵ��� UTF-16BE���� ����մϴ�. -i, -utf8, -utf16 �ɼǰ� �Բ� ���� �� �����ϴ�.\n"
				"\n"
				"-l - ����++(�Ǵ� ����) �ڵ尡 �Է��� ��û���� �� ��û������ ��Ÿ���� �޼����� ����մϴ�.\n",
				argv[0]);

			return false;
		}
		else if (option_version)
		{
			std::fprintf(error_stream,
				"����++ ǥ�� ���������� %s\n%s\n\n�� ����Ʈ����� ���� ����Ʈ�����, �ҽ� �ڵ�� �Ʒ� ������Ʈ���� MIT ���̼����� �����ǰ� �ֽ��ϴ�.\n\n%s\n",
				version_string, "(C) 2018. kmc7468 All rights reserved.", "https://github.com/kmc7468/Aheuiplusplus");

			return false;
		}

		if (option_aheui_ && option_version_ != version::none)
		{
			std::fprintf(error_stream, "����: -A �ɼǰ� -std �ɼ��� �Բ� ���� �� �����ϴ�.\n");
			return false;
		}
		else if (option_interpreting_mode_ && !option_code_path_.empty())
		{
			std::fprintf(error_stream, "����: ���������� ����� ��� path�� �ʿ����� �ʽ��ϴ�.\n");
			return false;
		}
		else if (!option_interpreting_mode_ && option_code_path_.empty())
		{
			std::fprintf(error_stream, "����: �Ϲ� ����� ��� path�� �ʿ��մϴ�.\n");
			return false;
		}
		else if (option_interpreting_mode_ && option_utf8_)
		{
			std::fprintf(error_stream, "����: -i �ɼǰ� -utf8 �ɼ��� �Բ� ���� �� �����ϴ�.\n");
			return false;
		}
		else if (option_interpreting_mode_ && option_utf16_)
		{
			std::fprintf(error_stream, "����: -i �ɼǰ� -utf16 �ɼ��� �Բ� ���� �� �����ϴ�.\n");
			return false;
		}
		else if (option_interpreting_mode_ && option_utf16be_)
		{
			std::fprintf(error_stream, "����: -i �ɼǰ� -utf16be �ɼ��� �Բ� ���� �� �����ϴ�.\n");
			return false;
		}
		else if (option_utf8_ && option_utf16_)
		{
			std::fprintf(error_stream, "����: -utf8 �ɼǰ� -utf16 �ɼ��� �Բ� ���� �� �����ϴ�.\n");
			return false;
		}
		else if (option_utf8_ && option_utf16be_)
		{
			std::fprintf(error_stream, "����: -utf8 �ɼǰ� -utf16be �ɼ��� �Բ� ���� �� �����ϴ�.\n");
			return false;
		}
		else if (option_utf16_ && option_utf16be_)
		{
			std::fprintf(error_stream, "����: -utf16 �ɼǰ� -utf16be �ɼ��� �Բ� ���� �� �����ϴ�.\n");
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