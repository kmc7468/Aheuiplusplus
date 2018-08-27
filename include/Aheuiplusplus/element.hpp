#ifndef AHEUIPLUSPLUS_HEADER_ELEMENT_HPP
#define AHEUIPLUSPLUS_HEADER_ELEMENT_HPP

#include <cstdint>
#include <memory>
#include <variant>
#include <vector>

namespace app
{
	enum class element_type
	{
		none,
		array		= 0b100000,

		number		= 0b000001,
		pointer		= 0b000010,
		instance	= 0b000100,
		function	= 0b001000,
		type		= 0b010000,

		array_of_number = number | array,
		array_of_pointer = pointer | array,
		array_of_instance = instance | array,
		array_of_function = function | array,
		array_of_type = type | array,
	};

	using element_base =
		std::variant<std::variant<long long, double>,	// number
					 std::uintptr_t						// pointer
					 // TODO: instance, function, type
					>;

	using element = std::variant<element_base, std::vector<element_base>>;
	using element_ptr = std::shared_ptr<element>;

	element_type get_element_type(const app::element_base& element) noexcept;
	element_type get_element_type(const app::element& element) noexcept;
}

#endif