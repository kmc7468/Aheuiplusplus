#include <Aheuiplusplus/interpreter.hpp>

namespace app
{
	interpreter::interpreter(app::version version)
		: version_(version)
	{}

	void interpreter::run(const raw_code& code)
	{
		bool is_comment = false;

		for (std::size_t i = 0; i < code.size(); ++i)
		{
			char32_t command = code[i];

			if (!is_comment && command == U'/' && code[i + 1] == U'*')
			{
				is_comment = true;
				++i;
			}
			else if (is_comment && command == U'*' && code[i + 1] == U'/')
			{
				is_comment = false;
				++i;
			}
			else if (!is_comment && is_complete_hangul(command))
			{
				char32_t chosung = get_chosung(command);
				char32_t jungsung = get_jungsung(command);
				char32_t jongsung = get_jongsung(command);

				// TODO
			}
		}
	}

	app::version interpreter::version() const noexcept
	{
		return version_;
	}
	const std::vector<std::vector<app::storage*>>& interpreter::storages() const noexcept
	{
		return storages_;
	}
	const app::storage* interpreter::storage(std::size_t index) const
	{
		return storages_[index][storage_indexs_[index]];
	}
	const std::vector<function*>& interpreter::functions() const noexcept
	{
		return functions_;
	}
	const std::vector<std::size_t>& interpreter::storage_indexs() const noexcept
	{
		return storage_indexs_;
	}
	std::size_t interpreter::storage_index(std::size_t index) const
	{
		return storage_indexs_[index];
	}
}