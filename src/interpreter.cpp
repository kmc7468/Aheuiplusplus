#include <Aheuiplusplus/interpreter.hpp>

#include <Aheuiplusplus/debugger.hpp>

#include <cstddef>
#include <cstdint>
#include <map>

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#	include <fcntl.h>
#	include <io.h>
#endif

namespace app
{
	interpreter::interpreter(std::FILE* input_stream, std::FILE* output_stream)
		: input_stream_(input_stream), output_stream_(output_stream)
	{
		initialize_();

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
		_setmode(_fileno(input_stream_), _O_U16TEXT);
		_setmode(_fileno(output_stream_), _O_U16TEXT);
#endif
	}
	interpreter::interpreter(app::version version, std::FILE* input_stream, std::FILE* output_stream)
		: version_(version), input_stream_(input_stream), output_stream_(output_stream)
	{
		initialize_();

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
		_setmode(_fileno(input_stream_), _O_U16TEXT);
		_setmode(_fileno(output_stream_), _O_U16TEXT);
#endif
	}
	interpreter::~interpreter()
	{
		delete_storage_();

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
		_setmode(_fileno(input_stream_), _O_TEXT);
		_setmode(_fileno(output_stream_), _O_TEXT);
#endif
	}

	long long interpreter::run(const raw_code& code)
	{
		return run(code, false, false);
	}
	long long interpreter::run(const raw_code& code, bool command_aheui, bool command_loud_mode)
	{
		std::size_t x;
		std::size_t y;

		std::size_t direction;
		std::size_t move;

		bool is_ignored;
		bool is_reflection;
		bool is_out_of_version;

		char32_t start_of_expression;

		return run_(code, command_aheui, command_loud_mode,
			x, y, direction, move, is_ignored, is_reflection, start_of_expression,
			is_out_of_version);
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

	long long interpreter::run_(const raw_code& code, bool command_aheui, bool command_loud_mode,
		std::size_t& x, std::size_t& y, std::size_t& direction, std::size_t& move, bool& is_ignored, bool& is_reflection, char32_t& start_of_expression,
		bool& is_out_of_version)
	{
		bool backup_is_compatible_with_aheui_ = is_compatible_with_aheui_;

		if (command_aheui)
		{
			is_compatible_with_aheui_ = true;
		}
		is_loud_mode_ = command_loud_mode;

		app::code splited_code = code;

		x = 0;
		y = 0;
		direction = 3; // 0: 왼쪽, 1: 오른쪽, 2: 위, 3: 아래
		move = 1;
		is_ignored = false;
		is_reflection = false;
		start_of_expression = 0;
		is_out_of_version = false;

		std::size_t processed_command_without_expression = 0;

		char32_t previous_command = 0;
		char32_t command = 0;

		while (true)
		{
			if (start_of_expression == 0)
			{
				++processed_command_without_expression;
			}

			previous_command = command;

			if (y < splited_code.codes().size() &&
				x < splited_code.line(y).size() - 1)
			{
				command = splited_code.command(x, y);
			}
			else
			{
				command = U' ';
			}

			if (is_complete_hangul(command))
			{
				std::size_t new_direction;
				std::size_t new_move;

				char32_t chosung = get_chosung(command);
				char32_t jungsung = get_jungsung(command);
				char32_t jongsung = get_jongsung(command);

				char32_t jungsung_org = get_jungsung_original(jungsung);
				bool is_added_additional_data = app::is_added_additional_data(jungsung);

				switch (jungsung_org)
				{
				case U'ㅏ':
					new_direction = 1;
					new_move = 1;
					break;

				case U'ㅑ':
					new_direction = 1;
					new_move = 2;
					break;

				case U'ㅓ':
					new_direction = 0;
					new_move = 1;
					break;

				case U'ㅕ':
					new_direction = 0;
					new_move = 2;
					break;

				case U'ㅗ':
					new_direction = 2;
					new_move = 1;
					break;

				case U'ㅛ':
					new_direction = 2;
					new_move = 2;
					break;

				case U'ㅜ':
					new_direction = 3;
					new_move = 1;
					break;

				case U'ㅠ':
					new_direction = 3;
					new_move = 2;
					break;

				case U'ㅡ':
				{
					if (direction == 0 || direction == 1)
					{
						new_direction = direction;
					}
					else if (direction == 2)
					{
						new_direction = 3;
					}
					else
					{
						new_direction = 2;
					}

					new_move = move;

					break;
				}

				case U'ㅣ':
				{
					if (direction == 0)
					{
						new_direction = 1;
					}
					else if (direction == 1)
					{
						new_direction = 0;
					}
					else
					{
						new_direction = direction;
					}

					new_move = move;

					break;
				}

				case U'ㅢ':
				{
					if (direction == 0)
					{
						new_direction = 1;
					}
					else if (direction == 1)
					{
						new_direction = 0;
					}
					else if (direction == 2)
					{
						new_direction = 3;
					}
					else
					{
						new_direction = 2;
					}

					new_move = move;

					break;
				}

				default:
					new_direction = direction;
					new_move = move;
					break;
				}

				if (is_added_additional_data && is_compatible_with_aheui_)
				{
					if (!(chosung == U'ㄲ' &&
						(jongsung == U'ㅁ' || jongsung == U'ㅂ' || jongsung == U'ㅄ' ||
							jongsung == U'ㅅ'))) // 예외 명령
					{
						new_direction = direction;
						new_move = move;

						is_added_additional_data = false;
					}

					if (command_aheui)
					{
						if ((chosung == U'ㄲ' &&
							(jongsung == U'ㅁ' || jongsung == U'ㅂ' || jongsung == U'ㅄ' ||
								jongsung == U'ㅅ')))
						{
							new_direction = direction;
							new_move = move;

							is_added_additional_data = false;
						}
					}
				}

				if (is_compatible_with_aheui_)
				{
					if (chosung == U'ㅃ' || chosung == U'ㅍ' || chosung == U'ㅈ' ||
						chosung == U'ㅊ')
					{
						jongsung = 0;
					}
					
					if (chosung == U'ㄱ' || chosung == U'ㅋ' || chosung == U'ㅉ' ||
						(chosung == U'ㄲ' && !(jongsung == U'ㅁ' || jongsung == U'ㅂ' ||
							jongsung == U'ㅄ' || jongsung == U'ㅅ')))
					{
						chosung = U'ㅇ';
					}

					if (command_aheui)
					{
						if ((chosung == U'ㄲ' &&
							(jongsung == U'ㅁ' || jongsung == U'ㅂ' || jongsung == U'ㅄ' ||
								jongsung == U'ㅅ')))
						{
							chosung = U'ㅇ';
						}
					}
				}

				if (chosung == U'ㄱ' || chosung == U'ㅋ') // 예약
				{
					is_ignored = true;

					goto reserved;
				}

				switch (chosung)
				{
				case U'ㄲ':
					is_ignored = type_and_mode_(jongsung, is_added_additional_data, start_of_expression,
						processed_command_without_expression, is_out_of_version);
					break;

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
				case U'ㅈ':
					is_ignored = compare_(jongsung, is_added_additional_data);
					break;
				case U'ㅊ':
					is_ignored = is_zero_(jongsung, is_added_additional_data, is_reflection);
					break;
				case U'ㅉ':
					is_ignored = expand_storage_(jongsung, is_added_additional_data);
					break;



				case U'ㅇ':
					break;
				case U'ㅎ':
				{
					if (start_of_expression != 0)
					{
						storage_backup_and_restore_();
					}

					return exit_();
				}
				}

				if (is_out_of_version)
				{
					return 0;
				}

				if (is_ignored && is_compatible_with_aheui_)
				{
					is_reflection = true;
				}
				else if (is_ignored && !is_compatible_with_aheui_)
				{
				reserved:
					new_direction = direction;
					new_move = move;
				}

				if (is_reflection)
				{
					switch (new_direction)
					{
					case 0:
						new_direction = 1;
						break;

					case 1:
						new_direction = 0;
						break;

					case 2:
						new_direction = 3;
						break;

					case 3:
						new_direction = 2;
						break;
					}

					is_reflection = false;
				}

				go_(x, y, new_move, new_direction, splited_code);
				direction = new_direction;
				move = new_move;
			}
			else
			{
				go_(x, y, move, direction, splited_code);
			}
		}

		if (command_aheui)
		{
			is_compatible_with_aheui_ = backup_is_compatible_with_aheui_;
		}
	}

	long long interpreter::exit_()
	{
		if (storage_()->length() == 0)
		{
			return 0;
		}
		else
		{
			element* value = nullptr;

			if (storage_()->type() == storage_type::list)
			{
				app::list* list = reinterpret_cast<app::list*>(storage_());

				for (std::size_t i = list->virtual_length() - 1; ; --i)
				{
					value = list->original()[i];
					
					switch (value->index())
					{
					case 0:
						return std::get<0>(*value).integer();

					case 1:
						return std::get<1>(*value);
					}

					if (i == 0)
					{
						break;
					}
				}
			}
			else
			{
				app::queue* queue = reinterpret_cast<app::queue*>(storage_());

				for (std::size_t i = queue->length() - 1; ; --i)
				{
					value = queue->original()[i];

					switch (value->index())
					{
					case 0:
						return std::get<0>(*value).integer();

					case 1:
						return std::get<1>(*value);
					}

					if (i == 0)
					{
						break;
					}
				}
			}

			return 0;
		}
	}

	void interpreter::go_(std::size_t& x, std::size_t& y, std::size_t move, std::size_t direction, app::code& splited_code)
	{
		switch (direction)
		{
		case 0:
		{
			if (x == 0 || (x == 1 && move == 2))
			{
				x = splited_code.line(y).size() - 2;
			}
			else
			{
				x -= move;
			}
			
			break;
		}

		case 1:
		{
			if (x == splited_code.line(y).size() - 2 || (x == splited_code.line(y).size() - 3 && move == 2))
			{
				x = 0;
			}
			else
			{
				x += move;
			}

			break;
		}

		case 2:
		{
			if (y == 0 || (y == 1 && move == 2))
			{
				y = splited_code.codes().size() - 1;
			}
			else
			{
				y -= move;
			}

			break;
		}

		case 3:
		{
			if (y == splited_code.codes().size() - 1 || (y == splited_code.codes().size() - 2 && move == 2))
			{
				y = 0;
			}
			else
			{
				y += move;
			}

			break;
		}
		}
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
	void interpreter::type_casting_arithmetic_(element* left_operand, element* right_operand,
		std::shared_ptr<element>& left_operand_out, std::shared_ptr<element>& right_operand_out) const
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
					if (is_integer_mode_)
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
					if (is_integer_mode_)
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
	void interpreter::type_casting_compare_(element* left_operand, element* right_operand,
		std::shared_ptr<element>& left_operand_out, std::shared_ptr<element>& right_operand_out) const
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
					if (std::get<0>(*left_operand).is_integer())
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
				}
				break;
			}

			case 1:
			{
				switch (right_operand->index())
				{
				case 0: // 문자, 숫자 = 숫자
				{
					if (std::get<0>(*right_operand).is_integer())
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
				}
				break;
			}
			}
		}
	}

	void interpreter::storage_backup_and_restore_()
	{
		static std::map<app::interpreter*, std::vector<std::vector<app::storage*>>> backup_storages;
		static std::map<app::interpreter*, std::vector<std::size_t>> backup_storage_indexs;
		static std::map<app::interpreter*, std::size_t> backup_selected_index;

		static std::map<app::interpreter*, bool> backup_is_integer_mode;
		static std::map<app::interpreter*, bool> backup_is_compatible_with_aheui;

		if (backup_is_integer_mode.find(this) == backup_is_integer_mode.end()) // 백업
		{
			// 저장공간 백업
			backup_storages.insert(
				std::make_pair<app::interpreter*, std::vector<std::vector<app::storage*>>>(
					this, std::vector<std::vector<app::storage*>>{}));

			for (std::size_t i = 0; i < storages_.size(); ++i)
			{
				backup_storages[this].push_back({});

				for (std::size_t j = 0; j < storages_[i].size(); ++j)
				{
					app::storage* old_storage = storages_[i][j];

					if (old_storage->type() == storage_type::list)
					{
						backup_storages[this][i].push_back(new list());

						list* old_storage_converted = reinterpret_cast<list*>(old_storage);

						for (std::size_t k = 0; k < old_storage_converted->original().size(); ++k)
						{
							backup_storages[this][i][j]->push(
								new element(*old_storage_converted->original()[k]));
						}
					}
					else
					{
						backup_storages[this][i].push_back(new queue());

						queue* old_storage_converted = reinterpret_cast<queue*>(old_storage);

						for (std::size_t k = 0; k < old_storage_converted->original().size(); ++k)
						{
							backup_storages[this][i][j]->push(
								new element(*old_storage_converted->pop()));
						}
					}
				}
			}

			// 저장공간 번호 백업
			backup_storage_indexs.insert(
				std::make_pair<app::interpreter*, std::vector<std::size_t>>(
					this, std::vector<std::size_t>{}));

			for (std::size_t index : storage_indexs_)
			{
				backup_storage_indexs[this].push_back(index);
			}

			// 선택된 저장공간 백업
			backup_selected_index.insert(
				std::make_pair<app::interpreter*, std::size_t>(
					this, std::size_t(selected_index_)));

			// 연산 모드 백업
			backup_is_integer_mode.insert(
				std::make_pair<app::interpreter*, bool>(
					this, bool(is_integer_mode_)));

			// 아희 호환 모드 백업
			backup_is_compatible_with_aheui.insert(std::make_pair<app::interpreter*, bool>(
				this, bool(is_compatible_with_aheui_)));
		}
		else // 복원
		{
			delete_storage_();
			
			storages_ = backup_storages[this];
			storage_indexs_ = backup_storage_indexs[this];
			selected_index_ = backup_selected_index[this];
			
			is_integer_mode_ = backup_is_integer_mode[this];
			is_compatible_with_aheui_ = backup_is_compatible_with_aheui[this];

			// 맵에서 삭제
			backup_storages.erase(backup_storages.find(this));
			backup_storage_indexs.erase(backup_storage_indexs.find(this));
			backup_selected_index.erase(backup_selected_index.find(this));

			backup_is_integer_mode.erase(backup_is_integer_mode.find(this));
			backup_is_compatible_with_aheui.erase(backup_is_compatible_with_aheui.find(this));
		}
	}
	void interpreter::delete_storage_()
	{
		for (std::vector<app::storage*>& storages : storages_)
		{
			for (app::storage* storage : storages)
			{
				element* value;

				while (value = storage->pop())
				{
					delete value;
				}

				delete storage;
			}
		}
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
	bool app::interpreter::is_compatible_with_aheui() const noexcept
	{
		return is_compatible_with_aheui_;
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