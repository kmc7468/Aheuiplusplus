#include <Aheuiplusplus/interpreter.hpp>

namespace app
{
	bool interpreter::change_storage_(char32_t jongsung, bool is_added_additional_data)
	{
		if (is_added_additional_data)
		{
			return true;
		}

		if (jongsung == 0)
		{
			selected_index_ = 0;
		}
		else
		{
			selected_index_ = jongsung - U'¤¡' + 1;
		}

		return false;
	}
	bool interpreter::move_(char32_t jongsung, bool is_added_additional_data)
	{
		std::size_t selected_index = jongsung == 0 ? 0 : (jongsung - U'¤¡' + 1);

		element* value = storage_()->pop();
		
		if (value == nullptr)
		{
			return true;
		}

		storages_[selected_index][storage_indexs_[selected_index]]->push(value);

		if (value->index() == 0 && is_added_additional_data)
		{
			number& value_number = std::get<0>(*value);
			value_number.is_integer(true);
		}

		return false;
	}
}