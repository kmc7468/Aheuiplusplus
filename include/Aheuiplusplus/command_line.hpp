#ifndef AHEUIPLUSPLUS_HEADER_PROGRAM_OPTIONS_HPP
#define AHEUIPLUSPLUS_HEADER_PROGRAM_OPTIONS_HPP

#include <Aheuiplusplus/version.hpp>

#include <cstdio>
#include <string>

namespace app
{
	class command_line final
	{
	public:
		command_line() = default;
		command_line(const command_line& data);
		~command_line() = default;

	public:
		command_line& operator=(const command_line& data);
		bool operator==(const command_line& data) const = delete;
		bool operator!=(const command_line& data) const = delete;

	public:
		bool parse(int argc, char** argv);
		bool parse(std::FILE* output_stream, int argc, char** argv);		

	public:
		bool option_aheui() const noexcept;
		void option_aheui(bool new_option_aheui) noexcept;
		bool option_interpreting_mode() const noexcept;
		void option_interpreting_mode(bool new_option_interpreting_mode) noexcept;
		version option_version() const noexcept;
		void option_version(version new_option_version) noexcept;
		bool option_utf8() const noexcept;
		void option_utf8(bool new_option_utf8) noexcept;
		bool option_utf16() const noexcept;
		void option_utf16(bool new_option_utf16) noexcept;
		bool option_utf16be() const noexcept;
		void option_utf16be(bool new_option_utf16be) noexcept;

		bool option_loud_mode() const noexcept;
		void option_loud_mode(bool new_option_loud_mode) noexcept;
		bool option_input_end_mode() const noexcept;
		void option_input_end_mode(bool new_option_input_end_mode) noexcept;

		std::string option_code_path() const;
		void option_code_path(const std::string& new_option_code_path);

	private:
		bool option_aheui_ = false;
		bool option_interpreting_mode_ = false;
		version option_version_ = version::none;
		bool option_utf8_ = false;
		bool option_utf16_ = false;
		bool option_utf16be_ = false;

		bool option_loud_mode_ = false;
		bool option_input_end_mode_ = false;
		
		std::string option_code_path_;
	};
}

#endif