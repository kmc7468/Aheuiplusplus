#ifndef AHEUIPLUSPLUS_HEADER_VERSION_HPP
#define AHEUIPLUSPLUS_HEADER_VERSION_HPP

namespace app
{
	enum class version
	{
		v1_0,
	};

	long long get_major(app::version version) noexcept;
}

#endif