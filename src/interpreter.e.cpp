#include <Aheuiplusplus/interpreter.hpp>

namespace app
{
	bool interpreter::add_(char32_t jongsung, bool is_added_additional_data)
	{
		if (jongsung == 0 && !is_added_additional_data)
		{
			element* right_operand = storage_()->pop();
			element* left_operand = storage_()->pop();

			if (is_integer_mode_)
			{
				long long right_operand_integer = std::get<0>(*right_operand).integer();
				long long left_operand_integer = std::get<0>(*left_operand).integer();

				long long result = left_operand_integer + right_operand_integer;

				storage_()->push(new element(number(result)));
			}
			else
			{
				double right_operand_decimal = std::get<0>(*right_operand).decimal();
				double left_operand_decimal = std::get<0>(*left_operand).decimal();

				double result = left_operand_decimal + right_operand_decimal;

				storage_()->push(new element(number(result)));
			}

			delete right_operand;
			delete left_operand;

			return false;
		}
		else if (jongsung == 0 && is_added_additional_data)
		{
			element* right_operand = storage_()->pop();
			element* left_operand = storage_()->pop();

			if (is_integer_mode_)
			{
				long long right_operand_integer = std::get<0>(*right_operand).integer();
				long long left_operand_integer = std::get<0>(*left_operand).integer();

				long long result = left_operand_integer | right_operand_integer;

				storage_()->push(new element(number(result)));
			}
			else
			{
				double right_operand_decimal = std::get<0>(*right_operand).decimal();
				double left_operand_decimal = std::get<0>(*left_operand).decimal();

				static_assert(sizeof(long long) == sizeof(double), "The sizes of long long and double types must be the same.");

				long long right_operand_integer = *reinterpret_cast<long long*>(&right_operand_decimal);
				long long left_operand_integer = *reinterpret_cast<long long*>(&left_operand_decimal);

				long long result = left_operand_integer + right_operand_integer;

				storage_()->push(new element(number(result)));
			}

			delete right_operand;
			delete left_operand;

			return false;
		}

		return true;
	}
	bool interpreter::mul_(char32_t jongsung, bool is_added_additional_data)
	{
		if (jongsung == 0 && !is_added_additional_data)
		{
			element* right_operand = storage_()->pop();
			element* left_operand = storage_()->pop();

			if (is_integer_mode_)
			{
				long long right_operand_integer = std::get<0>(*right_operand).integer();
				long long left_operand_integer = std::get<0>(*left_operand).integer();

				long long result = left_operand_integer * right_operand_integer;

				storage_()->push(new element(number(result)));
			}
			else
			{
				double right_operand_decimal = std::get<0>(*right_operand).decimal();
				double left_operand_decimal = std::get<0>(*left_operand).decimal();

				double result = left_operand_decimal * right_operand_decimal;

				storage_()->push(new element(number(result)));
			}

			delete right_operand;
			delete left_operand;

			return false;
		}
		else if (jongsung == 0 && is_added_additional_data)
		{
			element* right_operand = storage_()->pop();
			element* left_operand = storage_()->pop();

			if (is_integer_mode_)
			{
				long long right_operand_integer = std::get<0>(*right_operand).integer();
				long long left_operand_integer = std::get<0>(*left_operand).integer();

				long long result = left_operand_integer & right_operand_integer;

				storage_()->push(new element(number(result)));
			}
			else
			{
				double right_operand_decimal = std::get<0>(*right_operand).decimal();
				double left_operand_decimal = std::get<0>(*left_operand).decimal();

				long long right_operand_integer = *reinterpret_cast<long long*>(&right_operand_decimal);
				long long left_operand_integer = *reinterpret_cast<long long*>(&left_operand_decimal);

				long long result = left_operand_integer & right_operand_integer;

				storage_()->push(new element(number(result)));
			}

			delete right_operand;
			delete left_operand;

			return false;
		}

		return true;
	}
	bool interpreter::sub_(char32_t jongsung, bool is_added_additional_data)
	{
		if (jongsung == 0 && !is_added_additional_data)
		{
			element* right_operand = storage_()->pop();
			element* left_operand = storage_()->pop();

			if (is_integer_mode_)
			{
				long long right_operand_integer = std::get<0>(*right_operand).integer();
				long long left_operand_integer = std::get<0>(*left_operand).integer();

				long long result = left_operand_integer - right_operand_integer;

				storage_()->push(new element(number(result)));
			}
			else
			{
				double right_operand_decimal = std::get<0>(*right_operand).decimal();
				double left_operand_decimal = std::get<0>(*left_operand).decimal();

				double result = left_operand_decimal - right_operand_decimal;

				storage_()->push(new element(number(result)));
			}

			delete right_operand;
			delete left_operand;

			return false;
		}
		else if (jongsung == 0 && is_added_additional_data)
		{
			element* right_operand = storage_()->pop();
			element* left_operand = storage_()->pop();

			long long right_operand_integer = std::get<0>(*right_operand).integer();
			long long left_operand_integer = std::get<0>(*left_operand).integer();

			long long result = left_operand_integer << right_operand_integer;

			storage_()->push(new element(number(result)));

			delete right_operand;
			delete left_operand;

			return false;
		}

		return true;
	}
	bool interpreter::div_(char32_t jongsung, bool is_added_additional_data)
	{
		if (jongsung == 0 && !is_added_additional_data)
		{
			element* right_operand = storage_()->pop();
			element* left_operand = storage_()->pop();

			if (is_integer_mode_)
			{
				long long right_operand_integer = std::get<0>(*right_operand).integer();
				long long left_operand_integer = std::get<0>(*left_operand).integer();

				long long result = left_operand_integer / right_operand_integer;

				storage_()->push(new element(number(result)));
			}
			else
			{
				double right_operand_decimal = std::get<0>(*right_operand).decimal();
				double left_operand_decimal = std::get<0>(*left_operand).decimal();

				double result = left_operand_decimal / right_operand_decimal;

				storage_()->push(new element(number(result)));
			}

			delete right_operand;
			delete left_operand;

			return false;
		}
		else if (jongsung == 0 && is_added_additional_data)
		{
			element* right_operand = storage_()->pop();
			element* left_operand = storage_()->pop();

			long long right_operand_integer = std::get<0>(*right_operand).integer();
			long long left_operand_integer = std::get<0>(*left_operand).integer();

			long long result = left_operand_integer >> right_operand_integer;

			storage_()->push(new element(number(result)));

			delete right_operand;
			delete left_operand;

			return false;
		}

		return true;
	}
	bool interpreter::mod_(char32_t jongsung, bool is_added_additional_data)
	{
		if (jongsung == 0 && !is_added_additional_data)
		{
			element* right_operand = storage_()->pop();
			element* left_operand = storage_()->pop();

			if (is_integer_mode_)
			{
				long long right_operand_integer = std::get<0>(*right_operand).integer();
				long long left_operand_integer = std::get<0>(*left_operand).integer();

				long long result = left_operand_integer % right_operand_integer;

				storage_()->push(new element(number(result)));
			}
			else
			{
				double right_operand_decimal = std::get<0>(*right_operand).decimal();
				double left_operand_decimal = std::get<0>(*left_operand).decimal();

				double result = std::fmod(left_operand_decimal, right_operand_decimal);

				storage_()->push(new element(number(result)));
			}

			delete right_operand;
			delete left_operand;

			return false;
		}
		else if (jongsung == 0 && is_added_additional_data)
		{
			element* right_operand = storage_()->pop();
			element* left_operand = storage_()->pop();

			if (is_integer_mode_)
			{
				long long right_operand_integer = std::get<0>(*right_operand).integer();
				long long left_operand_integer = std::get<0>(*left_operand).integer();

				long long result = left_operand_integer ^ right_operand_integer;

				storage_()->push(new element(number(result)));
			}
			else
			{
				double right_operand_decimal = std::get<0>(*right_operand).decimal();
				double left_operand_decimal = std::get<0>(*left_operand).decimal();

				long long right_operand_integer = *reinterpret_cast<long long*>(&right_operand_decimal);
				long long left_operand_integer = *reinterpret_cast<long long*>(&left_operand_decimal);

				long long result = left_operand_integer ^ right_operand_integer;

				storage_()->push(new element(number(result)));
			}

			delete right_operand;
			delete left_operand;

			return false;
		}

		return true;
	}
}