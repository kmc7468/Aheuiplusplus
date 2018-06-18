#ifndef AHEUIPLUSPLUS_HEADER_DEBUGGER_HPP
#define AHEUIPLUSPLUS_HEADER_DEBUGGER_HPP

#include <Aheuiplusplus/code.hpp>
#include <Aheuiplusplus/interpreter.hpp>

#include <cstddef>
#include <cstdio>
#include <utility>
#include <vector>

namespace app
{
	class debugger final
	{
		friend class app::interpreter;

	public:
		debugger(std::FILE* output_stream, app::interpreter& interpreter) noexcept;
		debugger(const debugger& debugger) = delete;
		debugger(debugger&& debugger) noexcept = delete;
		~debugger();

	public:
		debugger& operator=(const debugger& debugger) = delete;
		debugger& operator=(debugger&& debugger) noexcept = delete;
		bool operator==(const debugger& debugger) const = delete;
		bool operator!=(const debugger& debugger) const = delete;

	public:
		void connect_debugger();
		void disconnect_debugger() noexcept;
		bool is_connceted_debugger() const noexcept;

		void dump_storages() const;
		void dump_storages(std::size_t depth) const;
		void dump_storage(char32_t storage) const;
		void dump_storage(std::size_t depth, char32_t storage) const;

		void add_breakpoint(std::size_t x, std::size_t y);
		void remove_breakpoint(std::size_t x, std::size_t y);
		bool is_added_breakpoint(std::size_t x, std::size_t y);

		long long run_with_debugging(const raw_code& code);
		long long run_with_debugging(const raw_code& code,
			bool command_aheui, bool command_loud_mode);

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
		bool is_last_input_utf16() const;
		void is_last_input_utf16(bool new_is_last_input_utf16);
#endif
		bool is_processed_space() const;
		void is_processed_space(bool new_is_proecessed_space);

	public:
		const std::vector<std::pair<std::size_t, std::size_t>>& breakpoints() const noexcept;
		bool is_inputed() const noexcept;
		void is_inputed(bool new_is_inputed) noexcept;

		std::FILE* output_stream() const noexcept;

	private:
		std::vector<std::pair<std::size_t, std::size_t>> breakpoints_;
		bool is_inputed_ = false;

		std::FILE* output_stream_;
		app::interpreter& interpreter_;
	};
}

#endif