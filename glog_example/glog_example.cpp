#include <iostream>
#include <gflags/gflags.h>

#include "glog_example.hpp"

int main(int argc, const char** argv)
{
    FLAGS_alsologtostderr = true;
    FLAGS_colorlogtostderr = true;
	gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);
    LOG(INFO) << "Hello from main.cpp!";
    complex_calculation();
    LOG(INFO) << "main function about to end!";
}
