#include <Aheuiplusplus/interpreter.hpp>

#include <Aheuiplusplus/debugger.hpp>

#include <cmath>
#include <cctype>
#include <cstdio>
#include <cwchar>
#include <cwctype>
#include <functional>

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#	include <fcntl.h>
#	include <io.h>
#endif

namespace app
{
	bool interpreter::pop_(char32_t jongsung, bool is_added_additional_data)
	{
		element* value = storage_()->pop();

		if (value != nullptr)
		{
			if (jongsung == U'ㅇ' && !is_added_additional_data) // 숫자(정수) 출력
			{
				if (value->index() == 0) // 숫자일 경우
				{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
					_setmode(_fileno(output_stream_), _O_TEXT);
#endif
					std::fprintf(output_stream_, "%lld", std::get<0>(*value).integer());
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
					_setmode(_fileno(output_stream_), _O_U16TEXT);
#endif
				}
				else if (value->index() == 1) // 문자일 경우
				{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
					_setmode(_fileno(output_stream_), _O_TEXT);
#endif
					std::fprintf(output_stream_, "%lld", static_cast<long long>(std::get<1>(*value)));
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
					_setmode(_fileno(output_stream_), _O_U16TEXT);
#endif
				}
				else if (value->index() == 2) // 문자열일 경우
				{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
					_setmode(_fileno(output_stream_), _O_TEXT);
#endif

					long long value_integer = 0;

					if (!std::get<2>(*value).empty())
					{
						value_integer = static_cast<long long>(std::get<2>(*value)[0]);
					}

					std::fprintf(output_stream_, "%lld", value_integer);

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
					_setmode(_fileno(output_stream_), _O_U16TEXT);
#endif
				}
			}
			else if (jongsung == U'ㅇ' && is_added_additional_data) // 숫자(소수) 출력
			{
				if (value->index() == 0) // 숫자일 경우
				{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
					_setmode(_fileno(output_stream_), _O_TEXT);
#endif
					std::fprintf(output_stream_, "%f", std::get<0>(*value).decimal());
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
					_setmode(_fileno(output_stream_), _O_U16TEXT);
#endif
				}
				else if (value->index() == 1) // 문자일 경우
				{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
					_setmode(_fileno(output_stream_), _O_TEXT);
#endif
					std::fprintf(output_stream_, "%f", static_cast<double>(std::get<1>(*value)));
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
					_setmode(_fileno(output_stream_), _O_U16TEXT);
#endif
				}
				else if (value->index() == 2) // 문자열일 경우
				{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
					_setmode(_fileno(output_stream_), _O_TEXT);
#endif

					double value_integer = 0.0;

					if (!std::get<2>(*value).empty())
					{
						value_integer = static_cast<double>(std::get<2>(*value)[0]);
					}

					std::fprintf(output_stream_, "%f", value_integer);

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
					_setmode(_fileno(output_stream_), _O_U16TEXT);
#endif
				}
			}
			else if (jongsung == U'ㅎ' && !is_added_additional_data) // 문자 출력
			{
				if (value->index() == 0) // 숫자일 경우
				{
					write_char(output_stream_, static_cast<char32_t>(std::get<0>(*value).integer()));
				}
				else if (value->index() == 1) // 문자일 경우
				{
					write_char(output_stream_, std::get<1>(*value));
				}
				else if (value->index() == 2) // 문자열일 경우
				{
					write_char(output_stream_, std::get<2>(*value).empty() ? 0 : std::get<2>(*value).front());
				}
			}
			else if (jongsung == U'ㅎ' && is_added_additional_data) // 문자열 출력
			{
				if (value->index() == 0)
				{
					write_char(output_stream_, static_cast<char32_t>(std::get<0>(*value).integer()));
				}
				else if (value->index() == 1) // 문자일 경우
				{
					write_char(output_stream_, std::get<1>(*value));
				}
				else if (value->index() == 2) // 문자열일 경우
				{
					raw_code value_string = std::get<2>(*value);

					for (char32_t c : value_string)
					{
						write_char(output_stream_, c);
					}
				}
			}
			else if (is_added_additional_data)
			{
				storage_()->push(value);
				value = nullptr;

				return true;
			}
			
			delete value;
			return false;
		}

		return true;
	}
	bool interpreter::push_(char32_t jongsung, bool is_added_additional_data)
	{
		if (jongsung == U'ㅇ' && !is_added_additional_data) // 숫자(정수) 입력
		{
			if (input_stream_ != stdin && std::feof(input_stream_))
			{
				storage_()->push(new element(number(0ll)));
				return false;
			}

			if (is_loud_mode_)
			{
				storage_()->push(new element(U"숫자(정수)를 입력하세요: "));
				pop_(U'ㅎ', true);
			}

			bool is_first = true;
			bool is_first_digit = true;

			std::string number;
			char32_t digit;

			while (digit = read_char(input_stream_))
			{
				if (is_first)
				{
					is_first = false;
					
					if (digit == U' ')
					{
						continue;
					}
				}

				if (digit >= 0x80)
				{
					unread_char(input_stream_, digit);
					break;
				}

				if (std::isdigit(static_cast<char>(digit)))
				{
					is_first_digit = false;

					number += static_cast<char>(digit);
				}
				else
				{
					if (digit == U'-' && is_first_digit)
					{
						is_first_digit = false;
						number += '-';

						continue;
					}
					else if (!(std::isspace(digit) && digit != U' '))
					{
						unread_char(input_stream_, digit);
					}

					break;
				}
			}

			storage_()->push(new element(app::number(std::stoll(number))));
			
			return false;
		}
		else if (jongsung == U'ㅇ' && is_added_additional_data) // 숫자(소수) 입력
		{
			if (input_stream_ != stdin && std::feof(input_stream_))
			{
				storage_()->push(new element(number(0.0)));
				return false;
			}

			if (is_loud_mode_)
			{
				storage_()->push(new element(U"숫자(정수 또는 소수)를 입력하세요: "));
				pop_(U'ㅎ', true);
			}

			bool is_first = true;
			bool is_first_digit = true;

			std::string number;
			char32_t digit;

			while (digit = read_char(input_stream_))
			{
				if (is_first)
				{
					is_first = false;

					if (digit == U' ')
					{
						continue;
					}
				}

				if (digit >= 0x80)
				{
					unread_char(input_stream_, digit);
					break;
				}

				if (std::isdigit(static_cast<char>(digit)) || digit == U'.')
				{
					if (digit == U'.' && number.find('.') != std::string::npos)
					{
						unread_char(input_stream_, digit);
						break;
					}

					is_first_digit = false;

					number += static_cast<char>(digit);
				}
				else
				{
					if (digit == U'-' && is_first_digit)
					{
						is_first_digit = false;
						number += '-';

						continue;
					}
					else if (!(std::isspace(digit) && digit != U' '))
					{
						unread_char(input_stream_, digit);
					}

					break;
				}
			}

			storage_()->push(new element(app::number(std::stod(number))));
			
			return false;
		}
		else if (jongsung == U'ㅎ' && !is_added_additional_data) // 문자 입력
		{
		input_char:
			if (input_stream_ != stdin && std::feof(input_stream_))
			{
				storage_()->push(new element(0));
				return false;
			}

			if (is_loud_mode_)
			{
				storage_()->push(new element(U"문자를 입력하세요: "));
				pop_(U'ㅎ', true);
			}

			char32_t input = read_char(input_stream_);

			if (input < 128)
			{
				if (std::isspace(static_cast<unsigned char>(input)) && input != U' ')
				{
					goto input_char;
				}
			}

			storage_()->push(new element(input));

			return false;
		}
		else if (jongsung == U'ㅎ' && is_added_additional_data) // 문자열 입력
		{
			if (input_stream_ != stdin && std::feof(input_stream_))
			{
				storage_()->push(new element(0));
				return false;
			}

			if (is_loud_mode_)
			{
				storage_()->push(new element(U"문자열을 입력하세요: "));
				pop_(U'ㅎ', true);
			}

			raw_code input;

			while (true)
			{
				char32_t c = read_char(input_stream_);

				if (c < 0x80)
				{
					if (std::isspace(static_cast<unsigned char>(c)))
					{
						break;
					}
				}

				input += c;
			}

			storage_()->push(new element(input));

			return false;
		}
		else
		{
			storage_()->push(new element(number(get_integer_(jongsung, is_added_additional_data))));

			return false;
		}
	}
	bool interpreter::copy_(char32_t jongsung, bool is_added_additional_data)
	{
		if (jongsung == 0)
		{
			element* copyed = storage_()->copy();

			if (copyed == nullptr)
			{
				return true;
			}

			if (is_added_additional_data)
			{
				if (copyed->index() == 0)
				{
					long long floor = std::get<0>(*copyed).integer();
					*copyed = element(number(floor));
				}
			}

			return false;
		}
		else if (jongsung == U'ㄱ')
		{
			element* copyed = storage_()->copy();

			if (copyed == nullptr)
			{
				return true;
			}

			if (copyed->index() == 0)
			{
				if (is_added_additional_data)
				{
					long long abs = static_cast<long long>(std::abs(std::get<0>(*copyed).decimal()));
					*copyed = element(number(abs));
				}
				else
				{
					double abs = std::abs(std::get<0>(*copyed).decimal());
					*copyed = element(number(abs));
				}
			}

			return false;
		}
		else if (jongsung == U'ㄴ')
		{
			element* copyed = storage_()->copy();

			if (copyed == nullptr)
			{
				return true;
			}

			if (copyed->index() == 0)
			{
				long long ceil = static_cast<long long>(std::ceil(std::get<0>(*copyed).decimal()));
				*copyed = element(number(ceil));
			}

			return false;
		}
		else if (jongsung == U'ㄵ')
		{
			element* copyed = storage_()->copy();

			if (copyed == nullptr)
			{
				return true;
			}

			if (copyed->index() == 0)
			{
				if (is_added_additional_data)
				{
					long long pow = static_cast<long long>(std::pow(std::get<0>(*copyed).decimal(), 2));
					*copyed = element(number(pow));
				}
				else
				{
					double pow = std::pow(std::get<0>(*copyed).decimal(), 2);
					*copyed = element(number(pow));
				}
			}

			return false;
		}
		else if (jongsung == U'ㄶ')
		{
			element* copyed = storage_()->copy();

			if (copyed == nullptr)
			{
				return true;
			}

			if (copyed->index() == 0)
			{
				if (is_added_additional_data)
				{
					long long sqrt = static_cast<long long>(std::sqrt(std::get<0>(*copyed).decimal()));
					*copyed = element(number(sqrt));
				}
				else
				{
					double sqrt = std::sqrt(std::get<0>(*copyed).decimal());
					*copyed = element(number(sqrt));
				}
			}

			return false;
		}
		else if (jongsung == U'ㄷ')
		{
			element* copyed = storage_()->copy();

			if (copyed == nullptr)
			{
				return true;
			}

			if (copyed->index() == 0)
			{
				if (is_added_additional_data)
				{
					long long exp = static_cast<long long>(std::exp(std::get<0>(*copyed).decimal()));
					*copyed = element(number(exp));
				}
				else
				{
					double exp = std::exp(std::get<0>(*copyed).decimal());
					*copyed = element(number(exp));
				}
			}

			return false;
		}

		return true;
	}
	bool interpreter::swap_(char32_t jongsung, bool is_added_additional_data)
	{
		if (jongsung == 0 && !is_added_additional_data)
		{
			try
			{
				storage_()->swap();
			}
			catch (std::bad_function_call)
			{
				return true;
			}

			return false;
		}

		return true;
	}
}