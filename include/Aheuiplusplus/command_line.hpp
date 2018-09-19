#ifndef AHUEIPLUSPLUS_HEADER_COMMAND_LINE_HPP
#define AHUEIPLUSPLUS_HEADER_COMMAND_LINE_HPP

#include <Aheuiplusplus/version.hpp>

#include <ostream>
#include <string>
#include <string_view>

namespace app
{
	class command_line final
	{
	public:
		command_line() = default;
		command_line(const command_line& data);
		command_line(command_line&& data) noexcept;
		~command_line() = default;

	public:
		command_line& operator=(const command_line& data);
		command_line& operator=(command_line&& data) noexcept;
		bool operator==(const command_line& data) const = delete;
		bool operator!=(const command_line& data) const = delete;

	public:
		bool parse(int argc, char** argv);
		bool parse(std::ostream& error_stream, int argc, char** argv);

	public:
		bool option_aheui() const noexcept;
		void option_aheui(bool new_option_aheui) noexcept;
		bool option_interpreting_mode() const noexcept;
		void option_interpreting_mode(bool new_option_interpreting_mode) noexcept;
		bool option_debugging_mode() const noexcept;
		void option_debugging_mode(bool new_option_debugging_mode) noexcept;
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

		std::string option_code_path() const;
		void option_code_path(const std::string_view& new_option_code_path);
		void option_code_path(std::string&& new_option_code_path);

	private:
		bool option_aheui_ = false;
		bool option_interpreting_mode_ = false;
		bool option_debugging_mode_ = false;
		version option_version_ = version::none;
		bool option_utf8_ = false;
		bool option_utf16_ = false;
		bool option_utf16be_ = false;

		bool option_loud_mode_ = false;
		
		std::string option_code_path_;
	};
}

#endif