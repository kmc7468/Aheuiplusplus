#include <Aheuiplusplus/cursor.hpp>

namespace app
{
	point::point(std::size_t x, std::size_t y) noexcept
		: x_(x), y_(y)
	{}
	point::point(const app::point& point) noexcept
		: x_(point.x_), y_(point.y_)
	{}

	point& point::operator=(const app::point& point) noexcept
	{
		x_ = point.x_;
		y_ = point.y_;
		
		return *this;
	}
	bool point::operator==(const app::point& point) const noexcept
	{
		return x_ == point.x_ && y_ == point.y_;
	}
	bool point::operator!=(const app::point& point) const noexcept
	{
		return x_ != point.x_ || y_ != point.y_;
	}

	std::size_t point::x() const noexcept
	{
		return x_;
	}
	void point::x(std::size_t new_x) noexcept
	{
		x_ = new_x;
	}
	std::size_t point::y() const noexcept
	{
		return y_;
	}
	void point::y(std::size_t new_y) noexcept
	{
		y_ = new_y;
	}
}

namespace app
{
	cursor::cursor(const app::cursor& cursor) noexcept
		: point_(cursor.point_), direction_(cursor.direction_), speed_(cursor.speed_)
	{}

	cursor& cursor::operator=(const app::cursor& cursor) noexcept
	{
		point_ = cursor.point_;
		direction_ = cursor.direction_;
		speed_ = cursor.speed_;

		return *this;
	}
	bool cursor::operator==(const app::cursor& cursor) const noexcept
	{
		return point_ == cursor.point_ && direction_ == cursor.direction_ &&
			   speed_ == cursor.speed_;
	}
	bool cursor::operator!=(const app::cursor& cursor) const noexcept
	{
		return point_ != cursor.point_ || direction_ != cursor.direction_ ||
			   speed_ != cursor.speed_;
	}

	std::size_t cursor::x() const noexcept
	{
		return point_.x();
	}
	void cursor::x(std::size_t new_x) noexcept
	{
		point_.x(new_x);
	}
	std::size_t cursor::y() const noexcept
	{
		return point_.y();
	}
	void cursor::y(std::size_t new_y) noexcept
	{
		point_.y(new_y);
	}
	direction cursor::direction() const noexcept
	{
		return direction_;
	}
	void cursor::direction(app::direction new_direction) noexcept
	{
		direction_ = new_direction;
	}
	std::size_t cursor::speed() const noexcept
	{
		return speed_;
	}
	void cursor::speed(std::size_t new_speed) noexcept
	{
		speed_ = new_speed;
	}
}