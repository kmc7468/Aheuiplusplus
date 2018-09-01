#ifndef AHEUIPLUSPLUS_HEADER_ELEMENT_HPP
#define AHEUIPLUSPLUS_HEADER_ELEMENT_HPP

#include <Aheuiplusplus/function.hpp>

#include <cstdint>
#include <memory>
#include <variant>
#include <vector>

namespace app
{
	enum class element_type
	{
		none,
		array		= 0b0100000,
		reference	= 0b1000000,

		number		= 0b0000001,
		pointer		= 0b0000010,
		instance	= 0b0000100,
		function	= 0b0001000,
		type		= 0b0010000,

		array_of_number = number | array,
		array_of_pointer = pointer | array,
		array_of_instance = instance | array,
		array_of_function = function | array,
		array_of_type = type | array,

		reference_of_number = number | reference,
		reference_of_pointer = pointer | reference,
		reference_of_instance = instance | reference,
		reference_of_function = function | reference,
		reference_of_type = type | reference,

		reference_of_array_of_number = array_of_number | reference,
		reference_of_array_of_pointer = array_of_pointer | reference,
		reference_of_array_of_instance = array_of_instance | reference,
		reference_of_array_of_function = array_of_function | reference,
		reference_of_array_of_type = array_of_type | reference,
	};

	using element_element =
		std::variant<std::variant<long long, double>,	// number
					 std::uintptr_t,					// pointer
					 // TODO: instance
					 function_ptr,						// function
					 element_type						// type
					>;
	using element_base = std::variant<element_element, std::vector<element_element>>;
	using element = std::variant<element_base, element_base*>;
	using element_ptr = std::shared_ptr<element>;

	element_type get_element_type(const element_element& element) noexcept;
	element_type get_element_type(const element_base& element) noexcept;
	element_type get_element_type(const element& element) noexcept;

	const element_base& dereference(const element& element) noexcept;
	element_base& dereference(element& element) noexcept;
}

#endif