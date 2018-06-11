#include <Aheuiplusplus/storage.hpp>

#include <Aheuiplusplus/function.hpp>

#include <stdexcept>

namespace app
{
	number::number(long long integer) noexcept
		: integer_(integer), decimal_(integer)
	{}
	number::number(double decimal) noexcept
		: integer_(decimal), decimal_(decimal), is_integer_(false)
	{}
	number::number(const number& number) noexcept
		: integer_(number.integer_), decimal_(number.decimal_), is_integer_(number.is_integer_)
	{}

	number& number::operator=(const number& number) noexcept
	{
		integer_ = number.integer_;
		decimal_ = number.decimal_;

		return *this;
	}
	bool number::operator==(const number& number) const noexcept
	{
		if (is_integer_ != number.is_integer_)
		{
			return false;
		}

		if (is_integer_)
		{
			return integer_ == number.integer_;
		}
		else
		{
			return decimal_ == number.decimal_;
		}
	}
	bool number::operator!=(const number& number) const noexcept
	{
		if (is_integer_ != number.is_integer_)
		{
			return true;
		}

		if (is_integer_)
		{
			return integer_ != number.integer_;
		}
		else
		{
			return decimal_ != number.decimal_;
		}
	}
	bool number::operator>(const number& number) const
	{
		if (is_integer_ != number.is_integer_)
			throw std::invalid_argument("인수 number와 비교할 수 없습니다.");

		if (is_integer_)
		{
			return integer_ > number.integer_;
		}
		else
		{
			return decimal_ > number.decimal_;
		}
	}
	bool number::operator>=(const number& number) const
	{
		if (is_integer_ != number.is_integer_)
			throw std::invalid_argument("인수 number와 비교할 수 없습니다.");

		if (is_integer_)
		{
			return integer_ >= number.integer_;
		}
		else
		{
			return decimal_ >= number.decimal_;
		}
	}
	bool number::operator<(const number& number) const
	{
		if (is_integer_ != number.is_integer_)
			throw std::invalid_argument("인수 number와 비교할 수 없습니다.");

		if (is_integer_)
		{
			return integer_ < number.integer_;
		}
		else
		{
			return decimal_ < number.decimal_;
		}
	}
	bool number::operator<=(const number& number) const
	{
		if (is_integer_ != number.is_integer_)
			throw std::invalid_argument("인수 number와 비교할 수 없습니다.");

		if (is_integer_)
		{
			return integer_ <= number.integer_;
		}
		else
		{
			return decimal_ <= number.decimal_;
		}
	}

	long long number::integer() const noexcept
	{
		return integer_;
	}
	void number::integer(long long new_integer) noexcept
	{
		if (!is_integer_)
		{
			is_integer_ = true;
		}

		integer_ = new_integer;
		decimal_ = new_integer;
	}
	double number::decimal() const noexcept
	{
		return decimal_;
	}
	void number::decimal(double new_decimal) noexcept
	{
		if (is_integer_)
		{
			is_integer_ = false;
		}

		integer_ = new_decimal;
		decimal_ = new_decimal;
	}
	bool number::is_integer() const noexcept
	{
		return is_integer_;
	}
	void number::is_integer(bool new_is_integer) noexcept
	{
		is_integer_ = new_is_integer;
	}
}

namespace app
{
	storage_type list::type() const noexcept
	{
		return storage_type::list;
	}
	element* list::pop()
	{
		std::vector<element*>::iterator end = vector_.begin() + virtual_length();

		if (vector_.begin() == end)
		{
			return nullptr;
		}

		element* data = *(end - 1);
		vector_.erase(end - 1);
		virtual_length(virtual_length() - 1);

		return data;
	}
	void list::push(element* element)
	{
		vector_.push_back(element);
		virtual_length(virtual_length() + 1);
	}
	element* list::copy()
	{
		element* last = pop();
		push(last);

		switch (last->index())
		{
		case 0:
			push(new element(std::get<0>(*last)));
			break;

		case 1:
			push(new element(std::get<1>(*last)));
			break;

		case 2:
			push(new element(std::get<2>(*last)));
			break;

		case 3:
			push(new element(std::get<3>(*last)));
			break;
		}

		last = pop();
		push(last);

		return last;
	}
	std::size_t list::length() const
	{
		return vector_.size();
	}

	std::size_t list::virtual_length() const noexcept
	{
		return virtual_length_;
	}
	void list::virtual_length(std::size_t new_virtual_length)
	{
		if (new_virtual_length > length())
			throw std::out_of_range("인수 new_virtual_length는 함숫값 length()보다 클 수 없습니다.");

		virtual_length_ = new_virtual_length;
	}
}

namespace app
{
	storage_type queue::type() const noexcept
	{
		return storage_type::queue;
	}
	element* queue::pop()
	{
		if (queue_.size() == 0)
		{
			return nullptr;
		}

		element* data = queue_.front();
		queue_.pop_front();

		return data;
	}
	void queue::push(element* element)
	{
		queue_.push_back(element);
	}
	element* queue::copy()
	{
		element* data = queue_.front();
		
		switch (data->index())
		{
		case 0:
			queue_.push_front(new element(std::get<0>(*data)));
			break;

		case 1:
			queue_.push_front(new element(std::get<1>(*data)));
			break;

		case 2:
			queue_.push_front(new element(std::get<2>(*data)));
			break;

		case 3:
			queue_.push_front(new element(std::get<3>(*data)));
			break;
		}

		return queue_.front();
	}
	std::size_t queue::length() const
	{
		return queue_.size();
	}
}