#if AHEUIPLUSPLUS_TARGET == 1
#include <Aheuiplusplus/Aheuiplusplus.hpp>

#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <ios>
#include <iostream>

int main(int argc, char** argv)
{
#ifdef AHEUIPLUSPLUS_PRINT_BENCHMARK
	std::clog << std::fixed << std::setprecision(10);

	const auto benchmark_timepoint_before_parsing_command_line = std::chrono::high_resolution_clock::now();
#endif

	app::command_line command_line;
	if (!command_line.parse(argc, argv))
	{
		return EXIT_FAILURE;
	}

#ifdef AHEUIPLUSPLUS_PRINT_BENCHMARK
	const auto benchmark_timepoint_after_parsing_command_line = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<long double> benchmark_duration_parsing_command_line =
		benchmark_timepoint_after_parsing_command_line - benchmark_timepoint_before_parsing_command_line;
	std::clog << "[Benchmark] Parsing command line: " << benchmark_duration_parsing_command_line.count() << "s(" <<
		std::chrono::duration_cast<std::chrono::duration<long double, std::milli>>(benchmark_duration_parsing_command_line).count() << "ms, " <<
		std::chrono::duration_cast<std::chrono::duration<long double, std::micro>>(benchmark_duration_parsing_command_line).count() << "µs)\n";
#endif

	return EXIT_SUCCESS;
}

#endif