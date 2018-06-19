#ifndef AHEUIPLUSPLUS_HEADER_INTERPRETER_HPP
#define AHEUIPLUSPLUS_HEADER_INTERPRETER_HPP

#define AHEUIPLUSPLUS_VERSION_STRING ("1.2.0-pre2")
#define AHEUIPLUSPLUS_VERSION_PRE ("pre2")
#define AHEUIPLUSPLUS_VERSION (1)
#define AHEUIPLUSPLUS_VERSION_MAJOR AHEUIPLUSPLUS_VERSION
#define AHEUIPLUSPLUS_VERSION_MINOR (2)
#define AHEUIPLUSPLUS_VERSION_PATCH (0)

#include <Aheuiplusplus/code.hpp>
#include <Aheuiplusplus/function.hpp>
#include <Aheuiplusplus/storage.hpp>
#include <Aheuiplusplus/version.hpp>

#include <memory>
#include <vector>

namespace app
{
	class debugger;

	class interpreter final
	{
		friend class app::debugger;

	public:
		interpreter(std::FILE* input_stream, std::FILE* output_stream);
		interpreter(app::version version, std::FILE* input_stream, std::FILE* output_stream);
		interpreter(const interpreter& interpreter) = delete;
		interpreter(interpreter&& interpreter) noexcept = delete;
		~interpreter();

	public:
		interpreter& operator=(const interpreter& interpreter) = delete;
		interpreter& operator=(interpreter&& interpreter) noexcept = delete;
		bool operator==(const interpreter& interpreter) const = delete;
		bool operator!=(const interpreter& interpreter) const = delete;

	public:
		long long run(const raw_code& code);
		long long run(const raw_code& code, bool command_aheui, bool command_loud_mode);

		const app::storage* storage(std::size_t index) const;
		std::size_t storage_index(std::size_t index) const;

	private:
		void initialize_();

		long long run_(const raw_code& code, bool command_aheui, bool command_loud_mode,
			std::size_t& x, std::size_t& y, std::size_t& direction,
			std::size_t& move, bool& is_ignored, bool& is_reflection, char32_t& start_of_expression,
			bool& is_out_of_version);

		bool type_and_mode_(char32_t jongsung, bool is_added_additional_data, char32_t& start_of_expression,
			std::size_t processed_command_without_expression, bool& is_out_of_version);

		bool add_(char32_t jongsung, bool is_added_additional_data);
		bool mul_(char32_t jongsung, bool is_added_additional_data);
		bool sub_(char32_t jongsung, bool is_added_additional_data);
		bool div_(char32_t jongsung, bool is_added_additional_data);
		bool mod_(char32_t jongsung, bool is_added_additional_data);
		
		bool pop_(char32_t jongsung, bool is_added_additional_data);
		bool push_(char32_t jongsung, bool is_added_additional_data);
		bool copy_(char32_t jongsung, bool is_added_additional_data);
		bool swap_(char32_t jongsung, bool is_added_additional_data);

		bool change_storage_(char32_t jongsung, bool is_added_additional_data);
		bool move_(char32_t jongsung, bool is_added_additional_data);
		bool compare_(char32_t jongsung, bool is_added_additional_data);
		bool is_zero_(char32_t jongsung, bool is_added_additional_data, bool& is_reflection);
		bool expand_storage_(char32_t jongsung, bool is_added_additional_data);

		long long exit_();

		void go_(std::size_t& x, std::size_t& y, std::size_t move, std::size_t direction, app::code& splited_code);
		long long get_integer_(char32_t jongsung, bool is_added_additional_data);
		void type_casting_arithmetic_(element* left_operand, element* right_operand,
			std::shared_ptr<element>& left_operand_out, std::shared_ptr<element>& right_operand_out) const;
		void type_casting_compare_(element* left_operand, element* right_operand,
			std::shared_ptr<element>& left_operand_out, std::shared_ptr<element>& right_operand_out) const;

		void storage_backup_and_restore_();
		void delete_storage_();

		app::storage* storage_();

	public:
		app::version version() const noexcept;
		const std::vector<std::vector<app::storage*>>& storages() const noexcept;
		const std::vector<std::size_t>& storage_indexs() const noexcept;
		std::size_t selected_index() const noexcept;
		const std::vector<function*>& functions() const noexcept;

		bool is_integer_mode() const noexcept;
		bool is_compatible_with_aheui() const noexcept;

		std::FILE* input_stream();
		std::FILE* output_stream();

	private:
		app::version version_ = app::version::v1_0;
		std::vector<std::vector<app::storage*>> storages_;
		std::vector<std::size_t> storage_indexs_;
		std::size_t selected_index_ = 0;
		std::vector<function*> functions_;

		bool is_integer_mode_ = true;
		bool is_compatible_with_aheui_ = true;

		bool is_loud_mode_ = false;

		std::FILE* input_stream_;
		std::FILE* output_stream_;
		app::debugger* debugger_ = nullptr;

	public:
		static constexpr const char* version_string = AHEUIPLUSPLUS_VERSION_STRING;
		static constexpr const char* version_pre = AHEUIPLUSPLUS_VERSION_PRE;
		static constexpr int version_major = AHEUIPLUSPLUS_VERSION_MAJOR;
		static constexpr int version_minor = AHEUIPLUSPLUS_VERSION_MINOR;
		static constexpr int version_patch = AHEUIPLUSPLUS_VERSION_PATCH;
	};
}

#endif