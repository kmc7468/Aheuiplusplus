#include <Aheuiplusplus/element.hpp>

namespace app
{
	number::number(long long integer) noexcept
		: value_(integer)
	{}
	number::number(double decimal) noexcept
		: value_(decimal)
	{}
	number::number(const number& number) noexcept
		: value_(number.value_)
	{}

	number& number::operator=(const number& number) noexcept
	{
		value_ = number.value_;
		return *this;
	}

	long long number::integer() const noexcept
	{
		if (is_integer())
		{
			return std::get<0>(value_);
		}
		else
		{
			return static_cast<long long>(std::get<1>(value_));
		}
	}
	void number::integer(long long new_integer) noexcept
	{
		value_ = new_integer;
	}
	double number::decimal() const noexcept
	{
		if (is_integer())
		{
			return static_cast<double>(std::get<0>(value_));
		}
		else
		{
			return std::get<1>(value_);
		}
	}
	void number::decimal(double new_decimal) noexcept
	{
		value_ = new_decimal;
	}
	bool number::is_integer() const noexcept
	{
		return value_.index() == 0;
	}
	void number::is_integer(bool new_is_integer) noexcept
	{
		if (is_integer() != new_is_integer)
		{
			if (new_is_integer)
			{
				value_ = static_cast<long long>(std::get<1>(value_));
			}
			else
			{
				value_ = static_cast<double>(std::get<0>(value_));
			}
		}
	}
}