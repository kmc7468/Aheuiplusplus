#include <Aheuiplusplus/interpreter.hpp>

#include <cmath>
#include <cstdio>
#include <cwchar>
#include <cwctype>

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
				}
				else if (value->index() == 1) // 문자일 경우
				{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
					_setmode(_fileno(output_stream_), _O_TEXT);
#endif
					std::fprintf(output_stream_, "%lld", static_cast<long long>(std::get<1>(*value)));
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
				if (value->index() == 0)
				{
					if constexpr (sizeof(wchar_t) == sizeof(char32_t)) // 숫자일 경우
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
			else if (is_added_additional_data)
			{
				storage_()->push(value);
				value = nullptr;
			}
			
			delete value;
		}

		return false;
	}
	bool interpreter::push_(char32_t jongsung, bool is_added_additional_data)
	{
		if (jongsung == U'ㅇ' && !is_added_additional_data) // 숫자(정수) 입력
		{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
			_setmode(_fileno(input_stream_), _O_TEXT);
#endif
			long long temp;
			std::fscanf(input_stream_, "%lld", &temp);

			storage_()->push(new element(number(temp)));

			return false;
		}
		else if (jongsung == U'ㅇ' && is_added_additional_data) // 숫자(소수) 입력
		{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
			_setmode(_fileno(input_stream_), _O_TEXT);
#endif
			double temp;
			std::fscanf(input_stream_, "%lf", &temp);

			storage_()->push(new element(number(temp)));

			return false;
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

			return false;
		}
		else if (jongsung == U'ㅎ' && is_added_additional_data) // 문자열 입력
		{
			if constexpr (sizeof(wchar_t) == sizeof(char32_t))
			{
				raw_code temp;

				while (!std::feof(input_stream_))
				{
					char32_t input = std::fgetwc(input_stream_);

					if (input < 65'535 && std::iswspace(static_cast<std::wint_t>(input)))
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
			element* first = storage_()->pop();
			element* second = storage_()->pop();

			storage_()->push(first);
			storage_()->push(second);

			return false;
		}

		return true;
	}
}