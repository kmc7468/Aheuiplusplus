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

	int get_major(app::version version) noexcept;
	int get_minor(app::version version) noexcept;
	version get_version(int major) noexcept;
	version get_version(int major, int minor) noexcept;
}

#endif