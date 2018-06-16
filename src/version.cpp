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
}