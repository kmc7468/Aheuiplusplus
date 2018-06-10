#ifndef AHEUIPLUSPLUS_HEADER_STORAGE_HPP
#define AHEUIPLUSPLUS_HEADER_STORAGE_HPP

#include <Aheuiplusplus/code.hpp>
#include <Aheuiplusplus/function.hpp>

#include <cstddef>
#include <variant>

namespace app
{
	enum class storage_type
	{
		list,
		queue,
		passage,
	};

	using element = std::variant<double, char32_t, code, function*>;

	class storage
	{
	public:
		storage() = default;
		storage(const storage& storage) = delete;
		storage(storage&& storage) noexcept = delete;
		virtual ~storage() = default;

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

	public:
		std::size_t virtual_length() const noexcept;
		void virtual_length(std::size_t new_virtual_length) noexcept;

	private:
		std::size_t virtual_length_;
	};
}

#endif