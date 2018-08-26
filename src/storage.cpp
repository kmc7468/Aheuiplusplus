#include <Aheuiplusplus/storage.hpp>

#include <algorithm>
#include <utility>

namespace app
{
	list::list(const list& list)
		: list_(list.list_), virtual_length_(list.virtual_length_)
	{}
	list::list(list&& list) noexcept
		: list_(std::move(list.list_)), virtual_length_(list.virtual_length_)
	{
		list.virtual_length_ = 0;
	}

	list& list::operator=(const list& list)
	{
		list_ = list.list_;
		virtual_length_ = list.virtual_length_;

		return *this;
	}
	list& list::operator=(list&& list) noexcept
	{
		list_ = std::move(list.list_);
		virtual_length_ = list.virtual_length_;
		
		list.virtual_length_ = 0;

		return *this;
	}

	storage_type list::type() const noexcept
	{
		return storage_type::list;
	}

	element_ptr list::pop()
	{
		if (virtual_length_ == 0)
			return nullptr;

		std::vector<element_ptr>::iterator end = list_.begin() + virtual_length_;

		element_ptr result = *end;
		list_.erase(end);
		virtual_length_ -= 1;

		return result;
	}
	element_ptr list::push(const element_ptr& value)
	{
		return list_.insert(list_.begin() + virtual_length_++, value), value;
	}

	element_ptr list::copy()
	{
		if (virtual_length_ == 0)
			return nullptr;

		std::vector<element_ptr>::iterator end = list_.begin() + virtual_length_;
		element_ptr result = element_ptr(new element(**(end - 1)));

		return push(result), result;
	}
	void list::move(const element_ptr& value)
	{
		push(value);
	}
	void list::swap()
	{
		if (virtual_length_ < 2)
			return;

		std::vector<element_ptr>::iterator last_iter = list_.begin() + virtual_length_ - 1;
		std::iter_swap(last_iter, last_iter - 1);
	}

	std::size_t list::size() const noexcept
	{
		return virtual_length_;
	}

	std::size_t list::physical_size() const noexcept
	{
		return list_.size();
	}
}

namespace app
{
	queue::queue(const queue& queue)
		: deque_(queue.deque_)
	{}
	queue::queue(queue&& queue) noexcept
		: deque_(std::move(queue.deque_))
	{}

	queue& queue::operator=(const queue& queue)
	{
		deque_ = queue.deque_;

		return *this;
	}
	queue& queue::operator=(queue&& queue) noexcept
	{
		deque_ = std::move(queue.deque_);

		return *this;
	}

	storage_type queue::type() const noexcept
	{
		return storage_type::queue;
	}

	element_ptr queue::pop()
	{
		if (deque_.size() == 0)
			return nullptr;
		
		element_ptr result = deque_.front();
		deque_.pop_front();

		return result;
	}
	element_ptr queue::push(const element_ptr& value)
	{
		return deque_.push_back(value), value;
	}

	element_ptr queue::copy()
	{
		element_ptr result = element_ptr(new element(*deque_.front()));

		return deque_.push_front(result), result;
	}
	void queue::move(const element_ptr& value)
	{
		push(value);
	}
	void queue::swap()
	{
		if (deque_.size() < 2)
			return;

		std::iter_swap(deque_.begin(), deque_.begin() + 1);
	}

	std::size_t queue::size() const noexcept
	{
		return deque_.size();
	}
}

namespace app
{
	pipe::pipe(extension* extension)
		: extension_(extension)
	{}
	pipe::pipe(pipe&& pipe) noexcept
		: extension_(pipe.extension_)
	{
		pipe.extension_ = nullptr;
	}

	pipe& pipe::operator=(pipe&& pipe) noexcept
	{
		extension_ = pipe.extension_;
		pipe.extension_ = nullptr;

		return *this;
	}

	storage_type pipe::type() const noexcept
	{
		return storage_type::pipe;
	}

	element_ptr pipe::pop()
	{
		if (!extension_)
			return nullptr;

		return extension_->pop();
	}
	element_ptr pipe::push(const element_ptr& value)
	{
		if (!extension_)
			return nullptr;

		return extension_->push(value);
	}

	element_ptr pipe::copy()
	{
		if (extension_ && last_sent_value_)
		{
			return extension_->push(last_sent_value_);
		}
		else
		{
			return nullptr;
		}
	}
	void pipe::move(const element_ptr& value)
	{
		push(value);
	}
	void pipe::swap()
	{}

	std::size_t pipe::size() const noexcept
	{
		return static_cast<std::size_t>(-1);
	}
}