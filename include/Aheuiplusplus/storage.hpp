#ifndef AHEUIPLUSPLUS_HEADER_STORAGE_HPP
#define AHEUIPLUSPLUS_HEADER_STORAGE_HPP

#include <Aheuiplusplus/code.hpp>

#include <cstddef>
#include <queue>
#include <variant>
#include <vector>

namespace app
{
	class function;

	enum class storage_type
	{
		list,
		queue,
		passage,
	};

	using element = std::variant<double, char32_t, raw_code, function*>;

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
		virtual element* pop() = 0;
		virtual void push(element* element) = 0;
		virtual std::size_t length() const = 0;
	};

	class list final : public storage
	{
	public:
		list() = default;
		list(const list& storage) = delete;
		list(list&& storage) noexcept = delete;
		virtual ~list() override = default;

	public:
		list& operator=(const list& storage) = delete;
		list& operator=(list&& storage) noexcept = delete;
		bool operator==(const list& storage) const = delete;
		bool operator!=(const list& storage) const = delete;

	public:
		virtual storage_type type() const noexcept override;
		virtual element* pop() override;
		virtual void push(element* element) override;
		virtual std::size_t length() const override;

	public:
		std::size_t virtual_length() const noexcept;
		void virtual_length(std::size_t new_virtual_length);

	private:
		std::vector<element*> vector_;
		std::size_t virtual_length_;
	};

	class queue final : public storage
	{
	public:
		queue() = default;
		queue(const queue& storage) = delete;
		queue(queue&& storage) noexcept = delete;
		virtual ~queue() override = default;

	public:
		queue& operator=(const queue& storage) = delete;
		queue& operator=(queue&& storage) noexcept = delete;
		bool operator==(const queue& storage) const = delete;
		bool operator!=(const queue& storage) const = delete;

	public:
		virtual storage_type type() const noexcept override;
		virtual element* pop() override;
		virtual void push(element* element) override;
		virtual std::size_t length() const override;

	private:
		std::queue<element*> queue_;
	};
}

#endif