#include <Aheuiplusplus/mode.hpp>

#include <stdexcept>

namespace app
{
	mode::mode(bool is_integer_mode, bool is_aheui_compatible_mode) noexcept
		: is_integer_mode_(is_integer_mode), is_aheui_compatible_mode_(is_aheui_compatible_mode)
	{}
	mode::mode(app::version minimum_version, app::version maximum_version)
		: minimum_version_(minimum_version), maximum_version_(maximum_version)
	{
		if (minimum_version > maximum_version)
			throw std::invalid_argument("�μ� minimum_version�� �μ� maximum_version���� ���� �����̿��� �մϴ�.");
	}
	mode::mode(bool is_integer_mode, bool is_aheui_compatible_mode,
			   app::version minimum_version, app::version maximum_version)
		: is_integer_mode_(is_integer_mode), is_aheui_compatible_mode_(is_aheui_compatible_mode),
		  minimum_version_(minimum_version), maximum_version_(maximum_version)
	{
		if (minimum_version > maximum_version)
			throw std::invalid_argument("�μ� minimum_version�� �μ� maximum_version���� ���� �����̿��� �մϴ�.");
	}
	mode::mode(const app::mode& mode) noexcept
		: is_integer_mode_(mode.is_integer_mode_), is_aheui_compatible_mode_(mode.is_aheui_compatible_mode_),
		  minimum_version_(mode.minimum_version_), maximum_version_(mode.maximum_version_)
	{}

	mode& mode::operator=(const mode& mode) noexcept
	{
		is_integer_mode_ = mode.is_integer_mode_;
		is_aheui_compatible_mode_ = mode.is_aheui_compatible_mode_;
		minimum_version_ = mode.minimum_version_;
		maximum_version_ = mode.maximum_version_;

		return *this;
	}
	bool mode::operator==(const mode& mode) const noexcept
	{
		return is_integer_mode_ == mode.is_integer_mode_ &&
			   is_aheui_compatible_mode_ == mode.is_aheui_compatible_mode_ &&
			   minimum_version_ == mode.minimum_version_ &&
			   maximum_version_ == mode.maximum_version_;
	}
	bool mode::operator!=(const mode& mode) const noexcept
	{
		return is_integer_mode_ != mode.is_integer_mode_ ||
			   is_aheui_compatible_mode_ != mode.is_aheui_compatible_mode_ ||
			   minimum_version_ != mode.minimum_version_ ||
			   maximum_version_ != mode.maximum_version_;
	}

	bool mode::is_integer_mode() const noexcept
	{
		return is_integer_mode_;
	}
	void mode::is_integer_mode(bool new_is_integer_mode) noexcept
	{
		is_integer_mode_ = new_is_integer_mode;
	}
	bool mode::is_aheui_compatible_mode() const noexcept
	{
		return is_aheui_compatible_mode_;
	}
	void mode::is_aheui_compatible_mode(bool new_is_aheui_compatible_mode) noexcept
	{
		is_aheui_compatible_mode_ = new_is_aheui_compatible_mode;
	}
	app::version mode::minimum_version() const noexcept
	{
		return minimum_version_;
	}
	void mode::minimum_version(app::version new_minimum_version)
	{
		minimum_version_ = new_minimum_version;
		
		if (maximum_version_ < new_minimum_version)
			throw std::invalid_argument("�μ� new_minimum_version�� �ʵ� maximum_version���� ���� �����̿��� �մϴ�.");
	}
	app::version mode::maximum_version() const noexcept
	{
		return maximum_version_;
	}
	void mode::maximum_version(app::version new_maximum_version)
	{
		maximum_version_ = new_maximum_version;

		if (minimum_version_ > new_maximum_version)
			throw std::invalid_argument("�μ� new_maximum_version�� �ʵ� minimum_version���� ���� �����̿��� �մϴ�.");
	}
}