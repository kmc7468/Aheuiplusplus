#ifndef AHEUIPLUSPLUS_HEADER_EXTENSION_HPP
#define AHEUIPLUSPLUS_HEADER_EXTENSION_HPP

#include <Aheuiplusplus/element.hpp>

#include <string>

namespace app
{
	class extension
	{
	public:
		extension(const extension& extension) = delete;
		extension(extension&& extension) noexcept = delete;
		virtual ~extension() = default;

	protected:
		extension() = default;

	public:
		extension& operator=(const extension& extension) = delete;
		extension& operator=(extension&& extension) noexcept = delete;
		bool operator==(const extension& extension) const = delete;
		bool operator!=(const extension& extension) const = delete;

	public:
		virtual std::u32string name() const = 0;
		virtual std::u32string developer() const = 0;
		virtual std::u32string description() const;

		virtual element_ptr pop() = 0;
		virtual void push(const element_ptr& value) = 0;
	};
}

#endif