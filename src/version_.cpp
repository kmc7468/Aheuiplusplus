#include <Aheuiplusplus/version.hpp>

namespace app
{
	int get_major(app::version version) noexcept
	{
		switch (version)
		{
		case app::version::v1_0:
		case app::version::v1_1:
			return 1;

		case app::version::v2_0:
			return 2;

		default:
			return -1;
		}
	}
	int get_minor(app::version version) noexcept
	{
		switch (version)
		{
		case app::version::v1_0:
		case app::version::v2_0:
			return 0;

		case app::version::v1_1:
			return 1;

		default:
			return -1;
		}
	}
	version get_version(int major) noexcept
	{
		switch (major)
		{
		case 1:
			return version::latest_v1;

		case 2:
			return version::latest_v2;

		default:
			return version::none;
		}
	}
	version get_version(int major, int minor) noexcept
	{
		switch (major)
		{
		case 1:
			switch (minor)
			{
			case 0:
				return version::v1_0;

			case 1:
				return version::v1_1;

			default:
				return version::none;
			}

		case 2:
			switch (minor)
			{
			case 0:
				return version::v2_0;

			default:
				return version::none;
			}

		default:
			return version::none;
		}
	}

	bool operator>(app::version lhs, app::version rhs) noexcept
	{
		return static_cast<int>(lhs) > static_cast<int>(rhs);
	}
	bool operator>=(app::version lhs, app::version rhs) noexcept
	{
		return static_cast<int>(lhs) >= static_cast<int>(rhs);
	}
	bool operator<(app::version lhs, app::version rhs) noexcept
	{
		return static_cast<int>(lhs) < static_cast<int>(rhs);
	}
	bool operator<=(app::version lhs, app::version rhs) noexcept
	{
		return static_cast<int>(lhs) <= static_cast<int>(rhs);
	}
}