#include <Aheuiplusplus/interpreter.hpp>

#include <Aheuiplusplus/debugger.hpp>

namespace app
{
	interpreter::interpreter(debugger& debugger)
		: debugger_(&debugger)
	{}
	interpreter::interpreter(debugger* debugger)
		: debugger_(debugger)
	{}
}