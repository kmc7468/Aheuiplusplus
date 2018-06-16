#include <Aheuiplusplus/interpreter.hpp>

namespace app
{
	bool interpreter::change_storage_(char32_t jongsung, bool is_added_additional_data)
	{
		if (is_added_additional_data)
		{
			return true;
		}

		if (jongsung == 0)
		{
			selected_index_ = 0;
		}
		else
		{
			static constexpr char32_t jongsungs[] = {
				0, U'ㄱ', U'ㄲ', U'ㄳ', U'ㄴ', U'ㄵ', U'ㄶ', U'ㄷ', U'ㄹ', U'ㄺ', U'ㄻ', U'ㄼ', U'ㄽ', U'ㄾ',
				U'ㄿ', U'ㅀ', U'ㅁ', U'ㅂ', U'ㅄ', U'ㅅ', U'ㅆ', U'ㅇ', U'ㅈ', U'ㅊ', U'ㅋ', U'ㅌ', U'ㅍ', U'ㅎ'
			};

			for (std::size_t i = 1; i < sizeof(jongsungs) / sizeof(char32_t); ++i)
			{
				if (jongsungs[i] == jongsung)
				{
					selected_index_ = i;
				}
			}
		}

		return false;
	}
	bool interpreter::move_(char32_t jongsung, bool is_added_additional_data)
	{
		std::size_t selected_index;

		static constexpr char32_t jongsungs[] = {
			0, U'ㄱ', U'ㄲ', U'ㄳ', U'ㄴ', U'ㄵ', U'ㄶ', U'ㄷ', U'ㄹ', U'ㄺ', U'ㄻ', U'ㄼ', U'ㄽ', U'ㄾ',
			U'ㄿ', U'ㅀ', U'ㅁ', U'ㅂ', U'ㅄ', U'ㅅ', U'ㅆ', U'ㅇ', U'ㅈ', U'ㅊ', U'ㅋ', U'ㅌ', U'ㅍ', U'ㅎ'
		};

		for (std::size_t i = 0; i < sizeof(jongsungs) / sizeof(char32_t); ++i)
		{
			if (jongsungs[i] == jongsung)
			{
				selected_index = i;
			}
		}

		element* value = storage_()->pop();
		
		if (value == nullptr)
		{
			return true;
		}

		storages_[selected_index][storage_indexs_[selected_index]]->move(value);

		if (value->index() == 0 && is_added_additional_data)
		{
			number& value_number = std::get<0>(*value);
			value_number.is_integer(true);
		}

		return false;
	}
	bool interpreter::compare_(char32_t jongsung, bool is_added_additional_data)
	{
		if (jongsung == 0)
		{
			element* left_operand = storage_()->pop();
			element* right_operand = storage_()->pop();

			if (right_operand == nullptr)
			{
				return true;
			}
			else if (left_operand == nullptr)
			{
				storage_()->push(right_operand);
				return true;
			}

			if (left_operand->index() >= 2 || right_operand->index() >= 2)
			{
				storage_()->push(new element(number(0ll)));

				return false;
			}

			std::shared_ptr<element> right_operand_converted;
			std::shared_ptr<element> left_operand_converted;
			type_casting_arithmetic_(left_operand, right_operand, left_operand_converted, right_operand_converted);

			if (is_added_additional_data)
			{
				long long right_operand_integer = std::get<0>(*right_operand_converted).integer();
				long long left_operand_integer = std::get<0>(*left_operand_converted).integer();

				bool result = left_operand_integer <= right_operand_integer;

				storage_()->push(new element(number(static_cast<long long>(result))));
			}
			else
			{
				double right_operand_integer = std::get<0>(*right_operand_converted).decimal();
				double left_operand_integer = std::get<0>(*left_operand_converted).decimal();

				bool result = left_operand_integer <= right_operand_integer;

				storage_()->push(new element(number(static_cast<long long>(result))));
			}

			delete left_operand;
			delete right_operand;

			return false;
		}

		return true;
	}
	bool interpreter::is_zero_(char32_t jongsung, bool is_added_additional_data, bool& is_reflection)
	{
		if (jongsung == 0 && !is_added_additional_data)
		{
			element* operand = storage_()->pop();

			if (operand == nullptr)
			{
				return true;
			}

			switch (operand->index())
			{
			case 0:
			{
				if (std::get<0>(*operand).is_integer())
				{
					is_reflection = std::get<0>(*operand).integer() == 0 ? true : false;
				}
				else
				{
					is_reflection = std::get<0>(*operand).decimal() == 0.0 ? true : false;
				}

				break;
			}

			case 1:
				is_reflection = std::get<1>(*operand) == 0 ? true : false;
				break;

			case 2:
				is_reflection = std::get<2>(*operand).empty();
				break;

			case 3:
				is_reflection = true;
				break;
			}

			return false;
		}

		return true;
	}
	bool interpreter::expand_storage_(char32_t jongsung, bool is_added_additional_data)
	{
		if (jongsung == U'ㄱ')
		{
			if (storage_()->type() == storage_type::list)
			{
				app::list* list = reinterpret_cast<app::list*>(storage_());

				element* new_virtual_length = storage_()->pop();
			
				if (new_virtual_length == nullptr)
				{
					return true;
				}
				else if (new_virtual_length->index() >= 2)
				{
					storage_()->push(new_virtual_length);

					return true;
				}

				long long new_virtual_length_soon;
				
				switch (new_virtual_length->index())
				{
				case 0:
					new_virtual_length_soon = std::get<0>(*new_virtual_length).integer();
					break;

				case 1:
					new_virtual_length_soon = static_cast<long long>(std::get<1>(*new_virtual_length));
					break;
				}

				if (new_virtual_length_soon < 0)
				{
					new_virtual_length_soon = 0;
				}
				else if (new_virtual_length_soon > list->length())
				{
					new_virtual_length_soon = list->length();
				}

				list->virtual_length(new_virtual_length_soon);
			}

			return false;
		}
		else if (jongsung == U'ㄲ' && !is_added_additional_data)
		{
			if (storage_()->type() == storage_type::list)
			{
				app::list* list = reinterpret_cast<app::list*>(storage_());

				element* new_virtual_length = storage_()->pop();

				if (new_virtual_length == nullptr)
				{
					return true;
				}
				else if (new_virtual_length->index() >= 2)
				{
					storage_()->push(new_virtual_length);

					return true;
				}

				long long new_virtual_length_soon = list->virtual_length();

				switch (new_virtual_length->index())
				{
				case 0:
					new_virtual_length_soon += std::get<0>(*new_virtual_length).integer();
					break;

				case 1:
					new_virtual_length_soon += static_cast<long long>(std::get<1>(*new_virtual_length));
					break;
				}

				if (new_virtual_length_soon < 0)
				{
					new_virtual_length_soon = 0;
				}
				else if (new_virtual_length_soon > list->length())
				{
					new_virtual_length_soon = list->length();
				}

				list->virtual_length(new_virtual_length_soon);
			}

			return false;
		}
		else if (jongsung == U'ㄳ' && !is_added_additional_data)
		{
			if (storage_()->type() == storage_type::list)
			{
				app::list* list = reinterpret_cast<app::list*>(storage_());

				element* new_virtual_length = storage_()->pop();

				if (new_virtual_length == nullptr)
				{
					return true;
				}
				else if (new_virtual_length->index() >= 2)
				{
					storage_()->push(new_virtual_length);

					return true;
				}

				long long new_virtual_length_soon = list->virtual_length();

				switch (new_virtual_length->index())
				{
				case 0:
					new_virtual_length_soon -= std::get<0>(*new_virtual_length).integer();
					break;

				case 1:
					new_virtual_length_soon -= static_cast<long long>(std::get<1>(*new_virtual_length));
					break;
				}

				if (new_virtual_length_soon < 0)
				{
					new_virtual_length_soon = 0;
				}
				else if (new_virtual_length_soon > list->length())
				{
					new_virtual_length_soon = list->length();
				}

				list->virtual_length(new_virtual_length_soon);
			}

			return false;
		}
		else if (jongsung == U'ㄲ' && is_added_additional_data)
		{
			if (storage_()->type() == storage_type::list)
			{
				app::list* list = reinterpret_cast<app::list*>(storage_());

				list->push(new element(number(static_cast<long long>(list->virtual_length()))));
			}
			else
			{
				storage_()->push(new element(number(static_cast<long long>(storage_()->length()))));
			}

			return false;
		}
		else if (jongsung == U'ㄳ' && is_added_additional_data)
		{
			storage_()->push(new element(number(static_cast<long long>(storage_()->length()))));

			return false;
		}
		else if (jongsung == U'ㄴ')
		{
			element* index = storage_()->pop();

			if (index == nullptr)
			{
				return true;
			}
			else if (index->index() >= 2)
			{
				storage_()->push(index);

				return true;
			}

			long long new_index;

			switch (index->index())
			{
			case 0:
				new_index = std::get<0>(*index).integer();
				break;

			case 1:
				new_index = static_cast<long long>(std::get<1>(*index));
				break;
			}

			if (new_index < 0)
			{
				new_index = 0;
			}
			else if (new_index > storages_[selected_index_].size())
			{
				new_index = storages_[selected_index_].size();
			}

			if (new_index == storages_[selected_index_].size())
			{
				if (storages_[selected_index_][0]->type() == storage_type::list)
				{
					storages_[selected_index_].push_back(new list());
				}
				else if (storages_[selected_index_][0]->type() == storage_type::queue)
				{
					storages_[selected_index_].push_back(new queue());
				}
			}

			storage_indexs_[selected_index_] = new_index;

			return false;
		}
		else if (jongsung == U'ㄵ' && !is_added_additional_data)
		{
			element* index = storage_()->pop();

			if (index == nullptr)
			{
				return true;
			}
			else if (index->index() >= 2)
			{
				storage_()->push(index);

				return true;
			}

			long long new_index = storage_indexs_[selected_index_];

			switch (index->index())
			{
			case 0:
				new_index += std::get<0>(*index).integer();
				break;

			case 1:
				new_index += static_cast<long long>(std::get<1>(*index));
				break;
			}

			if (new_index < 0)
			{
				new_index = 0;
			}
			else if (new_index > storages_[selected_index_].size())
			{
				new_index = storages_[selected_index_].size();
			}

			if (new_index == storages_[selected_index_].size())
			{
				if (storages_[selected_index_][0]->type() == storage_type::list)
				{
					storages_[selected_index_].push_back(new list());
				}
				else if (storages_[selected_index_][0]->type() == storage_type::queue)
				{
					storages_[selected_index_].push_back(new queue());
				}
			}

			storage_indexs_[selected_index_] = new_index;

			return false;
		}
		else if (jongsung == U'ㄶ' && !is_added_additional_data)
		{
			element* index = storage_()->pop();

			if (index == nullptr)
			{
				return true;
			}
			else if (index->index() >= 2)
			{
				storage_()->push(index);

				return true;
			}

			long long new_index = storage_indexs_[selected_index_];

			switch (index->index())
			{
			case 0:
				new_index -= std::get<0>(*index).integer();
				break;

			case 1:
				new_index -= static_cast<long long>(std::get<1>(*index));
				break;
			}

			if (new_index < 0)
			{
				new_index = 0;
			}
			else if (new_index > storages_[selected_index_].size())
			{
				new_index = storages_[selected_index_].size();
			}

			if (new_index == storages_[selected_index_].size())
			{
				if (storages_[selected_index_][0]->type() == storage_type::list)
				{
					storages_[selected_index_].push_back(new list());
				}
				else if (storages_[selected_index_][0]->type() == storage_type::queue)
				{
					storages_[selected_index_].push_back(new queue());
				}
			}

			storage_indexs_[selected_index_] = new_index;

			return false;
		}
		else if (jongsung == U'ㄵ' && is_added_additional_data)
		{
			storage_()->push(new element(number(static_cast<long long>(storage_indexs_[selected_index_]))));
		
			return false;
		}
		else if (jongsung == U'ㄶ' && is_added_additional_data)
		{
			storage_()->push(new element(number(static_cast<long long>(storages_[selected_index_].size()))));
		
			return false;
		}

		return true;
	}
}