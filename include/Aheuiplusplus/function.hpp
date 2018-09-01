#ifndef AHEUIPLUSPLUS_HEADER_FUNCTION_HPP
#define AHEUIPLUSPLUS_HEADER_FUNCTION_HPP

#include <Aheuiplusplus/code.hpp>
#include <Aheuiplusplus/storage.hpp>

#include <functional>
#include <memory>

namespace app
{
	enum class function_type
	{
		named_function,
		unnamed_function,
		native_function,
	};

	class function
	{
	public:
		function(const function& function) = delete;
		function(function&& function) noexcept = delete;
		virtual ~function() = default;

	protected:
		function() = default;

	public:
		function& operator=(const function& function) = delete;
		function& operator=(function&& function) noexcept = delete;
		bool operator==(const function& function) const = delete;
		bool operator!=(const function& function) const = delete;

	public:
		virtual code name() const = 0;
		virtual function_type type() const noexcept = 0;
		virtual void call(storages& storages) = 0;
	};

	using function_ptr = std::shared_ptr<function>;

	class named_function final : public function
	{
	public:
		named_function(const code_view& name, const code_view& code);
		named_function(code&& name, const code_view& code);
		named_function(const code_view& name, code&& code);
		named_function(code&& name, code&& code) noexcept;
		named_function(const named_function& function) = delete;
		named_function(named_function&& function) noexcept = delete;
		virtual ~named_function() override = default;

	public:
		named_function& operator=(const named_function& function) = delete;
		named_function& operator=(named_function&& function) noexcept = delete;
		bool operator==(const named_function& function) const = delete;
		bool operator!=(const named_function& function) const = delete;

	public:
		virtual code name() const override;
		virtual function_type type() const noexcept override;
		virtual void call(storages& storages) override;

	private:
		code name_;
		code code_;
	};

	class unnamed_function final : public function
	{
	public:
		explicit unnamed_function(const code_view& code);
		explicit unnamed_function(code&& code) noexcept;
		unnamed_function(const unnamed_function& function) = delete;
		unnamed_function(unnamed_function&& function) noexcept = delete;
		virtual ~unnamed_function() override = default;

	public:
		unnamed_function& operator=(const unnamed_function& function) = delete;
		unnamed_function& operator=(unnamed_function&& function) noexcept = delete;
		bool operator==(const unnamed_function& function) const = delete;
		bool operator!=(const unnamed_function& function) const = delete;

	public:
		virtual code name() const override;
		virtual function_type type() const noexcept override;
		virtual void call(storages& storages) override;

	private:
		code code_;
	};

	using native_function_object = std::function<void(storages&)>;

	class native_function final : public function
	{
	public:
		native_function(const code_view& name, const native_function_object& function);
		native_function(const code_view& name, native_function_object&& function);
		native_function(code&& name, const native_function_object& function);
		native_function(code&& name, native_function_object&& function);
		native_function(const native_function& function) = delete;
		native_function(native_function&& function) noexcept = delete;
		virtual ~native_function() override = default;

	public:
		native_function& operator=(const native_function& function) = delete;
		native_function& operator=(native_function&& function) noexcept = delete;
		bool operator==(const native_function& function) const = delete;
		bool operator!=(const native_function& function) const = delete;

	public:
		virtual code name() const override;
		virtual function_type type() const noexcept override;
		virtual void call(storages& storages) override;

	private:
		code name_;
		native_function_object function_;
	};
}

#endif