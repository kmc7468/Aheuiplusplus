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
	std::size_t& point::x() noexcept
	{
		return x_;
	}
	std::size_t point::y() const noexcept
	{
		return y_;
	}
	std::size_t& point::y() noexcept
	{
		return y_;
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
	std::size_t& cursor::x() noexcept
	{
		return point_.x();
	}
	std::size_t cursor::y() const noexcept
	{
		return point_.y();
	}
	std::size_t& cursor::y() noexcept
	{
		return point_.y();
	}
	direction cursor::direction() const noexcept
	{
		return direction_;
	}
	direction& cursor::direction() noexcept
	{
		return direction_;
	}
	std::size_t cursor::speed() const noexcept
	{
		return speed_;
	}
	std::size_t& cursor::speed() noexcept
	{
		return speed_;
	}
}