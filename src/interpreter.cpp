﻿#include <Aheuiplusplus/interpreter.hpp>

#include <cstddef>
#include <cstdint>

namespace app
{
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

				bool is_ignored = false;

				switch (chosung)
				{
				case U'ㄷ':
					is_ignored = add_(jongsung, is_added_additional_data);
					break;
				case U'ㄸ':
					is_ignored = mul_(jongsung, is_added_additional_data);
					break;
				case U'ㅌ':
					is_ignored = sub_(jongsung, is_added_additional_data);
					break;
				case U'ㄴ':
					is_ignored = div_(jongsung, is_added_additional_data);
					break;
				case U'ㄹ':
					is_ignored = mod_(jongsung, is_added_additional_data);
					break;



				case U'ㅁ':
					is_ignored = pop_(jongsung, is_added_additional_data);
					break;
				case U'ㅂ':
					is_ignored = push_(jongsung, is_added_additional_data);
					break;
				case U'ㅃ':
					is_ignored = copy_(jongsung, is_added_additional_data);
					break;
				case U'ㅍ':
					is_ignored = swap_(jongsung, is_added_additional_data);
					break;



				case U'ㅅ':
					is_ignored = change_storage_(jongsung, is_added_additional_data);
					break;
				case U'ㅆ':
					is_ignored = move_(jongsung, is_added_additional_data);
					break;



				case U'ㅇ':
					break;
				case U'ㅎ':
					return;
				}

				if (!is_ignored)
				{
					last_jungsung = jungsung_org;
				}

				switch (last_jungsung)
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
		long long value = 0;

		switch (jongsung)
		{
		case 0:
			value = 0;
			break;

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