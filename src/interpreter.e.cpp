#include <Aheuiplusplus/interpreter.hpp>

#include <cmath>

namespace app
{
	bool interpreter::add_(char32_t jongsung, bool is_added_additional_data)
	{
		if (!is_added_additional_data)
		{
			element* right_operand = storage_()->pop();
			element* left_operand = storage_()->pop();

			if (right_operand == nullptr)
			{
				return true;
			}
			else if (left_operand == nullptr)
			{
				storage_()->push(right_operand);
				return true;
			}

			std::shared_ptr<element> right_operand_converted;
			std::shared_ptr<element> left_operand_converted;
			type_casting_arithmetic_(left_operand, right_operand, left_operand_converted, right_operand_converted);

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

			if (right_operand == nullptr)
			{
				return true;
			}
			else if (left_operand == nullptr)
			{
				storage_()->push(right_operand);
				return true;
			}

			if (left_operand->index() >= 2 && right_operand->index() >= 2)
			{
				storage_()->push(left_operand);
				storage_()->push(right_operand);

				return true;
			}

			std::shared_ptr<element> right_operand_converted;
			std::shared_ptr<element> left_operand_converted;
			type_casting_arithmetic_(left_operand, right_operand, left_operand_converted, right_operand_converted);

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
		if (!is_added_additional_data)
		{
			element* right_operand = storage_()->pop();
			element* left_operand = storage_()->pop();

			if (right_operand == nullptr)
			{
				return true;
			}
			else if (left_operand == nullptr)
			{
				storage_()->push(right_operand);
				return true;
			}

			if (left_operand->index() >= 2 && right_operand->index() >= 2)
			{
				storage_()->push(left_operand);
				storage_()->push(right_operand);

				return true;
			}

			std::shared_ptr<element> right_operand_converted;
			std::shared_ptr<element> left_operand_converted;
			type_casting_arithmetic_(left_operand, right_operand, left_operand_converted, right_operand_converted);

			switch (left_operand_converted->index())
			{
			case 0:
			{
				if (is_integer_mode_)
				{
					storage_()->push(new element(number(
						std::get<0>(*left_operand_converted).integer() * std::get<0>(*right_operand_converted).integer()
					)));
				}
				else
				{
					storage_()->push(new element(number(
						std::get<0>(*left_operand_converted).decimal() * std::get<0>(*right_operand_converted).decimal()
					)));
				}

				break;
			}

			case 1:
			{
				storage_()->push(new element(
					std::get<1>(*left_operand_converted) * std::get<1>(*right_operand_converted)
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

			if (right_operand == nullptr)
			{
				return true;
			}
			else if (left_operand == nullptr)
			{
				storage_()->push(right_operand);
				return true;
			}

			if (left_operand->index() >= 2 && right_operand->index() >= 2)
			{
				storage_()->push(left_operand);
				storage_()->push(right_operand);

				return true;
			}

			std::shared_ptr<element> right_operand_converted;
			std::shared_ptr<element> left_operand_converted;
			type_casting_arithmetic_(left_operand, right_operand, left_operand_converted, right_operand_converted);

			switch (left_operand_converted->index())
			{
			case 0:
			{
				if (is_integer_mode_)
				{
					storage_()->push(new element(number(
						std::get<0>(*left_operand_converted).integer() & std::get<0>(*right_operand_converted).integer()
					)));
				}
				else
				{
					double left_operand_decimal = std::get<0>(*left_operand_converted).decimal();
					double right_operand_decimal = std::get<0>(*right_operand_converted).decimal();

					long long left_operand_integer = *reinterpret_cast<long long*>(&left_operand_decimal);
					long long right_operand_integer = *reinterpret_cast<long long*>(&right_operand_decimal);

					long long result = left_operand_integer & right_operand_integer;
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
					std::get<1>(*left_operand_converted) & std::get<1>(*right_operand_converted)
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
	bool interpreter::sub_(char32_t jongsung, bool is_added_additional_data)
	{
		if (!is_added_additional_data)
		{
			element* right_operand = storage_()->pop();
			element* left_operand = storage_()->pop();

			if (right_operand == nullptr)
			{
				return true;
			}
			else if (left_operand == nullptr)
			{
				storage_()->push(right_operand);
				return true;
			}

			if (left_operand->index() >= 2 && right_operand->index() >= 2)
			{
				storage_()->push(left_operand);
				storage_()->push(right_operand);

				return true;
			}

			std::shared_ptr<element> right_operand_converted;
			std::shared_ptr<element> left_operand_converted;
			type_casting_arithmetic_(left_operand, right_operand, left_operand_converted, right_operand_converted);

			switch (left_operand_converted->index())
			{
			case 0:
			{
				if (is_integer_mode_)
				{
					storage_()->push(new element(number(
						std::get<0>(*left_operand_converted).integer() - std::get<0>(*right_operand_converted).integer()
					)));
				}
				else
				{
					storage_()->push(new element(number(
						std::get<0>(*left_operand_converted).decimal() - std::get<0>(*right_operand_converted).decimal()
					)));
				}

				break;
			}

			case 1:
			{
				storage_()->push(new element(
					std::get<1>(*left_operand_converted) - std::get<1>(*right_operand_converted)
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

			if (right_operand == nullptr)
			{
				return true;
			}
			else if (left_operand == nullptr)
			{
				storage_()->push(right_operand);
				return true;
			}

			if (left_operand->index() >= 2 && right_operand->index() >= 2)
			{
				storage_()->push(left_operand);
				storage_()->push(right_operand);

				return true;
			}

			std::shared_ptr<element> right_operand_converted;
			std::shared_ptr<element> left_operand_converted;
			type_casting_arithmetic_(left_operand, right_operand, left_operand_converted, right_operand_converted);

			switch (left_operand_converted->index())
			{
			case 0:
			{
				if (is_integer_mode_)
				{
					storage_()->push(new element(number(
						std::get<0>(*left_operand_converted).integer() << std::get<0>(*right_operand_converted).integer()
					)));
				}
				else
				{
					double left_operand_decimal = std::get<0>(*left_operand_converted).decimal();
					double right_operand_decimal = std::get<0>(*right_operand_converted).decimal();

					long long left_operand_integer = *reinterpret_cast<long long*>(&left_operand_decimal);
					long long right_operand_integer = *reinterpret_cast<long long*>(&right_operand_decimal);

					long long result = left_operand_integer << right_operand_integer;
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
					std::get<1>(*left_operand_converted) << std::get<1>(*right_operand_converted)
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
	bool interpreter::div_(char32_t jongsung, bool is_added_additional_data)
	{
		if (!is_added_additional_data)
		{
			element* right_operand = storage_()->pop();
			element* left_operand = storage_()->pop();

			if (right_operand == nullptr)
			{
				return true;
			}
			else if (left_operand == nullptr)
			{
				storage_()->push(right_operand);
				return true;
			}

			if (left_operand->index() >= 2 && right_operand->index() >= 2)
			{
				storage_()->push(left_operand);
				storage_()->push(right_operand);

				return true;
			}

			std::shared_ptr<element> right_operand_converted;
			std::shared_ptr<element> left_operand_converted;
			type_casting_arithmetic_(left_operand, right_operand, left_operand_converted, right_operand_converted);

			switch (left_operand_converted->index())
			{
			case 0:
			{
				if (is_integer_mode_)
				{
					storage_()->push(new element(number(
						std::get<0>(*left_operand_converted).integer() / std::get<0>(*right_operand_converted).integer()
					)));
				}
				else
				{
					storage_()->push(new element(number(
						std::get<0>(*left_operand_converted).decimal() / std::get<0>(*right_operand_converted).decimal()
					)));
				}

				break;
			}

			case 1:
			{
				storage_()->push(new element(
					std::get<1>(*left_operand_converted) / std::get<1>(*right_operand_converted)
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

			if (right_operand == nullptr)
			{
				return true;
			}
			else if (left_operand == nullptr)
			{
				storage_()->push(right_operand);
				return true;
			}

			if (left_operand->index() >= 2 && right_operand->index() >= 2)
			{
				storage_()->push(left_operand);
				storage_()->push(right_operand);

				return true;
			}

			std::shared_ptr<element> right_operand_converted;
			std::shared_ptr<element> left_operand_converted;
			type_casting_arithmetic_(left_operand, right_operand, left_operand_converted, right_operand_converted);

			switch (left_operand_converted->index())
			{
			case 0:
			{
				if (is_integer_mode_)
				{
					storage_()->push(new element(number(
						std::get<0>(*left_operand_converted).integer() >> std::get<0>(*right_operand_converted).integer()
					)));
				}
				else
				{
					double left_operand_decimal = std::get<0>(*left_operand_converted).decimal();
					double right_operand_decimal = std::get<0>(*right_operand_converted).decimal();

					long long left_operand_integer = *reinterpret_cast<long long*>(&left_operand_decimal);
					long long right_operand_integer = *reinterpret_cast<long long*>(&right_operand_decimal);

					long long result = left_operand_integer >> right_operand_integer;
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
					std::get<1>(*left_operand_converted) >> std::get<1>(*right_operand_converted)
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
	bool interpreter::mod_(char32_t jongsung, bool is_added_additional_data)
	{
		if (!is_added_additional_data)
		{
			element* right_operand = storage_()->pop();
			element* left_operand = storage_()->pop();

			if (right_operand == nullptr)
			{
				return true;
			}
			else if (left_operand == nullptr)
			{
				storage_()->push(right_operand);
				return true;
			}

			if (left_operand->index() >= 2 && right_operand->index() >= 2)
			{
				storage_()->push(left_operand);
				storage_()->push(right_operand);

				return true;
			}

			std::shared_ptr<element> right_operand_converted;
			std::shared_ptr<element> left_operand_converted;
			type_casting_arithmetic_(left_operand, right_operand, left_operand_converted, right_operand_converted);

			switch (left_operand_converted->index())
			{
			case 0:
			{
				if (is_integer_mode_)
				{
					storage_()->push(new element(number(
						std::get<0>(*left_operand_converted).integer() % std::get<0>(*right_operand_converted).integer()
					)));
				}
				else
				{
					storage_()->push(new element(number(
						std::fmod(std::get<0>(*left_operand_converted).decimal(), std::get<0>(*right_operand_converted).decimal())
					)));
				}

				break;
			}

			case 1:
			{
				storage_()->push(new element(
					std::get<1>(*left_operand_converted) % std::get<1>(*right_operand_converted)
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

			if (right_operand == nullptr)
			{
				return true;
			}
			else if (left_operand == nullptr)
			{
				storage_()->push(right_operand);
				return true;
			}

			if (left_operand->index() >= 2 && right_operand->index() >= 2)
			{
				storage_()->push(left_operand);
				storage_()->push(right_operand);

				return true;
			}

			std::shared_ptr<element> right_operand_converted;
			std::shared_ptr<element> left_operand_converted;
			type_casting_arithmetic_(left_operand, right_operand, left_operand_converted, right_operand_converted);

			switch (left_operand_converted->index())
			{
			case 0:
			{
				if (is_integer_mode_)
				{
					storage_()->push(new element(number(
						std::get<0>(*left_operand_converted).integer() ^ std::get<0>(*right_operand_converted).integer()
					)));
				}
				else
				{
					double left_operand_decimal = std::get<0>(*left_operand_converted).decimal();
					double right_operand_decimal = std::get<0>(*right_operand_converted).decimal();

					long long left_operand_integer = *reinterpret_cast<long long*>(&left_operand_decimal);
					long long right_operand_integer = *reinterpret_cast<long long*>(&right_operand_decimal);

					long long result = left_operand_integer ^ right_operand_integer;
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
					std::get<1>(*left_operand_converted) ^ std::get<1>(*right_operand_converted)
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
}