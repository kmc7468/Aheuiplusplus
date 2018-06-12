#include <Aheuiplusplus/interpreter.hpp>

namespace app
{
	bool interpreter::add_(char32_t jongsung, bool is_added_additional_data)
	{
		if (jongsung == 0 && !is_added_additional_data)
		{
			element* right_operand = storage_()->pop();
			element* left_operand = storage_()->pop();

			if (left_operand->index() == right_operand->index())
			{
				switch (left_operand->index())
				{
				case 0: // 숫자
				{
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

					break;
				}

				case 1: // 문자
				{
					char32_t right_operand_char = std::get<1>(*right_operand);
					char32_t left_operand_char = std::get<1>(*left_operand);

					char32_t result = left_operand_char + right_operand_char;

					storage_()->push(new element(result));

					break;
				}

				case 2: // 문자열
				{
					raw_code right_operand_string = std::get<2>(*right_operand);
					raw_code left_operand_string = std::get<2>(*right_operand);

					storage_()->push(new element(left_operand_string + right_operand_string));

					break;
				}
				}
			}
			else
			{
				switch (left_operand->index())
				{
				case 0:
				{
					switch (right_operand->index())
					{
					case 1: // 숫자 + 문자 = 숫자
					{
						if (is_integer_mode_)
						{
							long long left_operand_integer = std::get<0>(*left_operand).integer();
							long long right_operand_integer = static_cast<long long>(std::get<1>(*right_operand));
						
							long long result = left_operand_integer + right_operand_integer;

							storage_()->push(new element(number(result)));
						}
						else
						{
							double left_operand_decimal = std::get<0>(*left_operand).decimal();
							double right_operand_decimal = static_cast<double>(std::get<1>(*right_operand));

							double result = left_operand_decimal + right_operand_decimal;

							storage_()->push(new element(number(result)));
						}

						break;
					}

					case 2: // 숫자 + 문자열 = 문자열
					{
						char32_t left_operand_char = static_cast<char32_t>(std::get<0>(*left_operand).integer());
						raw_code right_operand_string = std::get<2>(*right_operand);

						storage_()->push(new element(left_operand_char + right_operand_string));

						break;
					}
					}
					break;
				}

				case 1:
				{
					switch (right_operand->index())
					{
					case 0: // 문자 + 숫자 = 숫자
					{
						if (is_integer_mode_)
						{
							long long left_operand_integer = static_cast<long long>(std::get<1>(*left_operand));
							long long right_operand_integer = std::get<0>(*right_operand).integer();

							long long result = left_operand_integer + right_operand_integer;

							storage_()->push(new element(number(result)));
						}
						else
						{
							double left_operand_decimal = static_cast<double>(std::get<1>(*left_operand));
							double right_operand_decimal = std::get<0>(*right_operand).decimal();

							double result = left_operand_decimal + right_operand_decimal;

							storage_()->push(new element(number(result)));
						}

						break;
					}

					case 2: // 문자 + 문자열 = 문자열
					{
						char32_t left_operand_char = std::get<1>(*left_operand);
						raw_code right_operand_string = std::get<2>(*right_operand);

						storage_()->push(new element(left_operand_char + left_operand_char));

						break;
					}
					}
					break;
				}

				case 2:
				{
					switch (right_operand->index())
					{
					case 0: // 문자열 + 숫자 = 문자열
					{
						raw_code left_operand_string = std::get<2>(*left_operand);
						char32_t right_operand_char = static_cast<char32_t>(std::get<0>(*right_operand).integer());

						storage_()->push(new element(left_operand_string + right_operand_char));

						break;
					}

					case 1: // 문자열 + 문자 = 문자열
					{
						raw_code left_operand_string = std::get<2>(*left_operand);
						char32_t right_operand_char = std::get<1>(*right_operand);

						storage_()->push(new element(left_operand_string + right_operand_char));

						break;
					}
					}
					break;
				}
				}
			}

			delete right_operand;
			delete left_operand;

			return false;
		}
		else if (jongsung == 0 && is_added_additional_data)
		{
			element* right_operand = storage_()->pop();
			element* left_operand = storage_()->pop();

			if (left_operand->index() == right_operand->index())
			{
				switch (left_operand->index())
				{
				case 0: // 숫자
				{
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

					break;
				}

				case 1: // 문자
				{
					char32_t right_operand_char = std::get<1>(*right_operand);
					char32_t left_operand_char = std::get<1>(*left_operand);

					char32_t result = left_operand_char | right_operand_char;

					storage_()->push(new element(result));

					break;
				}
				}
			}
			else
			{
				switch (left_operand->index())
				{
				case 0: // 숫자 | 문자 = 숫자
				{
					if (is_integer_mode_)
					{
						long long right_operand_integer = static_cast<long long>(std::get<1>(*right_operand));
						long long left_operand_integer = std::get<0>(*left_operand).integer();

						long long result = left_operand_integer | right_operand_integer;

						storage_()->push(new element(number(result)));
					}
					else
					{
						double right_operand_decimal = static_cast<double>(std::get<1>(*right_operand));
						double left_operand_decimal = std::get<0>(*left_operand).decimal();
						
						long long right_operand_integer = *reinterpret_cast<long long*>(&right_operand_decimal);
						long long left_operand_integer = *reinterpret_cast<long long*>(&left_operand_decimal);

						long long result = left_operand_integer + right_operand_integer;

						storage_()->push(new element(number(result)));
					}

					break;
				}

				case 1: // 문자 | 숫자 = 숫자
				{
					if (is_integer_mode_)
					{
						long long right_operand_integer = std::get<0>(*right_operand).integer();
						long long left_operand_integer = static_cast<long long>(std::get<1>(*left_operand));

						long long result = left_operand_integer | right_operand_integer;

						storage_()->push(new element(number(result)));
					}
					else
					{
						double right_operand_decimal = std::get<0>(*right_operand).decimal();
						double left_operand_decimal = static_cast<double>(std::get<1>(*left_operand));

						long long right_operand_integer = *reinterpret_cast<long long*>(&right_operand_decimal);
						long long left_operand_integer = *reinterpret_cast<long long*>(&left_operand_decimal);

						long long result = left_operand_integer + right_operand_integer;

						storage_()->push(new element(number(result)));
					}

					break;
				}
				}
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