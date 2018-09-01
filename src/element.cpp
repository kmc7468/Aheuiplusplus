#include <Aheuiplusplus/element.hpp>

namespace app
{
	element_type get_element_type(const element_element & element) noexcept
	{
		switch (element.index())
		{
		case 0:
			return element_type::number;

		case 1:
			return element_type::pointer;

		case 2:
			return element_type::instance;

		case 3:
			return element_type::function;

		case 4:
			return element_type::type;

		default:
			return element_type::none;
		}
	}
	element_type get_element_type(const app::element_base& element) noexcept
	{
		if (element.index() == 0)
		{
			return get_element_type(std::get<0>(element));
		}

		return static_cast<element_type>(
			static_cast<int>(
				get_element_type(std::get<1>(element)[0])
				) | static_cast<int>(element_type::array)
			);
	}
	element_type get_element_type(const app::element& element) noexcept
	{
		if (element.index() == 0)
		{
			return get_element_type(std::get<0>(element));
		}

		return static_cast<element_type>(
			static_cast<int>(
				get_element_type(*std::get<1>(element))
				) | static_cast<int>(element_type::reference)
			);
	}

	const element_base& dereference(const element& element) noexcept
	{
		if (element.index() == 0)
		{
			return std::get<0>(element);
		}
		else
		{
			return *std::get<1>(element);
		}
	}
	element_base& dereference(element& element) noexcept
	{
		if (element.index() == 0)
		{
			return std::get<0>(element);
		}
		else
		{
			return *std::get<1>(element);
		}
	}
}