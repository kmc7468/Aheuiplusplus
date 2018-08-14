#ifndef AHEUIPLUSPLUS_HEADER_ELEMENT_HPP
#define AHEUIPLUSPLUS_HEADER_ELEMENT_HPP

#include <variant>
#include <vector>

namespace app
{
	enum class element_type
	{
		number		= 0b000001,
		pointer		= 0b000010,
		instance	= 0b000100,
		function	= 0b001000,
		type		= 0b010000,
		array		= 0b100000,

		array_of_number = number | array,
		array_of_pointer = pointer | array,
		array_of_instance = instance | array,
		array_of_function = function | array,
		array_of_type = type | array,
	};

	class number final
	{
	public:
		number() noexcept = default;
		number(long long integer) noexcept;
		number(double decimal) noexcept;
		number(const number& number) noexcept;
		~number() = default;

	public:
		number& operator=(const number& number) noexcept;
		bool operator==(const number& number) const = delete;
		bool operator!=(const number& number) const = delete;

	public:
		long long integer() const noexcept;
		void integer(long long new_integer) noexcept;
		double decimal() const noexcept;
		void decimal(double new_decimal) noexcept;
		bool is_integer() const noexcept;
		void is_integer(bool new_is_integer) noexcept;

	private:
		std::variant<long long, double> value_ = 0ll;
	};
}

#endif