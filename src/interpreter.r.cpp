#include <Aheuiplusplus/interpreter.hpp>

namespace app
{
	bool interpreter::type_and_mode_(char32_t jongsung, bool is_added_additional_data, char32_t& start_of_expression,
		std::size_t processed_command_without_expression, bool& is_out_of_version)
	{
		if (jongsung == U'ㅁ' && !is_added_additional_data)
		{
			is_integer_mode_ = true;

			return false;
		}
		else if (jongsung == U'ㅁ' && is_added_additional_data)
		{
			is_integer_mode_ = false;

			return false;
		}
		else if (jongsung == U'ㅂ' && !is_added_additional_data)
		{
			is_integer_mode_ = !is_integer_mode_;

			return false;
		}
		else if (jongsung == U'ㅂ' && is_added_additional_data)
		{
			storage_()->push(new element(number(static_cast<long long>(!is_integer_mode_))));

			return false;
		}
		else if (jongsung == U'ㅄ' && !is_added_additional_data)
		{
			if (processed_command_without_expression > 2)
			{
				return true;
			}
			else if (start_of_expression != 0)
			{
				return true;
			}

			start_of_expression = U'ㅄ';
			storage_backup_and_restore_();

			return false;
		}
		else if (jongsung == U'ㅅ' && !is_added_additional_data)
		{
			if (processed_command_without_expression > 2)
			{
				return true;
			}
			else if (start_of_expression != 0)
			{
				return true;
			}

			start_of_expression = U'ㅅ';
			storage_backup_and_restore_();

			return false;
		}
		else if (jongsung == U'ㅄ' && is_added_additional_data)
		{
			if (start_of_expression == 0 || start_of_expression != U'ㅄ')
			{
				return true;
			}

			element* major = storage_()->pop();

			if (major != nullptr && major->index() <= 1)
			{
				long long major_integer = 0;

				switch (major->index())
				{
				case 0:
					major_integer = std::get<0>(*major).integer();
					break;

				case 1:
					major_integer = static_cast<long long>(std::get<1>(*major));
					break;
				}

				if (major_integer < 1)
				{
					major_integer = 1;
				}

				if (get_major(version_) < major_integer)
				{
					is_out_of_version = true;
				}
			}

			start_of_expression = 0;

			storage_backup_and_restore_();

			return false;
		}
		else if (jongsung == U'ㅅ' && is_added_additional_data)
		{
			if (start_of_expression == 0 || start_of_expression != U'ㅅ')
			{
				return true;
			}

			element* major = storage_()->pop();

			if (major != nullptr && major->index() <= 1)
			{
				long long major_integer = 0;

				switch (major->index())
				{
				case 0:
					major_integer = std::get<0>(*major).integer();
					break;

				case 1:
					major_integer = static_cast<long long>(std::get<1>(*major));
					break;
				}

				if (major_integer < 1)
				{
					major_integer = 1;
				}

				if (get_major(version_) > major_integer)
				{
					is_out_of_version = true;
				}
			}

			start_of_expression = 0;

			storage_backup_and_restore_();

			return false;
		}
		else if (jongsung == U'ㅆ' && !is_added_additional_data)
		{
			is_compatible_with_aheui_ = true;

			return false;
		}
		else if (jongsung == U'ㅆ' && is_added_additional_data)
		{
			is_compatible_with_aheui_ = false;

			return false;
		}

		return true;
	}
}