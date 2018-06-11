#ifndef AHEUIPLUSPLUS_HEADER_INTERPRETER_HPP
#define AHEUIPLUSPLUS_HEADER_INTERPRETER_HPP

#include <Aheuiplusplus/code.hpp>
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
		interpreter(app::version version, std::FILE* input_stream, std::FILE* output_stream);
		interpreter(const interpreter& interpreter) = delete;
		interpreter(interpreter&& interpreter) noexcept = delete;
		~interpreter() = default;

	public:
		interpreter& operator=(const interpreter& interpreter) = delete;
		interpreter& operator=(interpreter&& interpreter) noexcept = delete;
		bool operator==(const interpreter& interpreter) const = delete;
		bool operator!=(const interpreter& interpreter) const = delete;

	public:
		void run(const raw_code& code);

		const app::storage* storage(std::size_t index) const;
		std::size_t storage_index(std::size_t index) const;

	private:
		void pop_(char32_t jongsung, bool is_added_additional_data);

		void go_left_(std::size_t& x, std::size_t& y, std::size_t move, int direction, app::code& splited_code);
		void go_right_(std::size_t& x, std::size_t& y, std::size_t move, int direction, app::code& splited_code);
		void go_up_(std::size_t& x, std::size_t& y, std::size_t move, int direction, app::code& splited_code);
		void go_down_(std::size_t& x, std::size_t& y, std::size_t move, int direction, app::code& splited_code);
		
		app::storage* storage_();

	public:
		app::version version() const noexcept;
		const std::vector<std::vector<app::storage*>>& storages() const noexcept;
		const std::vector<std::size_t>& storage_indexs() const noexcept;
		std::size_t selected_index() const noexcept;
		const std::vector<function*>& functions() const noexcept;

		std::FILE* input_stream();
		std::FILE* output_stream();

	private:
		app::version version_ = app::version::v1_0;
		std::vector<std::vector<app::storage*>> storages_;
		std::vector<std::size_t> storage_indexs_;
		std::size_t selected_index_ = 0;
		std::vector<function*> functions_;

		std::FILE* input_stream_;
		std::FILE* output_stream_;
	};
}

#endif