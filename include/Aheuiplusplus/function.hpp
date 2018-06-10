#ifndef AHEUIPLUSPLUS_HEADER_FUNCTION_HPP
#define AHEUIPLUSPLUS_HEADER_FUNCTION_HPP

#include <Aheuiplusplus/code.hpp>
#include <Aheuiplusplus/storage.hpp>

#include <functional>

namespace app
{
	enum class function_type
	{
		aheuiplusplus_function,
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
		function(const code& name);

	public:
		function& operator=(const function& function) = delete;
		function& operator=(function&& function) noexcept = delete;
		bool operator==(const function& function) const = delete;
		bool operator!=(const function& function) const = delete;

	public:
		virtual function_type type() const noexcept = 0;

	public:
		code name() const;
		void name(const code& new_name);
		void name(code&& new_name) noexcept;

	private:
		code name_;
	};

	class aheuiaheuiplus_function final : public function
	{
	public:
		aheuiaheuiplus_function() = default;
		aheuiaheuiplus_function(const app::code& name);
		aheuiaheuiplus_function(const aheuiaheuiplus_function& function) = delete;
		aheuiaheuiplus_function(aheuiaheuiplus_function&& function) = delete;
		virtual ~aheuiaheuiplus_function() override = default;

	public:
		aheuiaheuiplus_function& operator=(const aheuiaheuiplus_function& function) = delete;
		aheuiaheuiplus_function& operator=(aheuiaheuiplus_function&& function) noexcept = delete;
		bool operator==(const aheuiaheuiplus_function& function) const = delete;
		bool operator!=(const aheuiaheuiplus_function& function) const = delete;

	public:
		virtual function_type type() const noexcept override;

	public:
		app::code code() const;
		void code(const app::code& code);
		void code(app::code&& code) noexcept;

	private:
		app::code code_;
	};

	class native_function final : public function
	{
	public:
		using function_t = std::function<void(std::vector<std::vector<storage*>>&, std::vector<function*>&)>;

	public:
		native_function(const function_t& functor);
		native_function(const app::code& name, const function_t& functor);
		native_function(const native_function& function) = delete;
		native_function(native_function&& function) noexcept = delete;
		virtual ~native_function() override = default;

	public:
		native_function& operator=(const native_function& function) = delete;
		native_function& operator=(native_function&& function) noexcept = delete;
		bool operator==(const native_function& function) const = delete;
		bool operator!=(const native_function& function) const = delete;

	public:
		virtual function_type type() const noexcept override;

	public:
		const function_t& functor() const noexcept;

	private:
		function_t functor_;
	};
}

#endif