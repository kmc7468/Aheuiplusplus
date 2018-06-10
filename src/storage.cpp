#include <Aheuiplusplus/storage.hpp>

#include <stdexcept>

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
		element* data = queue_.front();
		queue_.pop();

		return data;
	}
	void queue::push(element* element)
	{
		queue_.push(element);
	}
	std::size_t queue::length() const
	{
		return queue_.size();
	}
}