#include <Aheuiplusplus/memory.hpp>

#include <stdexcept>

namespace app
{
	bool is_little_endian()
	{
		static bool is_little_endian;
		static bool is_first = true;

		if (is_first)
		{
			is_first = false;
			std::uint32_t test = 1;

			is_little_endian = *reinterpret_cast<std::uint8_t*>(&test) == 1;

			if (!is_little_endian && *(reinterpret_cast<std::uint8_t*>(&test) + 3) != 1)
			{
				is_first = true;
				throw std::runtime_error("�� ����� �Ǵ� ��Ʋ ������� ����ϴ� �÷����� �����մϴ�.");
			}
		}

		return is_little_endian;
	}
}