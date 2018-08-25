#include <Aheuiplusplus/command_line.hpp>

#include <cstdlib>

int main(int argc, char** argv)
{
	app::command_line command_line;
	if (!command_line.parse(argc, argv))
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}