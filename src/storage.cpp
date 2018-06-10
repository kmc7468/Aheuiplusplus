#include <Aheuiplusplus/storage.hpp>

namespace app
{
	std::size_t storage::virtual_length() const noexcept
	{
		return virtual_length_;
	}
	void storage::virtual_length(std::size_t new_virtual_length) noexcept
	{
		virtual_length_ = new_virtual_length;
	}
}