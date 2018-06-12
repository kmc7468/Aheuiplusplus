#include <Aheuiplusplus/interpreter.hpp>

#include <memory>

namespace app
{
	namespace
	{
		void type_casting_arithmetic_(element* left_operand, element* right_operand,
			std::shared_ptr<element>& left_operand_out, std::shared_ptr<element>& right_operand_out,
			bool is_integer_mode)
		{
			if (left_operand->index() == right_operand->index())
			{
				left_operand_out = std::make_shared<element>(*left_operand);
				right_operand_out = std::make_shared<element>(*right_operand);
			}
			else
			{
				switch (left_operand->index())
				{
				case 0:
				{
					switch (right_operand->index())
					{
					case 1: // 숫자, 문자 = 숫자
					{
						if (is_integer_mode)
						{
							left_operand_out = std::make_shared<element>(*left_operand);
							right_operand_out = std::make_shared<element>(
								number(static_cast<long long>(std::get<1>(*right_operand))));
						}
						else
						{
							left_operand_out = std::make_shared<element>(
								number(static_cast<double>(std::get<0>(*left_operand).decimal())));
							right_operand_out = std::make_shared<element>(
								number(static_cast<double>(std::get<1>(*right_operand))));
						}

						break;
					}

					case 2: // 숫자, 문자열 = 문자열
					{
						left_operand_out = std::make_shared<element>(
							raw_code(1, static_cast<char32_t>(std::get<0>(*left_operand).integer())));
						right_operand_out = std::make_shared<element>(*right_operand);

						break;
					}
					}
					break;
				}

				case 1:
				{
					switch (right_operand->index())
					{
					case 0: // 문자, 숫자 = 숫자
					{
						if (is_integer_mode)
						{
							left_operand_out = std::make_shared<element>(
								number(static_cast<long long>(std::get<1>(*left_operand))));
							right_operand_out = std::make_shared<element>(*right_operand);
						}
						else
						{
							left_operand_out = std::make_shared<element>(
								number(static_cast<double>(std::get<1>(*left_operand))));
							right_operand_out = std::make_shared<element>(
									number(static_cast<double>(std::get<0>(*right_operand).decimal())));
						}

						break;
					}

					case 2: // 문자 + 문자열 = 문자열
					{
						left_operand_out = std::make_shared<element>(
							raw_code(1, std::get<1>(*left_operand)));
						right_operand_out = std::make_shared<element>(*right_operand);

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
						left_operand_out = std::make_shared<element>(*left_operand);
						right_operand_out = std::make_shared<element>(
							raw_code(1, static_cast<char32_t>(std::get<0>(*right_operand).integer())));

						break;
					}

					case 1: // 문자열 + 문자 = 문자열
					{
						left_operand_out = std::make_shared<element>(*left_operand);
						right_operand_out = std::make_shared<element>(
							raw_code(1, std::get<1>(*right_operand)));

						break;
					}
					}
					break;
				}
				}
			}
		}
	}

	bool interpreter::add_(char32_t jongsung, bool is_added_additional_data)
	{
		if (jongsung == 0 && !is_added_additional_data)
		{
			element* right_operand = storage_()->pop();
			element* left_operand = storage_()->pop();

			std::shared_ptr<element> right_operand_converted;
			std::shared_ptr<element> left_operand_converted;
			type_casting_arithmetic_(left_operand, right_operand, left_operand_converted, right_operand_converted, is_integer_mode_);

			switch (left_operand_converted->index())
			{
			case 0:
			{
				if (is_integer_mode_)
				{
					storage_()->push(new element(number(
						std::get<0>(*left_operand_converted).integer() + std::get<0>(*right_operand_converted).integer()
					)));
				}
				else
				{
					storage_()->push(new element(number(
						std::get<0>(*left_operand_converted).decimal() + std::get<0>(*right_operand_converted).decimal()
					)));
				}

				break;
			}

			case 1:
			{
				storage_()->push(new element(
					std::get<1>(*left_operand_converted) + std::get<1>(*right_operand_converted)
				));

				break;
			}

			case 2:
			{
				storage_()->push(new element(
					std::get<2>(*left_operand_converted) + std::get<2>(*right_operand_converted)
				));

				break;
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

			if (left_operand->index() >= 2 && right_operand->index() >= 2)
			{
				storage_()->push(left_operand);
				storage_()->push(right_operand);

				return true;
			}

			std::shared_ptr<element> right_operand_converted;
			std::shared_ptr<element> left_operand_converted;
			type_casting_arithmetic_(left_operand, right_operand, left_operand_converted, right_operand_converted, is_integer_mode_);

			switch (left_operand_converted->index())
			{
			case 0:
			{
				if (is_integer_mode_)
				{
					storage_()->push(new element(number(
						std::get<0>(*left_operand_converted).integer() | std::get<0>(*right_operand_converted).integer()
					)));
				}
				else
				{
					double left_operand_decimal = std::get<0>(*left_operand_converted).decimal();
					double right_operand_decimal = std::get<0>(*right_operand_converted).decimal();

					long long left_operand_integer = *reinterpret_cast<long long*>(&left_operand_decimal);
					long long right_operand_integer = *reinterpret_cast<long long*>(&right_operand_decimal);
					
					static_assert(sizeof(long long) == sizeof(double), "The sizes of long long and double types must be the same.");

					long long result = left_operand_integer | right_operand_integer;
					double result_decimal = *reinterpret_cast<double*>(&result);

					storage_()->push(new element(number(
						result_decimal
					)));
				}

				break;
			}

			case 1:
			{
				storage_()->push(new element(
					std::get<1>(*left_operand_converted) | std::get<1>(*right_operand_converted)
				));

				break;
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