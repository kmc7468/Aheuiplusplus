#include <Aheuiplusplus/version.hpp>

namespace app
{
	long long get_major(app::version version) noexcept
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
	long long get_minor(app::version version) noexcept
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
	version get_version(long long major) noexcept
	{
		switch (major)
		{
		case 1:
			return version::v1_1;

		case 2:
			return version::v2_0;

		default:
			return version::none;
		}
	}
	version get_version(long long major, long long minor) noexcept
	{
		switch (major)
		{
		case 1:
		{
			switch (minor)
			{
			case 0:
				return version::v1_0;

			case 1:
				return version::v1_1;

			default:
				return version::none;
			}
		}

		case 2:
		{
			switch (minor)
			{
			case 0:
				return version::v2_0;

			default:
				return version::none;
			}
		}

		default:
			return version::none;
		}
	}

	bool is_later(app::version lhs, app::version rhs) noexcept
	{
		return static_cast<long long>(lhs) <= static_cast<long long>(rhs);
	}
}