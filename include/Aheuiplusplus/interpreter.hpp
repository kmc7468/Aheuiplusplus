#ifndef AHEUIPLUSPLUS_HEADER_INTERPRETER_HPP
#define AHEUIPLUSPLUS_HEADER_INTERPRETER_HPP

namespace app
{
#define AHEUIPLUSPLUS_VERSION_STRING "2.0.0"
#define AHEUIPLUSPLUS_VERSION 2
#define AHEUIPLUSPLUS_VERSION_MAJOR AHEUIPLUSPLUS_VERSION
#define AHEUIPLUSPLUS_VERSION_MINOR 0
#define AHEUIPLUSPLUS_VERSION_PATCH 0

	inline constexpr const char* version_string = AHEUIPLUSPLUS_VERSION_STRING;
	inline constexpr int version_major = AHEUIPLUSPLUS_VERSION_MAJOR;
	inline constexpr int version_minor = AHEUIPLUSPLUS_VERSION_MINOR;
	inline constexpr int version_patch = AHEUIPLUSPLUS_VERSION_PATCH;

	class interpreter final
	{
		friend class debugger;
	
	public:
		interpreter() = default;
		interpreter(const interpreter& interpreter) = delete;
		interpreter(interpreter&& interpreter) noexcept = delete;
		~interpreter() = default;

	private:
		interpreter(debugger& debugger);
		interpreter(debugger* debugger);

	public:
		interpreter& operator=(const interpreter& interpreter) = delete;
		interpreter& operator=(interpreter&& interpreter) noexcept = delete;
		bool operator==(const interpreter& interpreter) const = delete;
		bool operator!=(const interpreter& interpreter) const = delete;

	private:
		debugger* const debugger_ = nullptr;
	};
}

#endif