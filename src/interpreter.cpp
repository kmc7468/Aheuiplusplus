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

		reset_namespaces();
	}

	void interpreter::reset_state() noexcept
	{
		state_.reset();
	}
	void interpreter::reset_storages()
	{
		storages_.reset();
	}
	void interpreter::reset_namespaces()
	{
		namespaces_.clear();

		namespaces_.push_back(
			std::make_shared<namespace_info>(code_view(U""))
		);
	}

	namespace_ptr interpreter::create_namespace(const code_view& name)
	{
		namespace_ptr result = std::make_shared<namespace_info>(name);

		return namespaces_.push_back(result), result;
	}
	namespace_ptr interpreter::create_namespace(app::code&& name)
	{
		namespace_ptr result = std::make_shared<namespace_info>(std::move(name));

		return namespaces_.push_back(result), result;
	}
	void interpreter::add_namespace(const namespace_ptr& namespace_info)
	{
		namespaces_.push_back(namespace_info);
	}
	void interpreter::remove_namespace(const namespace_ptr& namespace_info)
	{
		std::vector<namespace_ptr>::iterator iter =
			std::find(namespaces_.begin(), namespaces_.end(), namespace_info);

		if (iter != namespaces_.end())
		{
			namespaces_.erase(iter);
		}
		
		throw std::invalid_argument("인수 namespace_info가 존재하지 않아 삭제할 수 없습니다.");
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