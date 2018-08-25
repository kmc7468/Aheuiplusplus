#ifndef AHEUIPLUSPLUS_HEADER_MODE_HPP
#define AHEUIPLUSPLUS_HEADER_MODE_HPP

#include <Aheuiplusplus/version.hpp>

namespace app
{
	class mode final
	{
	public:
		mode() noexcept = default;
		mode(bool is_integer_mode, bool is_aheui_compatible_mode) noexcept;
		mode(app::version minimum_version, app::version maximum_version);
		mode(bool is_integer_mode, bool is_aheui_compatible_mode,
			 app::version minimum_version, app::version maximum_version);
		mode(const app::mode& mode) noexcept;
		~mode() = default;

	public:
		mode& operator=(const mode& mode) noexcept;
		bool operator==(const mode& mode) const noexcept;
		bool operator!=(const mode& mode) const noexcept;

	public:
		void reset() noexcept;

	public:
		bool is_integer_mode() const noexcept;
		void is_integer_mode(bool new_is_integer_mode) noexcept;
		bool is_aheui_compatible_mode() const noexcept;
		void is_aheui_compatible_mode(bool new_is_aheui_compatible_mode) noexcept;
		app::version minimum_version() const noexcept;
		void minimum_version(app::version new_minimum_version);
		app::version maximum_version() const noexcept;
		void maximum_version(app::version new_maximum_version);

	private:
		bool is_integer_mode_ = true;
		bool is_aheui_compatible_mode_ = true;
		app::version minimum_version_ = app::version::none;
		app::version maximum_version_ = app::version::latest;
	};
}

#endif