#ifdef AHEUIPLUSPLUS_USE_EXTENSION
#include <Aheuiplusplus/extension.hpp>

#include <Aheuiplusplus/interpreter.hpp>

#include <algorithm>
#include <cstdio>
#include <stdexcept>
#include <string_view>

#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
#	include <Windows.h>
#else
#	include <dlfcn.h>
#endif

namespace app
{
	std::u32string extension::description() const
	{
		return std::u32string();
	}

	void extension::enabled()
	{}
	void extension::disabled()
	{}

	std::tuple<int, int, int> extension::target_version() const
	{
		return { version_major, version_minor, version_patch };
	}
}

namespace app
{
	extension_engine::extension_engine(const std::vector<std::string>& extensions)
	{
		for (const std::string& path : extensions)
		{
			using allocate_function = void*();

#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
			HMODULE extension_module = LoadLibraryA(path.c_str());
			if (!extension_module)
				throw std::runtime_error("인수 extensions 중 '" + path + "'를 여는데 실패했습니다.");
			
			const std::map<void*, extension*>::iterator iter =
				std::find_if(extensions_.begin(), extensions_.end(), [extension_module](const auto& data)
			{
				return data.first == extension_module;
			});
			if (iter != extensions_.end())
				throw std::runtime_error("인수 extensions 중 '" + path + "'가 두번 이상 등록되었습니다.");

			allocate_function* allocate = reinterpret_cast<allocate_function*>(
				GetProcAddress(extension_module, "allocate_extension"));
			if (!allocate)
			{
				FreeLibrary(extension_module);
				throw std::runtime_error("인수 extensions 중 '" + path + "'는 올바른 아희++ 표준 인터프리터 확장이 아닙니다.");
			}

			extension* extension = reinterpret_cast<app::extension*>(allocate());
			if (!extension)
			{
				FreeLibrary(extension_module);
				throw std::runtime_error("인수 extensions 중 '" + path + "'를 등록하는데 실패했습니다.");
			}

			extensions_[extension_module] = extension;
			extension->enabled();
#else
			void* extension_module = dlopen(path.c_str(), RTLD_LAZY);
			if (!extension_module)
				throw std::runtime_error("인수 extensions 중 '" + path + "'를 여는데 실패했습니다.");

			const std::map<void*, extension*>::iterator iter =
				std::find_if(extensions_.begin(), extensions_.end(), [extension_module](const auto& data)
			{
				return data.first == extension_module;
			});
			if (iter != extensions_.end())
				throw std::runtime_error("인수 extensions 중 '" + path + "'가 두번 이상 등록되었습니다.");

			allocate_function* allocate = reinterpret_cast<allocate_function*>(
				dlsym(extension_module, "allocate_extension"));
			if (!allocate)
			{
				dlclose(extension_module);
				throw std::runtime_error("인수 extensions 중 '" + path + "'는 올바른 아희++ 표준 인터프리터 확장이 아닙니다.");
			}

			extension* extension = reinterpret_cast<app::extension*>(allocate());
			if (!extension)
			{
				dlclose(extension_module);
				throw std::runtime_error("인수 extensions 중 '" + path + "'를 등록하는데 실패했습니다.");
			}

			extensions_[extension_module] = extension;
			extension->enabled();
#endif
		}
	}
	extension_engine::~extension_engine()
	{
		for (const std::pair<void*, extension*>& extension : extensions_)
		{
			extension.second->disabled();
			delete extension.second;

#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
			FreeLibrary(reinterpret_cast<HMODULE>(extension.first));
#else
			dlclose(extension.first);
#endif
		}
	}
}

#endif