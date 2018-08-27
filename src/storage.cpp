#include <Aheuiplusplus/storage.hpp>

#include <algorithm>
#include <stdexcept>
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

	void list::clear()
	{
		list_.clear();
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

	void queue::clear()
	{
		deque_.clear();
	}
}

namespace app
{
	pipe::pipe(extension* extension) noexcept
		: extension_(extension)
	{}
	pipe::pipe(const pipe& pipe) noexcept
		: extension_(pipe.extension_)
	{}
	pipe::pipe(pipe&& pipe) noexcept
		: extension_(pipe.extension_)
	{
		pipe.extension_ = nullptr;
	}

	pipe& pipe::operator=(const pipe& pipe) noexcept
	{
		extension_ = pipe.extension_;
		
		return *this;
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

	void pipe::clear()
	{}
}

namespace app
{
	storages::storages()
	{
		for (std::size_t i = 0; i < 26; ++i) // List
		{
			std::vector<storage_ptr> lists;
			lists.emplace_back(new list());

			storages_.push_back(lists);
		}

		std::vector<storage_ptr> queues; // Queue
		queues.emplace_back(new queue());

		storages_.insert(storages_.begin() + 21, queues);

		storages_.emplace_back(); // Pipe

		for (std::size_t i = 0; i < 28; ++i)
		{
			storages_index_.push_back(0);
		}
	}
	storages::storages(const storages& storages)
		: storages_(storages.storages_), storages_index_(storages.storages_index_), selected_storage_(storages.selected_storage_)
	{}
	storages::storages(storages&& storages) noexcept
		: storages_(std::move(storages.storages_)), storages_index_(std::move(storages.storages_index_)),
		selected_storage_(storages.selected_storage_)
	{
		storages.selected_storage_ = 0;
	}

	storages& storages::operator=(const storages& storages)
	{
		storages_ = storages.storages_;
		storages_index_ = storages.storages_index_;
		selected_storage_ = storages.selected_storage_;

		return *this;
	}
	storages& storages::operator=(storages&& storages) noexcept
	{
		storages_ = std::move(storages.storages_);
		storages_index_ = std::move(storages.storages_index_);
		selected_storage_ = storages.selected_storage_;

		storages.selected_storage_ = 0;

		return *this;
	}

	storage_ptr storages::get() const
	{
		return storages_[selected_storage_][storages_index_[selected_storage_]];
	}
	void storages::reset()
	{
		for (std::vector<storage_ptr>& storages : storages_)
		{
			storage_ptr first = storages.front();
			
			storages.clear();
			
			first->clear();
			storages.push_back(std::move(first));
		}
	}

	std::size_t storages::storage_max_index() const noexcept
	{
		return storages_[selected_storage_].size() - 1;
	}
	std::size_t storages::storage_index() const noexcept
	{
		return storages_index_[selected_storage_];
	}
	void storages::storage_index(std::size_t new_storage_index)
	{
		if (new_storage_index > storages_[selected_storage_].size())
			throw std::out_of_range("인수 new_storage_index는 함숫값 app::storages::storage_max_index() const noexcept보다 1 큰 값 이하여야 합니다.");

		if (new_storage_index == storages_[selected_storage_].size())
		{
			switch (storages_[selected_storage_][0]->type())
			{
			case storage_type::list:
				storages_[selected_storage_].emplace_back(new list());
				break;

			case storage_type::queue:
				storages_[selected_storage_].emplace_back(new queue());
				break;

			case storage_type::pipe:
				storages_[selected_storage_].emplace_back(get());
				break;
			}
		}

		storages_index_[selected_storage_] = new_storage_index;
	}
	void storages::selected_storage(std::size_t new_selected_storage)
	{
		if (new_selected_storage >= 28)
			throw std::out_of_range("인수 new_selected_storage는 28 미만이여야 합니다.");

		selected_storage_ = new_selected_storage;
	}
}