#ifndef AHEUIPLUSPLUS_HEADER_CURSOR_HPP
#define AHEUIPLUSPLUS_HEADER_CURSOR_HPP

#include <cstddef>

namespace app
{
	enum class direction
	{
		left,
		right,
		up,
		down,
	};

	class point final
	{
	public:
		point() noexcept = default;
		point(std::size_t x, std::size_t y) noexcept;
		point(const app::point& point) noexcept;
		~point() = default;

	public:
		point& operator=(const app::point& point) noexcept;
		bool operator==(const app::point& point) const noexcept;
		bool operator!=(const app::point& point) const noexcept;

	public:
		std::size_t x() const noexcept;
		void x(std::size_t new_x) noexcept;
		std::size_t y() const noexcept;
		void y(std::size_t new_y) noexcept;

	private:
		std::size_t x_ = 0;
		std::size_t y_ = 0;
	};

	class cursor final
	{
	public:
		cursor() noexcept = default;
		cursor(const app::cursor& cursor) noexcept;
		~cursor() = default;

	public:
		cursor& operator=(const app::cursor& cursor) noexcept;
		bool operator==(const app::cursor& cursor) const noexcept;
		bool operator!=(const app::cursor& cursor) const noexcept;

	public:
		std::size_t x() const noexcept;
		void x(std::size_t new_x) noexcept;
		std::size_t y() const noexcept;
		void y(std::size_t new_y) noexcept;
		app::direction direction() const noexcept;
		void direction(app::direction new_direction) noexcept;
		std::size_t speed() const noexcept;
		void speed(std::size_t new_speed) noexcept;

	private:
		app::point point_;
		app::direction direction_ = app::direction::down;
		std::size_t speed_ = 1;
	};
}

#endif