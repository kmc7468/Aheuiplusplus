#ifndef AHEUIPLUSPLUS_HEADER_EXTENSION_HPP
#define AHEUIPLUSPLUS_HEADER_EXTENSION_HPP
#ifdef AHEUIPLUSPLUS_USE_EXTENSION

#include <Aheuiplusplus/element.hpp>

#include <map>
#include <string>
#include <tuple>
#include <vector>

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
		virtual element_ptr push(const element_ptr& value) = 0;

		virtual void enabled();
		virtual void disabled();

	public:
		std::tuple<int, int, int> target_version() const;
	};

	class extension_engine final
	{
	public:
		extension_engine(const std::vector<std::string>& extensions);
		extension_engine(const extension_engine& engine) = delete;
		extension_engine(extension_engine&& engine) noexcept = delete;
		~extension_engine();

	public:
		extension_engine& operator=(const extension_engine& engine) = delete;
		extension_engine& operator=(extension_engine&& engine) noexcept = delete;
		bool operator==(const extension_engine& engine) const = delete;
		bool operator!=(const extension_engine& engine) const = delete;
		
	private:
		std::map<void*, extension*> extensions_;
	};
}

#endif
#endif