#include <Aheuiplusplus/function.hpp>

#include <utility>

namespace app
{
	function::function(const code& name)
		: name_(name)
	{}

	code function::name() const
	{
		return name_;
	}
	void function::name(const code& new_name)
	{
		name_ = new_name;
	}
	void function::name(code&& new_name) noexcept
	{
		name_ = std::move(new_name);
	}
}

namespace app
{
	aheuiaheuiplus_function::aheuiaheuiplus_function(const app::code& name)
		: function(name)
	{}

	function_type aheuiaheuiplus_function::type() const noexcept
	{
		return function_type::aheuiplusplus_function;
	}

	app::code aheuiaheuiplus_function::code() const
	{
		return code_;
	}
	void aheuiaheuiplus_function::code(const app::code& new_code)
	{
		code_ = new_code;
	}
	void aheuiaheuiplus_function::code(app::code&& new_code) noexcept
	{
		code_ = std::move(new_code);
	}
}

namespace app
{
	native_function::native_function(const function_t& functor)
		: functor_(functor)
	{}
	native_function::native_function(const code& name, const function_t& functor)
		: function(name), functor_(functor)
	{}

	function_type native_function::type() const noexcept
	{
		return function_type::native_function;
	}

	const native_function::function_t& native_function::functor() const noexcept
	{
		return functor_;
	}
}