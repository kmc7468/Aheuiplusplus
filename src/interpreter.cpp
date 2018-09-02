#include <Aheuiplusplus/interpreter.hpp>

#include <Aheuiplusplus/debugger.hpp>

#include <memory>
#include <stdexcept>
#include <utility>

#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
#	include <fcntl.h>
#	include <io.h>
#endif

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
		: interpreter(stdin, stdout, command_line)
	{}
	interpreter::interpreter(std::FILE* input_stream, std::FILE* output_stream, const command_line& command_line)
		: input_stream_(input_stream), output_stream_(output_stream)
	{
#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
		input_stream_mode_ = _setmode(_fileno(input_stream), _O_U16TEXT);
		output_stream_mode_ = _setmode(_fileno(output_stream), _O_U16TEXT);

		if (input_stream_mode_ == -1)
			throw std::runtime_error("인수 input_stream의 변환 모드를 변경하는데 실패했습니다.");
		if (output_stream_mode_ == -1)
			throw std::runtime_error("인수 output_stream의 변환 모드를 변경하는데 실패했습니다.");
#endif

		version_ = command_line.option_version();

		if (version_ == version::none)
			throw std::invalid_argument("인수 command_line이 올바르지 않은 값을 갖고 있습니다. (필드 option_version)");

		namespaces_.push_back(
			std::make_shared<namespace_info>(code_view(U""))
		);
	}
	interpreter::~interpreter()
	{
#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
		if (input_stream_mode_ != -1)
		{
			_setmode(_fileno(input_stream_), input_stream_mode_);
		}
		if (output_stream_mode_ != -1)
		{
			_setmode(_fileno(output_stream_), output_stream_mode_);
		}
#endif
	}

	interpreter::interpreter(debugger* debugger)
		: debugger_(debugger)
	{}
	interpreter::interpreter(debugger* debugger, const app::code& code)
		: debugger_(debugger), code_(code)
	{}
	interpreter::interpreter(debugger* debugger, app::code&& code)
		: debugger_(debugger), code_(std::move(code))
	{}

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
	void interpreter::code(const app::code& new_code)
	{
		code_ = new_code;
		reset_state();
	}
	void interpreter::code(app::code&& new_code) noexcept
	{
		code_ = std::move(new_code);
		reset_state();
	}
}