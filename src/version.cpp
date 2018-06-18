#include <Aheuiplusplus/version.hpp>

namespace app
{
	long long get_major(app::version version) noexcept
	{
		switch (version)
		{
		case app::version::v1_0:
			return 1;

		default:
			return -1;
		}
	}
	long long get_minor(app::version version) noexcept
	{
		switch (version)
		{
		case app::version::v1_0:
			return 0;

		default:
			return -1;
		}
	}
	version get_version(long long major) noexcept
	{
		return get_version(major, 0);
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
				return app::version::v1_0;
			}
		}

		default:
			return app::version::none;
		}
	}
}