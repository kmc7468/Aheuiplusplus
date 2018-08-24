#ifndef AHEUIPLUSPLUS_HEADER_VERSION_HPP
#define AHEUIPLUSPLUS_HEADER_VERSION_HPP

namespace app
{
	enum class version
	{
		none = -1,
		v1_0 = 0,
		v1_1,
		v2_0,

		latest = v2_0,
	};

	int get_major(app::version version) noexcept;
	int get_minor(app::version version) noexcept;
	version get_version(int major, int minor) noexcept;
	version get_version(int major) noexcept;
	bool is_later(app::version lhs, app::version rhs) noexcept;
}

#endif