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
	namespace
	{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
		std::wstring read_wchar(std::FILE* input_stream)
		{
			wchar_t high_surrogate = std::fgetwc(input_stream);

			if (high_surrogate >= 0xD800 && high_surrogate <= 0xDBFF)
			{
				wchar_t low_surrogate = std::fgetwc(input_stream);

				return { high_surrogate, low_surrogate };
			}
			else
			{
				return { high_surrogate };
			}
		}
		void write_wchar(std::FILE* output_stream, const std::wstring& character)
		{
			for (std::size_t i = 0; i < character.length(); ++i)
			{
				std::fputwc(character[i], output_stream);
			}
		}
#else
		std::string read_u8char(std::FILE* input_stream)
		{
			std::string result;

			unsigned char first = static_cast<unsigned char>(std::fgetc(input_stream));
			int length = app::u8char_length(first);

			if (length == 1)
			{
				result.push_back(first);
			}
			else if (length == 2)
			{
				unsigned char second = std::fgetc(input_stream);

				result.push_back(app::u8char_to_char32(first, second));
			}
			else if (length == 3)
			{
				unsigned char second = std::fgetc(input_stream);
				unsigned char third = std::fgetc(input_stream);

				result.push_back(app::u8char_to_char32(first, second, third));
			}
			else
			{
				unsigned char second = std::fgetc(input_stream);
				unsigned char third = std::fgetc(input_stream);
				unsigned char fourth = std::fgetc(input_stream);

				result.push_back(app::u8char_to_char32(first, second, third, fourth));
			}

			return result;
		}
		void write_u8char(std::FILE* output_stream, const std::string& character)
		{
			for (std::size_t i = 0; i < character.length(); ++i)
			{
				std::fputc(character[i], output_stream);
			}
		}
#endif

		char32_t read_char(std::FILE* input_stream)
		{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
			_setmode(_fileno(input_stream), _O_U16TEXT);

			wchar_t high_surrogate = std::fgetwc(input_stream);

			if (wchar_length(high_surrogate) == 1)
			{
				return high_surrogate;
			}
			else
			{
				wchar_t low_surroagte = std::fgetwc(input_stream);

				return wchar_to_char32(high_surrogate, low_surroagte);
			}
#else
			std::string input = read_u8char(input_stream);

			if (input.length() == 1)
			{
				return u8char_to_char32(input[0]);
			}
			else if (input.length() == 2)
			{
				return u8char_to_char32(input[0], input[1]);
			}
			else if (input.length() == 3)
			{
				return u8char_to_char32(input[0], input[1], input[2]);
			}
			else
			{
				return u8char_to_char32(input[0], input[1], input[2], input[3]);
			}
#endif
		}
		void write_char(std::FILE* output_stream, char32_t character)
		{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
			_setmode(_fileno(output_stream), _O_U16TEXT);

			std::wstring utf16 = char32_to_wchar(character);

			write_wchar(output_stream, utf16);

			_setmode(_fileno(output_stream), _O_TEXT);
#else
			std::string utf8 = char32_to_u8char(character);

			write_u8char(output_stream, utf8);
#endif
		}
	}

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
				}
			}
			else if (jongsung == U'ㅎ' && !is_added_additional_data) // 문자 출력
			{
				if (value->index() == 0) // 숫자일 경우
				{
					write_char(output_stream_, std::get<0>(*value).integer());
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
					write_char(output_stream_, std::get<0>(*value).integer());
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

			if (!is_processed_space_char_)
			{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
				if (is_last_input_utf16_)
				{
					std::fgetwc(input_stream_);
				}
				else
				{
					std::fgetc(input_stream_);
				}
#else
				read_char(input_stream_);
#endif
			}

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
			_setmode(_fileno(input_stream_), _O_TEXT);
#endif

			long long temp;
			std::fscanf(input_stream_, "%lld", &temp);

			storage_()->push(new element(number(temp)));

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
			is_last_input_utf16_ = false;
#endif
			is_processed_space_char_ = false;

			if (debugger_ != nullptr)
			{
				debugger_->is_inputed_ = true;
			}

			return false;
		}
		else if (jongsung == U'ㅇ' && is_added_additional_data) // 숫자(소수) 입력
		{
			if (input_stream_ != stdin && std::feof(input_stream_))
			{
				storage_()->push(new element(number(0.0)));
				return false;
			}

			if (!is_processed_space_char_)
			{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
				if (is_last_input_utf16_)
				{
					std::fgetwc(input_stream_);
				}
				else
				{
					std::fgetc(input_stream_);
				}
#else
				read_char(input_stream_);
#endif
			}

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
			_setmode(_fileno(input_stream_), _O_TEXT);
#endif

			double temp;
			std::fscanf(input_stream_, "%lf", &temp);

			storage_()->push(new element(number(temp)));

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
			is_last_input_utf16_ = false;
#endif
			is_processed_space_char_ = false;

			if (debugger_ != nullptr)
			{
				debugger_->is_inputed_ = true;
			}

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

			if (!is_processed_space_char_)
			{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
				if (is_last_input_utf16_)
				{
					std::fgetwc(input_stream_);
				}
				else
				{
					std::fgetc(input_stream_);
				}
#else
				read_char(input_stream_);
#endif
			}

			char32_t input = read_char(input_stream_);

			if (input < 128)
			{
				if (std::isspace(static_cast<unsigned char>(input)))
				{
					goto input_char;
				}
			}

			storage_()->push(new element(input));

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
			is_last_input_utf16_ = true;
#endif
			is_processed_space_char_ = false;

			if (debugger_ != nullptr)
			{
				debugger_->is_inputed_ = true;
			}

			return false;
		}
		else if (jongsung == U'ㅎ' && is_added_additional_data) // 문자열 입력
		{
			if (input_stream_ != stdin && std::feof(input_stream_))
			{
				storage_()->push(new element(0));
				return false;
			}

			if (!is_processed_space_char_)
			{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
				if (is_last_input_utf16_)
				{
					std::fgetwc(input_stream_);
				}
				else
				{
					std::fgetc(input_stream_);
				}
#else
				read_char(input_stream_);
#endif
			}

			raw_code input;

			while (true)
			{
				char32_t c = read_char(input_stream_);

				if (c < 128)
				{
					if (std::isspace(static_cast<unsigned char>(c)))
					{
						break;
					}
				}

				input += c;
			}

			storage_()->push(new element(input));

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
			is_last_input_utf16_ = true;
#endif
			is_processed_space_char_ = true;

			if (debugger_ != nullptr)
			{
				debugger_->is_inputed_ = false;
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