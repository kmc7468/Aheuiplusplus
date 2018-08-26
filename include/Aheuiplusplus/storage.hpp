#ifndef AHEUIPLUSPLUS_HEADER_STORAGE_HPP
#define AHEUIPLUSPLUS_HEADER_STORAGE_HPP

#include <Aheuiplusplus/element.hpp>
#include <Aheuiplusplus/extension.hpp>

#include <cstddef>
#include <deque>
#include <memory>
#include <vector>

namespace app
{
	enum class storage_type
	{
		list,
		queue,
		pipe,
	};

	class storage
	{
	public:
		storage(const storage& storage) = delete;
		storage(storage&& storage) noexcept = delete;
		virtual ~storage() = default;

	protected:
		storage() = default;

	public:
		storage& operator=(const storage& storage) = delete;
		storage& operator=(storage&& storage) noexcept = delete;
		bool operator==(const storage& storage) const = delete;
		bool operator!=(const storage& storage) const = delete;

	public:
		virtual storage_type type() const noexcept = 0;

		virtual element_ptr pop() = 0;
		virtual element_ptr push(const element_ptr& value) = 0;

		virtual element_ptr copy() = 0;
		virtual void move(const element_ptr& value) = 0;
		virtual void swap() = 0;

		virtual std::size_t size() const noexcept = 0;
	};

	using storage_ptr = std::shared_ptr<storage>;

	class list final : public storage
	{
	public:
		list() = default;
		list(const list& list);
		list(list&& list) noexcept;
		virtual ~list() override = default;

	public:
		list& operator=(const list& list);
		list& operator=(list&& list) noexcept;
		bool operator==(const list& list) const = delete;
		bool operator!=(const list& list) const = delete;

	public:
		virtual storage_type type() const noexcept override;

		virtual element_ptr pop() override;
		virtual element_ptr push(const element_ptr& value) override;

		virtual element_ptr copy() override;
		virtual void move(const element_ptr& value) override;
		virtual void swap() override;

		virtual std::size_t size() const noexcept override;
		
	public:
		std::size_t physical_size() const noexcept;

	private:
		std::vector<element_ptr> list_;
		std::size_t virtual_length_ = 0;
	};

	class queue final : public storage
	{
	public:
		queue() = default;
		queue(const queue& queue);
		queue(queue&& queue) noexcept;
		virtual ~queue() override = default;

	public:
		queue& operator=(const queue& queue);
		queue& operator=(queue&& queue) noexcept;
		bool operator==(const queue& queue) const = delete;
		bool operator!=(const queue& queue) const = delete;

	public:
		virtual storage_type type() const noexcept override;

		virtual element_ptr pop() override;
		virtual element_ptr push(const element_ptr& value) override;

		virtual element_ptr copy() override;
		virtual void move(const element_ptr& value) override;
		virtual void swap() override;

		virtual std::size_t size() const noexcept override;

	private:
		std::deque<element_ptr> deque_;
	};

	class pipe final : public storage
	{
	public:
		pipe(extension* extension) noexcept;
		pipe(const pipe& pipe) noexcept;
		pipe(pipe&& pipe) noexcept;
		virtual ~pipe() override = default;

	public:
		pipe& operator=(const pipe& pipe) noexcept;
		pipe& operator=(pipe&& pipe) noexcept;
		bool operator==(const pipe& pipe) const = delete;
		bool operator!=(const pipe& pipe) const = delete;

	public:
		virtual storage_type type() const noexcept override;

		virtual element_ptr pop() override;
		virtual element_ptr push(const element_ptr& value) override;

		virtual element_ptr copy() override;
		virtual void move(const element_ptr& value) override;
		virtual void swap() override;

		virtual std::size_t size() const noexcept override;

	private:
		extension* extension_;
		element_ptr last_sent_value_ = nullptr;
	};

	class storages final
	{
	public:
		storages();
		storages(const storages& storages);
		storages(storages&& storages) noexcept;
		~storages() = default;

	public:
		storages& operator=(const storages& storages);
		storages& operator=(storages&& storages) noexcept;
		bool operator==(const storages& storages) const = delete;
		bool operator!=(const storages& storages) const = delete;

	public:
		storage_ptr get() const;

	public:
		std::size_t storage_max_index() const noexcept;
		std::size_t storage_index() const noexcept;
		void storage_index(std::size_t new_storage_index);
		void selected_storage(std::size_t new_selected_storage);

	private:
		std::vector<std::vector<storage_ptr>> storages_;
		std::vector<std::size_t> storages_index_;
		std::size_t selected_storage_ = 0;
	};
}

#endif