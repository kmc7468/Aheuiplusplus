#include <Aheuiplusplus/cursor.hpp>

namespace app
{
	cursor::cursor(const app::cursor& cursor) noexcept
		: x_(cursor.x_), y_(cursor.y_), direction_(cursor.direction_), speed_(cursor.speed_)
	{}

	cursor& cursor::operator=(const app::cursor& cursor) noexcept
	{
		x_ = cursor.x_;
		y_ = cursor.y_;
		direction_ = cursor.direction_;
		speed_ = cursor.speed_;

		return *this;
	}
	bool cursor::operator==(const app::cursor& cursor) const noexcept
	{
		return x_ == cursor.x_ && y_ == cursor.y_ && direction_ == cursor.direction_ &&
			   speed_ == cursor.speed_;
	}
	bool cursor::operator!=(const app::cursor& cursor) const noexcept
	{
		return x_ != cursor.x_ || y_ != cursor.y_ || direction_ != cursor.direction_ ||
			   speed_ != cursor.speed_;
	}

	std::size_t cursor::x() const noexcept
	{
		return x_;
	}
	void cursor::x(std::size_t new_x) noexcept
	{
		x_ = new_x;
	}
	std::size_t cursor::y() const noexcept
	{
		return y_;
	}
	void cursor::y(std::size_t new_y) noexcept
	{
		y_ = new_y;
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