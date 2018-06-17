#include <Aheuiplusplus/debugger.hpp>

#include <algorithm>
#include <exception>
#include <functional>
#include <stdexcept>
#include <string>

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#	include <fcntl.h>
#	include <io.h>
#endif

namespace app
{
	debugger::debugger(std::FILE* output_stream, app::interpreter& interpreter) noexcept
		: output_stream_(output_stream), interpreter_(interpreter)
	{}
	debugger::~debugger()
	{
		disconnect_debugger();
	}

	void debugger::connect_debugger()
	{
		if (interpreter_.debugger_ != this && interpreter_.debugger_ != nullptr)
			throw std::bad_function_call();
		else if (interpreter_.debugger_ == nullptr)
		{
			interpreter_.debugger_ = this;
		}
	}
	void debugger::disconnect_debugger() noexcept
	{
		if (interpreter_.debugger_ == this)
		{
			interpreter_.debugger_ = nullptr;
		}
	}
	bool debugger::is_connceted_debugger() const noexcept
	{
		return interpreter_.debugger_ == this;
	}

	void debugger::dump_storages() const
	{
		dump_storages(0);
	}
	void debugger::dump_storages(std::size_t depth) const
	{
		if (!is_connceted_debugger())
			throw std::bad_function_call();

		for (std::size_t i = 0; i < interpreter_.storages_.size() - 1; ++i) // 통로는 제외
		{
			static constexpr char32_t jongsungs[] = {
				0, U'ㄱ', U'ㄲ', U'ㄳ', U'ㄴ', U'ㄵ', U'ㄶ', U'ㄷ', U'ㄹ', U'ㄺ', U'ㄻ', U'ㄼ', U'ㄽ', U'ㄾ',
				U'ㄿ', U'ㅀ', U'ㅁ', U'ㅂ', U'ㅄ', U'ㅅ', U'ㅆ', U'ㅇ', U'ㅈ', U'ㅊ', U'ㅋ', U'ㅌ', U'ㅍ', U'ㅎ'
			};

			dump_storage(depth, get_complete_hangul(U'ㅇ', U'ㅏ', jongsungs[i]));
		}
	}
	void debugger::dump_storage(char32_t storage) const
	{
		dump_storage(0, storage);
	}
	void debugger::dump_storage(std::size_t depth, char32_t storage) const
	{
		if (!is_connceted_debugger())
			throw std::bad_function_call();
		if (!is_complete_hangul(storage))
			throw std::invalid_argument("인수 storage는 완성된 현대 한글이여야 합니다.");
		if (get_jongsung(storage) == U'ㅎ')
		{
			std::printf("[Debugger] 현재 버전에서는 통로는 지원되지 않습니다.\n\n");
			return;
		}

		std::string tab(depth * 4, ' ');

		static constexpr char32_t jongsungs[] = {
			0, U'ㄱ', U'ㄲ', U'ㄳ', U'ㄴ', U'ㄵ', U'ㄶ', U'ㄷ', U'ㄹ', U'ㄺ', U'ㄻ', U'ㄼ', U'ㄽ', U'ㄾ',
			U'ㄿ', U'ㅀ', U'ㅁ', U'ㅂ', U'ㅄ', U'ㅅ', U'ㅆ', U'ㅇ', U'ㅈ', U'ㅊ', U'ㅋ', U'ㅌ', U'ㅍ', U'ㅎ'
		};

		std::size_t i = 0;

		for (std::size_t j = 0; j < sizeof(jongsungs) / sizeof(char32_t); ++j)
		{
			if (get_jongsung(storage) == jongsungs[j])
			{
				i = j;
				break;
			}
		}
		
		std::fprintf(output_stream_, "%s%lc(선택된 번호: %lld, 번호 개수: %lld):\n", tab.c_str(),
			static_cast<wchar_t>(storage),
			static_cast<long long>(interpreter_.storage_indexs_[i]),
			static_cast<long long>(interpreter_.storages_[i].size()));

		++depth;

		for (std::size_t j = 0; j < interpreter_.storages_[i].size(); ++j)
		{
			tab = std::string(depth * 4, ' ');

			app::storage* storage = interpreter_.storages_[i][j];
			std::size_t virtual_length = storage->length();

			if (storage->type() == storage_type::list)
			{
				virtual_length = reinterpret_cast<list*>(storage)->virtual_length();
			}

			std::fprintf(output_stream_, "%s[%lld](가상 길이: %lld, 실제 길이: %lld):\n", tab.c_str(), static_cast<long long>(j),
				static_cast<long long>(virtual_length), static_cast<long long>(storage->length()));

			app::storage* storage_backup;

			if (storage->type() == storage_type::list)
			{
				storage_backup = new list();

				list* storage_converted = reinterpret_cast<list*>(storage);

				for (std::size_t k = 0; k < storage->length(); ++k)
				{
					reinterpret_cast<list*>(storage_backup)->original().push_back(
						new element(*storage_converted->original()[k]));

					reinterpret_cast<list*>(storage_backup)->virtual_length(storage_converted->length());
				}
			}
			else
			{
				storage_backup = new queue();

				queue* storage_converted = reinterpret_cast<queue*>(storage);

				for (std::size_t k = 0; k < storage->length(); ++k)
				{
					reinterpret_cast<list*>(storage_backup)->original().push_back(
						new element(*storage_converted->original()[k]));
				}
			}

			++depth;

			if (storage_backup->length() == 0)
			{
				tab = std::string(depth * 4, ' ');

				std::fprintf(output_stream_, "%s(비어 있음)\n", tab.c_str());
			}

			for (std::size_t k = 0; k < storage_backup->length(); ++k)
			{
				tab = std::string(depth * 4, ' ');

				std::fprintf(output_stream_, "%s[%lld] = ", tab.c_str(), static_cast<long long>(k));

				element* value = storage_backup->pop();

				switch (value->index())
				{
				case 0:
				{
					if (std::get<0>(*value).is_integer())
					{
						std::fprintf(output_stream_, "number(%lld)\n", std::get<0>(*value).integer());
					}
					else
					{
						std::fprintf(output_stream_, "number(%f)\n", std::get<0>(*value).decimal());
					}
					break;
				}

				case 1:
				{
					if constexpr (sizeof(char32_t) == sizeof(wchar_t))
					{
						std::fprintf(output_stream_, "character(%lc)\n", std::get<1>(*value));
					}
					else
					{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
						_setmode(_fileno(output_stream_), _O_U16TEXT);

						std::wstring converted = char32_to_wchar(std::get<1>(*value));
						std::fwprintf(output_stream_, L"character(%ls)\n", converted.c_str());

						_setmode(_fileno(output_stream_), _O_TEXT);
#endif
					}
					break;
				}

				case 2:
				{
					if constexpr (sizeof(wchar_t) == sizeof(char32_t))
					{
						raw_code print_value = std::get<2>(*value);
						std::fwprintf(output_stream_, L"string(%ls)\n", print_value.c_str());
					}
					else
					{
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
						_setmode(_fileno(output_stream_), _O_U16TEXT);

						raw_code convert_value = std::get<2>(*value);
						std::wstring converted;

						for (char32_t c : convert_value)
						{
							converted += char32_to_wchar(c);
						}

						std::fwprintf(output_stream_, L"string(%ls)\n", converted.c_str());

						_setmode(_fileno(output_stream_), _O_TEXT);
#endif
					}
					break;
				}
				}

				delete value;
			}

			delete storage_backup;

			--depth;
		}

		--depth;
		std::printf("\n");
	}

	void debugger::add_breakpoint(std::size_t x, std::size_t y)
	{
		if (std::find_if(breakpoints_.begin(), breakpoints_.end(), [&x, &y](const std::pair<std::size_t, std::size_t>& breakpoint)
		{
			return breakpoint.first == x && breakpoint.second == y;
		}) == breakpoints_.end())
		{
			breakpoints_.push_back({ x, y });
		}
		else
			throw std::invalid_argument("인수 {x, y}는 이미 등록된 중단점입니다.");
	}
	void debugger::remove_breakpoint(std::size_t x, std::size_t y)
	{
		std::vector<std::pair<std::size_t, std::size_t>>::iterator iterator = std::find_if(breakpoints_.begin(), breakpoints_.end(),
			[&x, &y](const std::pair<std::size_t, std::size_t>& breakpoint)
		{
			return breakpoint.first == x && breakpoint.second == y;
		});

		if (iterator != breakpoints_.end())
		{
			breakpoints_.erase(iterator);
		}
		else
			throw std::invalid_argument("인수 {x, y}는 등록되지 않은 중단점입니다.");
	}
	bool debugger::is_added_breakpoint(std::size_t x, std::size_t y)
	{
		return std::find_if(breakpoints_.begin(), breakpoints_.end(),
			[&x, &y](const std::pair<std::size_t, std::size_t>& breakpoint)
		{
			return breakpoint.first == x && breakpoint.second == y;
		}) != breakpoints_.end();
	}

	long long debugger::run_with_debugging(const raw_code& code)
	{
		std::size_t x;
		std::size_t y;

		std::size_t direction;
		std::size_t move;

		bool is_ignored;
		bool is_reflection;
		bool is_out_of_version;
		char32_t start_of_expression = 0;
		
		try
		{
			if (is_connceted_debugger())
			{
				long long result = interpreter_.run_(code, x, y, direction, move, is_ignored, is_reflection,
					start_of_expression, is_out_of_version);

				if (is_out_of_version)
				{
					std::fprintf(output_stream_, "[Debugger] 실행하고자 하는 코드가 현재 버전을 지원하지 않습니다.\n");
				}

				return result;
			}
			else
			{
				goto throw_bad_function_call;
			}
		}
		catch (const std::exception& exception)
		{
			std::fprintf(output_stream_, "[Debugger] %lld줄 %lld행에서 처리되지 못한 구현체 예외가 발생하였습니다. 개발자에게 보고해 주십시오.\n"
										 "[Debugger] 예외 메세지: %s\n", static_cast<long long>(y), static_cast<long long>(x), exception.what());
		
			return -1;
		}

	throw_bad_function_call:
		throw std::bad_function_call();
	}

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
	bool debugger::is_last_input_utf16() const
	{
		if (!is_connceted_debugger())
			throw std::bad_function_call();

		return interpreter_.is_last_input_utf16_;
	}
	void debugger::is_last_input_utf16(bool new_is_last_input_utf16)
	{
		if (!is_connceted_debugger())
			throw std::bad_function_call();

		interpreter_.is_last_input_utf16_ = new_is_last_input_utf16;
	}
#endif
	bool debugger::is_processed_space() const
	{
		if (!is_connceted_debugger())
			throw std::bad_function_call();

		return interpreter_.is_processed_space_char_;
	}
	void debugger::is_processed_space(bool new_is_proecessed_space)
	{
		if (!is_connceted_debugger())
			throw std::bad_function_call();

		interpreter_.is_processed_space_char_ = new_is_proecessed_space;
	}

	const std::vector<std::pair<std::size_t, std::size_t>>& debugger::breakpoints() const noexcept
	{
		return breakpoints_;
	}
	bool debugger::is_inputed() const noexcept
	{
		return is_inputed_;
	}
	void debugger::is_inputed(bool new_is_inputed) noexcept
	{
		is_inputed_ = new_is_inputed;
	}

	std::FILE* debugger::output_stream() const noexcept
	{
		return output_stream_;
	}
}