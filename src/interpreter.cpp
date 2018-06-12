#include <Aheuiplusplus/interpreter.hpp>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cwchar>
#include <cwctype>
#include <string>

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#	include <fcntl.h>
#	include <io.h>
#endif

namespace app
{
	namespace
	{
		std::wstring char32_to_wchar(char32_t character)
		{
			if (character <= 65'535)
			{
				return std::wstring(1, static_cast<wchar_t>(character));
			}
			
			std::wstring result;
			result.resize(2);

			char32_t temp = character - 0x10000;

			wchar_t high_surrogate = (temp / 0x400) + 0xD800;
			wchar_t low_surrogate = (temp % 0x400) + 0xDC00;

			result[0] = low_surrogate;
			result[1] = high_surrogate;

			return result;
		}
		char32_t wchar_to_char32(wchar_t high_surrogate, wchar_t low_surrogate)
		{
			char32_t temp_high = (high_surrogate - 0xD800) * 0x400;
			char32_t temp_low = low_surrogate - 0xDC00;

			return temp_high + temp_low + 0x10000;
		}
	}

	interpreter::interpreter(std::FILE* input_stream, std::FILE* output_stream)
		: input_stream_(input_stream), output_stream_(output_stream)
	{
		initialize_();
	}
	interpreter::interpreter(app::version version, std::FILE* input_stream, std::FILE* output_stream)
		: version_(version), input_stream_(input_stream), output_stream_(output_stream)
	{
		initialize_();
	}

	void interpreter::run(const raw_code& code)
	{
		app::code splited_code = code;

		std::size_t x = 0;
		std::size_t y = 0;

		std::size_t direction = 0; // 0: 왼쪽, 1: 오른쪽, 2: 위, 3: 아래

		char32_t last_jungsung = 0;

		while (true)
		{
			char32_t command = splited_code.command(x, y);
			
			if (is_complete_hangul(command))
			{
				char32_t chosung = get_chosung(command);
				char32_t jungsung = get_jungsung(command);
				char32_t jongsung = get_jongsung(command);
				
				char32_t jungsung_org = get_jungsung_original(jungsung);
				bool is_added_additional_data = app::is_added_additional_data(jungsung);

				last_jungsung = jungsung;

				switch (chosung)
				{
				case U'ㄷ':
					add_(jongsung, is_added_additional_data);
					break;
				case U'ㄸ':
					mul_(jongsung, is_added_additional_data);
					break;
				case U'ㅌ':
					sub_(jongsung, is_added_additional_data);
					break;
				case U'ㄴ':
					div_(jongsung, is_added_additional_data);
					break;
				case U'ㄹ':
					mod_(jongsung, is_added_additional_data);
					break;



				case U'ㅁ':
					pop_(jongsung, is_added_additional_data);
					break;
				case U'ㅂ':
					push_(jongsung, is_added_additional_data);
					break;
				case U'ㅃ':
					copy_(jongsung, is_added_additional_data);
					break;
				case U'ㅍ':
					swap_(jongsung, is_added_additional_data);
					break;



				case U'ㅅ':
					change_storage_(jongsung, is_added_additional_data);
					break;
				case U'ㅆ':
					move_(jongsung, is_added_additional_data);
					break;



				case U'ㅇ':
					break;
				case U'ㅎ':
					return;
				}

				switch (jungsung_org)
				{
				case U'ㅏ':
					go_right_(x, y, 1, direction, splited_code);
					break;

				case U'ㅑ':
					go_right_(x, y, 2, direction, splited_code);
					break;

				case U'ㅓ':
					go_left_(x, y, 1, direction, splited_code);
					break;

				case U'ㅕ':
					go_left_(x, y, 2, direction, splited_code);
					break;

				case U'ㅗ':
					go_up_(x, y, 1, direction, splited_code);
					break;

				case U'ㅛ':
					go_up_(x, y, 2, direction, splited_code);
					break;

				case U'ㅜ':
					go_down_(x, y, 1, direction, splited_code);
					break;

				case U'ㅠ':
					go_down_(x, y, 2, direction, splited_code);
					break;

				case U'ㅡ':
				{
					if (direction == 0)
					{
						go_right_(x, y, 1, direction, splited_code);
					}
					else if (direction == 1)
					{
						go_left_(x, y, 1, direction, splited_code);
					}
					else if (direction == 2)
					{
						go_down_(x, y, 1, direction, splited_code);
					}
					else
					{
						go_up_(x, y, 1, direction, splited_code);
					}

					break;
				}

				case U'ㅣ':
				{
					if (direction == 0)
					{
						go_left_(x, y, 1, direction, splited_code);
					}
					else if (direction == 1)
					{
						go_right_(x, y, 1, direction, splited_code);
					}
					else if (direction == 2)
					{
						go_up_(x, y, 1, direction, splited_code);
					}
					else
					{
						go_down_(x, y, 1, direction, splited_code);
					}

					break;
				}
				
				case U'ㅢ':
				{
					if (direction == 0)
					{
						go_left_(x, y, 1, direction, splited_code);
					}
					else if (direction == 1)
					{
						go_right_(x, y, 1, direction, splited_code);
					}
					else if (direction == 2)
					{
						go_down_(x, y, 1, direction, splited_code);
					}
					else
					{
						go_up_(x, y, 1, direction, splited_code);
					}

					break;
				}
				}
			}
		}
	}

	const app::storage* interpreter::storage(std::size_t index) const
	{
		return storages_[index][storage_indexs_[index]];
	}
	std::size_t interpreter::storage_index(std::size_t index) const
	{
		return storage_indexs_[index];
	}

	void interpreter::initialize_()
	{
		for (std::size_t i = 0; i < 26; ++i)
		{
			std::vector<app::storage*> storages;
			storages.push_back(new list());

			storages_.push_back(storages);
		}

		std::vector<app::storage*> queues;
		queues.push_back(new queue());
		storages_.insert(storages_.begin() + 21, queues);

		std::vector<app::storage*> passages;
		storages_.push_back(passages);

		for (std::size_t i = 0; i < 28; ++i)
		{
			storage_indexs_.push_back(0);
		}
	}

	void interpreter::add_(char32_t jongsung, bool is_added_additional_data)
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
		}
	}
	void interpreter::mul_(char32_t jongsung, bool is_added_additional_data)
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
		}
	}
	void interpreter::sub_(char32_t jongsung, bool is_added_additional_data)
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
		}
	}
	void interpreter::div_(char32_t jongsung, bool is_added_additional_data)
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
		}
	}
	void interpreter::mod_(char32_t jongsung, bool is_added_additional_data)
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
		}
	}

	void interpreter::pop_(char32_t jongsung, bool is_added_additional_data)
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
				}
				else if (value->index() == 1) // 문자일 경우
				{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
					_setmode(_fileno(output_stream_), _O_TEXT);
#endif
					std::fprintf(output_stream_, "%lld", static_cast<std::int64_t>(std::get<1>(*value)));
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
				}
				else if (value->index() == 1) // 문자일 경우
				{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
					_setmode(_fileno(output_stream_), _O_TEXT);
#endif
					std::fprintf(output_stream_, "%f", static_cast<double>(std::get<1>(*value)));
				}
			}
			else if (jongsung == U'ㅎ' && !is_added_additional_data) // 문자 출력
			{
				if (value->index() == 0) // 숫자일 경우
				{
					if constexpr (sizeof(wchar_t) == sizeof(char32_t))
					{
						std::fwprintf(output_stream_, L"%lc", static_cast<wchar_t>(std::get<0>(*value).integer()));
					}
					else
					{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
						_setmode(_fileno(output_stream_), _O_U16TEXT);
#endif
						std::wstring converted = char32_to_wchar(static_cast<char32_t>(std::get<0>(*value).integer()));
						std::fwprintf(output_stream_, L"%ls", converted.c_str());
					}
				}
				else if (value->index() == 1) // 문자일 경우
				{
					if constexpr (sizeof(wchar_t) == sizeof(char32_t))
					{
						std::fwprintf(output_stream_, L"%c", std::get<1>(*value));
					}
					else
					{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
						_setmode(_fileno(output_stream_), _O_U16TEXT);
#endif
						std::wstring converted = char32_to_wchar(std::get<1>(*value));
						std::fwprintf(output_stream_, L"%ls", converted.c_str());
					}
				}
			}
			else if (jongsung == U'ㅎ' && is_added_additional_data) // 문자열 출력
			{
				if (value->index() == 1) // 문자일 경우
				{
					if constexpr (sizeof(wchar_t) == sizeof(char32_t))
					{
						std::fwprintf(output_stream_, L"%lc", std::get<1>(*value));
					}
					else
					{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
						_setmode(_fileno(output_stream_), _O_U16TEXT);
#endif
						std::wstring converted = char32_to_wchar(std::get<1>(*value));
						std::fwprintf(output_stream_, L"%ls", converted.c_str());
					}
				}
				else if (value->index() == 2) // 문자열일 경우
				{
					if constexpr (sizeof(wchar_t) == sizeof(char32_t))
					{
						raw_code print_value = std::get<2>(*value);
						std::fwprintf(output_stream_, L"%ls", print_value.c_str());
					}
					else
					{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
						_setmode(_fileno(output_stream_), _O_U16TEXT);
#endif
						raw_code convert_value = std::get<2>(*value);
						std::wstring converted;

						for (char32_t c : convert_value)
						{
							converted += char32_to_wchar(c);
						}

						std::fwprintf(output_stream_, L"%ls", converted.c_str());
					}
				}
			}
			else if (jongsung == 0 && !is_added_additional_data)
			{
				storage_()->push(value);
				value = nullptr;

				storage_()->push(new element(static_cast<double>(storage_()->length())));
			}
			else if (is_added_additional_data)
			{
				storage_()->push(value);
				value = nullptr;
			}

			delete value;
		}
	}
	void interpreter::push_(char32_t jongsung, bool is_added_additional_data)
	{
		if (jongsung == U'ㅇ' && !is_added_additional_data) // 숫자(정수) 입력
		{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
			_setmode(_fileno(input_stream_), _O_TEXT);
#endif
			long long temp;
			std::fscanf(input_stream_, "%lld", &temp);

			storage_()->push(new element(number(temp)));
		}
		else if (jongsung == U'ㅇ' && is_added_additional_data) // 숫자(소수) 입력
		{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
			_setmode(_fileno(input_stream_), _O_TEXT);
#endif
			double temp;
			std::fscanf(input_stream_, "%lf", &temp);

			storage_()->push(new element(number(temp)));
		}
		else if (jongsung == U'ㅎ' && !is_added_additional_data) // 문자 입력
		{
			if constexpr (sizeof(wchar_t) == sizeof(char32_t))
			{
				char32_t temp;
				std::fwscanf(input_stream_, L"%lc", &temp);

				storage_()->push(new element(temp));
			}
			else
			{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
				_setmode(_fileno(input_stream_), _O_U16TEXT);
#endif
				wchar_t high_surrogate = std::fgetwc(input_stream_);

				if (high_surrogate >= 0xD800 && high_surrogate <= 0xDBFF)
				{
					wchar_t low_surrogate = std::fgetwc(input_stream_);
					
					storage_()->push(new element(wchar_to_char32(high_surrogate, low_surrogate)));
				}
				else
				{
					storage_()->push(new element(static_cast<char32_t>(high_surrogate)));
				}
			}
		}
		else if (jongsung == U'ㅎ' && is_added_additional_data) // 문자열 입력
		{
			if constexpr (sizeof(wchar_t) == sizeof(char32_t))
			{
				raw_code temp;

				while (!std::feof(input_stream_))
				{
					char32_t input = std::fgetwc(input_stream_);

					if (std::iswspace(input))
					{
						break;
					}

					temp += input;
				}

				storage_()->push(new element(temp));
			}
			else
			{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
				_setmode(_fileno(input_stream_), _O_U16TEXT);
#endif
				raw_code temp;

				while (!std::feof(input_stream_))
				{
					wchar_t high_surrogate = std::fgetwc(input_stream_);

					if (high_surrogate >= 0xD800 && high_surrogate <= 0xDBFF)
					{
						wchar_t low_surrogate = std::fgetwc(input_stream_);

						temp += wchar_to_char32(high_surrogate, low_surrogate);
					}
					else
					{
						if (std::iswspace(high_surrogate))
						{
							break;
						}

						temp += high_surrogate;
					}
				}

				storage_()->push(new element(temp));
			}
		}
		else if (jongsung == 0 && !is_added_additional_data)
		{
			storage_()->push(new element(0.0));
		}
		else if (jongsung == 0 && is_added_additional_data)
		{
			// TODO: 클래스, 구조체, 함수
		}
		else
		{
			storage_()->push(new element(number(get_integer_(jongsung, is_added_additional_data))));
		}
	}
	void interpreter::copy_(char32_t jongsung, bool is_added_additional_data)
	{
		if (jongsung == 0)
		{
			element* copyed = storage_()->copy();

			if (is_added_additional_data)
			{
				if (copyed->index() == 0)
				{
					std::int64_t floor = std::get<0>(*copyed).integer();
					*copyed = element(floor);
				}
			}
		}
		else if (jongsung == U'ㄱ')
		{
			element* copyed = storage_()->copy();

			if (copyed->index() == 0)
			{
				if (is_added_additional_data)
				{
					double abs = std::floor(std::abs(std::get<0>(*copyed).decimal()));
					*copyed = element(abs);
				}
				else
				{
					double abs = std::abs(std::get<0>(*copyed).decimal());
					*copyed = element(abs);
				}
			}
		}
		else if (jongsung == U'ㄴ')
		{
			element* copyed = storage_()->copy();

			if (copyed->index() == 0)
			{
				double ceil = std::ceil(std::get<0>(*copyed).decimal());
				*copyed = element(ceil);
			}
		}
		else if (jongsung == U'ㄵ')
		{
			element* copyed = storage_()->copy();

			if (copyed->index() == 0)
			{
				if (is_added_additional_data)
				{
					double pow = std::floor(std::pow(std::get<0>(*copyed).decimal(), 2));
					*copyed = element(pow);
				}
				else
				{
					double pow = std::pow(std::get<0>(*copyed).decimal(), 2);
					*copyed = element(pow);
				}
			}
		}
		else if (jongsung == U'ㄶ')
		{
			element* copyed = storage_()->copy();

			if (copyed->index() == 0)
			{
				if (is_added_additional_data)
				{
					double sqrt = std::floor(std::sqrt(std::get<0>(*copyed).decimal()));
					*copyed = element(sqrt);
				}
				else
				{
					double sqrt = std::sqrt(std::get<0>(*copyed).decimal());
					*copyed = element(sqrt);
				}
			}
		}
		else if (jongsung == U'ㄷ')
		{
			element* copyed = storage_()->copy();

			if (copyed->index() == 0)
			{
				if (is_added_additional_data)
				{
					double exp = std::floor(std::exp(std::get<0>(*copyed).decimal()));
					*copyed = element(exp);
				}
				else
				{
					double exp = std::exp(std::get<0>(*copyed).decimal());
					*copyed = element(exp);
				}
			}
		}
	}
	void interpreter::swap_(char32_t jongsung, bool is_added_additional_data)
	{
		if (jongsung == 0 && !is_added_additional_data)
		{
			element* first = storage_()->pop();
			element* second = storage_()->pop();

			storage_()->push(first);
			storage_()->push(second);
		}
	}
	
	void interpreter::change_storage_(char32_t jongsung, bool is_added_additional_data)
	{
		if (is_added_additional_data)
		{
			return;
		}

		if (jongsung == 0)
		{
			selected_index_ = 0;
		}
		else
		{
			selected_index_ = jongsung - U'ㄱ' + 1;
		}
	}
	void interpreter::move_(char32_t jongsung, bool is_added_additional_data)
	{
		std::size_t selected_index = jongsung == 0 ? 0 : (jongsung - U'ㄱ' + 1);

		element* value = storage_()->pop();
		storages_[selected_index][storage_indexs_[selected_index]]->push(value);

		if (value->index() == 0 && is_added_additional_data)
		{
			number& value_number = std::get<0>(*value);
			value_number.is_integer(true);
		}
	}

	void interpreter::go_left_(std::size_t& x, std::size_t& y, std::size_t move, int direction, app::code& splited_code)
	{
		x -= move;

		if (x == static_cast<std::size_t>(-1))
		{
			x = splited_code.line(y).size() - 2;
		}

		direction = 0;
	}
	void interpreter::go_right_(std::size_t& x, std::size_t& y, std::size_t move, int direction, app::code& splited_code)
	{
		x += move;

		if (x >= splited_code.line(y).size() - 1)
		{
			x = 0;
		}

		direction = 1;
	}
	void interpreter::go_up_(std::size_t&, std::size_t& y, std::size_t move, int direction, app::code& splited_code)
	{
		y -= move;

		if (y == static_cast<std::size_t>(-1))
		{
			y = splited_code.codes().size() - 2;
		}

		direction = 2;
	}
	void interpreter::go_down_(std::size_t&, std::size_t& y, std::size_t move, int direction, app::code& splited_code)
	{
		y += move;

		if (y >= splited_code.codes().size() - 1)
		{
			y = 0;
		}

		direction = 3;
	}
	long long app::interpreter::get_integer_(char32_t jongsung, bool is_added_additional_data)
	{
		long long value;

		switch (jongsung)
		{
		case U'ㄱ':
		case U'ㄴ':
		case U'ㅅ':
			value = 2;
			break;

		case U'ㄷ':
		case U'ㅈ':
		case U'ㅋ':
			value = 3;
			break;

		case U'ㄲ':
		case U'ㄳ':
		case U'ㅁ':
		case U'ㅆ':
		case U'ㅂ':
		case U'ㅊ':
		case U'ㅌ':
		case U'ㅍ':
			value = 4;
			break;

		case U'ㄵ':
		case U'ㄶ':
		case U'ㄹ':
			value = 5;
			break;

		case U'ㅄ':
			value = 6;
			break;

		case U'ㄺ':
		case U'ㄽ':
			value = 7;
			break;

		case U'ㅀ':
			value = 8;
			break;

		case U'ㄻ':
		case U'ㄼ':
		case U'ㄾ':
		case U'ㄿ':
			value = 9;
			break;
		}

		if (is_added_additional_data)
		{
			value = -value;
		}

		return value;
	}

	app::storage* interpreter::storage_()
	{
		return storages_[selected_index_][storage_indexs_[selected_index_]];
	}

	app::version interpreter::version() const noexcept
	{
		return version_;
	}
	const std::vector<std::vector<app::storage*>>& interpreter::storages() const noexcept
	{
		return storages_;
	}
	
	const std::vector<function*>& interpreter::functions() const noexcept
	{
		return functions_;
	}
	const std::vector<std::size_t>& interpreter::storage_indexs() const noexcept
	{
		return storage_indexs_;
	}
	std::size_t interpreter::selected_index() const noexcept
	{
		return selected_index_;
	}

	bool interpreter::is_integer_mode() const noexcept
	{
		return is_integer_mode_;
	}

	std::FILE* interpreter::input_stream()
	{
		return input_stream_;
	}
	std::FILE* interpreter::output_stream()
	{
		return output_stream_;
	}
}