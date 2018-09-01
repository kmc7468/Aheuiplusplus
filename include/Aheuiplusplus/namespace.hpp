#ifndef AHEUIPLUSPLUS_HEADER_NAMESPACE_HPP
#define AHEUIPLUSPLUS_HEADER_NAMESPACE_HPP

#include <Aheuiplusplus/code.hpp>
#include <Aheuiplusplus/function.hpp>

#include <memory>
#include <vector>

namespace app
{
	class namespace_info final
	{
	public:
		namespace_info(const code_view& name);
		namespace_info(code&& name);
		namespace_info(const namespace_info& namespace_) = delete;
		namespace_info(namespace_info&& namespace_) noexcept = delete;
		~namespace_info() = default;

	public:
		namespace_info& operator=(const namespace_info& namespace_) = delete;
		namespace_info& operator=(namespace_info&& namespace_) noexcept = delete;
		bool operator==(const namespace_info& namespace_) const = delete;
		bool operator!=(const namespace_info& namespace_) const = delete;

	public:
		code_view name() const;

		const std::vector<std::shared_ptr<namespace_info>>& namespaces() const noexcept;
		std::vector<std::shared_ptr<namespace_info>>& namespaces() noexcept;
		const std::vector<function_ptr>& functions() const noexcept;
		std::vector<function_ptr>& functions() noexcept;

	private:
		code name_;

		std::vector<std::shared_ptr<namespace_info>> namespaces_;
		std::vector<function_ptr> functions_;
	};

	using namespace_ptr = std::shared_ptr<namespace_info>;
}

#endif