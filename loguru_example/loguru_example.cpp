#include <iostream>

#include "loguru_example.hpp"

#include "../loguru.cpp"

#if defined(BUILD_MONOLITHIC)
#define main loguru_example_main
#endif

extern "C"
int main(int argc, const char** argv)
{
    loguru::init(argc, argv);
    LOG_F(INFO, "Hello from main.cpp!");
    complex_calculation();
    LOG_F(INFO, "main function about to end!");
	return 0;
}
