#include <Aheuiplusplus/interpreter.hpp>

#include <Aheuiplusplus/debugger.hpp>

#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>

namespace app
{
	interpreter_state::interpreter_state() noexcept
	{
		reset();
	}
	interpreter_state::interpreter_state(const interpreter_state& state) noexcept
		: cursor_(state.cursor_), is_out_of_version_(state.is_out_of_version_)
	{}

	interpreter_state& interpreter_state::operator=(const interpreter_state& state) noexcept
	{
		cursor_ = state.cursor_;
		is_out_of_version_ = state.is_out_of_version_;

		return *this;
	}

	void interpreter_state::reset() noexcept
	{
		cursor_.x() = 0;
		cursor_.y() = 0;
		cursor_.direction() = direction::down;
		cursor_.speed() = 1;

		is_out_of_version_ = false;

		mode_.reset();
	}

	cursor interpreter_state::cursor() const noexcept
	{
		return cursor_;
	}
	bool interpreter_state::is_out_of_version() const noexcept
	{
		return is_out_of_version_;
	}
	app::mode interpreter_state::mode() const noexcept
	{
		return mode_;
	}
}

namespace app
{
	interpreter::interpreter(const command_line& command_line)
		: interpreter(nullptr, std::cin, std::cout, command_line)
	{}
	interpreter::interpreter(std::istream& input_stream, std::ostream& output_stream, const command_line& command_line)
		: interpreter(nullptr, input_stream, output_stream, command_line)
	{}

	interpreter::interpreter(debugger* debugger, const command_line& command_line)
		: interpreter(debugger, std::cin, std::cout, command_line)
	{}
	interpreter::interpreter(debugger* debugger, std::istream& input_stream, std::ostream& output_stream, const command_line& command_line)
		: debugger_(debugger), input_stream_(input_stream), output_stream_(output_stream)
	{
		version_ = command_line.option_version();

		if (version_ == version::none)
			throw std::invalid_argument("인수 command_line이 올바르지 않은 값을 갖고 있습니다. (필드 option_version)");

		namespaces_.push_back(
			std::make_shared<namespace_info>(code_view(U""))
		);
	}

	void interpreter::reset_state() noexcept
	{
		state_.reset();
	}
	void interpreter::reset_storages()
	{
		storages_.reset();
	}

	const app::code& interpreter::code() const noexcept
	{
		return code_;
	}
	void interpreter::code(const app::code_view& new_code)
	{
		code_ = new_code;
		reset_state();
	}
	void interpreter::code(app::code&& new_code) noexcept
	{
		code_ = std::move(new_code);
		reset_state();
	}

	const std::vector<namespace_ptr>& interpreter::namespaces() const noexcept
	{
		return namespaces_;
	}
	std::vector<namespace_ptr>& interpreter::namespaces() noexcept
	{
		return namespaces_;
	}
}