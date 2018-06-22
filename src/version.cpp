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

		default:
			return -1;
		}
	}
	int get_minor(app::version version) noexcept
	{
		switch (version)
		{
		case app::version::v1_0:
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
			return version::v1_1;

		default:
			return version::none;
		}
	}
	version get_version(int major, int minor) noexcept
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
			}
		}

		default:
			return version::none;
		}
	}
}