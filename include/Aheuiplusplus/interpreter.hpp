#ifndef AHEUIPLUSPLUS_HEADER_INTERPRETER_HPP
#define AHEUIPLUSPLUS_HEADER_INTERPRETER_HPP

#include <Aheuiplusplus/code.hpp>
#include <Aheuiplusplus/cursor.hpp>

namespace app
{
#define AHEUIPLUSPLUS_VERSION_STRING "2.0.0"
#define AHEUIPLUSPLUS_VERSION 2
#define AHEUIPLUSPLUS_VERSION_MAJOR AHEUIPLUSPLUS_VERSION
#define AHEUIPLUSPLUS_VERSION_MINOR 0
#define AHEUIPLUSPLUS_VERSION_PATCH 0

	inline constexpr const char* version_string = AHEUIPLUSPLUS_VERSION_STRING;
	inline constexpr int version_major = AHEUIPLUSPLUS_VERSION_MAJOR;
	inline constexpr int version_minor = AHEUIPLUSPLUS_VERSION_MINOR;
	inline constexpr int version_patch = AHEUIPLUSPLUS_VERSION_PATCH;

	class interpreter;

	class interpreter_state
	{
		friend class interpreter;
		
	public:
		interpreter_state() noexcept;
		interpreter_state(const interpreter_state& state) noexcept;
		~interpreter_state() = default;

	public:
		interpreter_state& operator=(const interpreter_state& state) noexcept;
		bool operator==(const interpreter_state& state) const = delete;
		bool operator!=(const interpreter_state& state) const = delete;

	public:
		void reset() noexcept;

	public:
		app::cursor cursor() const noexcept;
		bool is_out_of_version() const noexcept;

	private:
		app::cursor cursor_;
		bool is_out_of_version_;
	};

	class debugger;

	class interpreter final
	{
		friend class debugger;
	
	public:
		interpreter() = default;
		interpreter(const interpreter& interpreter) = delete;
		interpreter(interpreter&& interpreter) noexcept = delete;
		~interpreter() = default;

	private:
		interpreter(debugger* debugger);
		interpreter(debugger* debugger, const app::code& code);
		interpreter(debugger* debugger, app::code&& code);

	public:
		interpreter& operator=(const interpreter& interpreter) = delete;
		interpreter& operator=(interpreter&& interpreter) noexcept = delete;
		bool operator==(const interpreter& interpreter) const = delete;
		bool operator!=(const interpreter& interpreter) const = delete;

	public:
		void reset_state() noexcept;

	public:
		const app::code& code() const noexcept;
		void code(const app::code& new_code);
		void code(app::code&& new_code) noexcept;

	private:
		app::code code_;
		interpreter_state state_;

		debugger* const debugger_ = nullptr;
	};
}

#endif