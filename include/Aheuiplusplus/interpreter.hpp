#ifndef AHEUIPLUSPLUS_HEADER_INTERPRETER_HPP
#define AHEUIPLUSPLUS_HEADER_INTERPRETER_HPP

#include <Aheuiplusplus/function.hpp>
#include <Aheuiplusplus/storage.hpp>
#include <Aheuiplusplus/version.hpp>

#include <vector>

namespace app
{
	class interpreter final
	{
	public:
		interpreter() = default;
		interpreter(app::version version);
		interpreter(const interpreter& interpreter) = delete;
		interpreter(interpreter&& interpreter) noexcept = delete;
		~interpreter() = default;

	public:
		interpreter& operator=(const interpreter& interpreter) = delete;
		interpreter& operator=(interpreter&& interpreter) noexcept = delete;
		bool operator==(const interpreter& interpreter) const = delete;
		bool operator!=(const interpreter& interpreter) const = delete;

	public:
		app::version version() const noexcept;
		const std::vector<std::vector<app::storage*>>& storages() const noexcept;
		const app::storage* storage(std::size_t index) const;
		const std::vector<std::size_t>& storage_indexs() const noexcept;
		std::size_t storage_index(std::size_t index) const;
		const std::vector<function*>& functions() const noexcept;

	private:
		app::version version_ = app::version::v1_0;
		std::vector<std::vector<app::storage*>> storages_;
		std::vector<std::size_t> storage_indexs_;
		std::vector<function*> functions_;
	};
}

#endif