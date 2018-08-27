#ifndef AHEUIPLUSPLUS_HEADER_VERSION_HPP
#define AHEUIPLUSPLUS_HEADER_VERSION_HPP

namespace app
{
	enum class version
	{
		none,

		v1_0,
		v1_1,

		v2_0,

		latest_v1 = v1_1,
		latest_v2 = v2_0,
		latest = latest_v2,
	};

	int get_major(app::version version) noexcept;
	int get_minor(app::version version) noexcept;
	version get_version(int major) noexcept;
	version get_version(int major, int minor) noexcept;

	bool operator>(app::version lhs, app::version rhs) noexcept;
	bool operator>=(app::version lhs, app::version rhs) noexcept;
	bool operator<(app::version lhs, app::version rhs) noexcept;
	bool operator<=(app::version lhs, app::version rhs) noexcept;
}

#endif