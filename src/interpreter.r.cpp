#include <Aheuiplusplus/interpreter.hpp>

namespace app
{
	bool interpreter::type_and_mode_(char32_t jongsung, bool is_added_additional_data)
	{
		if (jongsung == U'け' && !is_added_additional_data)
		{
			is_integer_mode_ = true;
		}
		else if (jongsung == U'け' && is_added_additional_data)
		{
			is_integer_mode_ = false;
		}
		else if (jongsung == U'げ' && !is_added_additional_data)
		{
			is_integer_mode_ = !is_integer_mode_;
		}
		else if (jongsung == U'げ' && is_added_additional_data)
		{
			storage_()->push(new element(number(static_cast<long long>(!is_integer_mode_))));
		}
		else if (jongsung == U'こ' && is_added_additional_data)
		{
			is_compatible_with_aheui_ = true;
		}
		else if (jongsung == U'ご' && is_added_additional_data)
		{
			is_compatible_with_aheui_ = false;
		}

		return true;
	}
}