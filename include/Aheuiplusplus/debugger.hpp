﻿#ifndef AHEUIPLUSPLUS_HEADER_DEBUGGER_HPP
#define AHEUIPLUSPLUS_HEADER_DEBUGGER_HPP

#include <Aheuiplusplus/code.hpp>
#include <Aheuiplusplus/command_line.hpp>
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
		long long run_with_debugging(const raw_code& code, const command_line& command_line);

		bool is_integer_mode() const;
		void is_integer_mode(bool new_is_integer_mode);
		bool is_compatible_with_aheui() const;
		void is_compatible_with_aheui(bool new_is_compatible_with_aheui);

	public:
		const std::vector<std::pair<std::size_t, std::size_t>>& breakpoints() const noexcept;

		std::FILE* output_stream() const noexcept;

	private:
		std::vector<std::pair<std::size_t, std::size_t>> breakpoints_;

		std::FILE* output_stream_;
		app::interpreter& interpreter_;
	};
}

#endif