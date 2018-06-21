#ifndef AHEUIPLUSPLUS_HEADER_VERSION_HPP
#define AHEUIPLUSPLUS_HEADER_VERSION_HPP

namespace app
{
	enum class version
	{
		none = -1,
		v1_0 = 0,
		v1_1,

		latest = v1_1,
	};

	long long get_major(app::version version) noexcept;
	long long get_minor(app::version version) noexcept;
	version get_version(long long major) noexcept;
	version get_version(long long major, long long minor) noexcept;
}

#endif