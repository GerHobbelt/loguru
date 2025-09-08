#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>

#include <glog/logging.h>
#include <gflags/gflags.h>
#include <glog/raw_logging.h>

static const size_t kNumIterations = 50 * 1000;
static const size_t kNumRuns = 10;
static const double kPi = 3.1415926535897932384626433;

static long long now_ns()
{
	using namespace std::chrono;
	return duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();
}

template<typename Function>
static double time_sec(const Function& function)
{
	auto start_ns = now_ns();
	function();
	return (now_ns() - start_ns) * 1e-9;
}

template<typename Function>
static void bench(const std::string& name, const Function& function)
{
	function(); // Warm-up

	printf("%-30s ", name.c_str());
	fflush(stdout);
	std::vector<double> times;
	double sum = 0;
	for (size_t i = 0; i < kNumRuns; ++i)
	{
		times.push_back(time_sec(function) / kNumIterations);
		sum += times.back();
	}

	double mean = sum / kNumRuns;
	double std_dev_sum = 0;

	for (double time : times) {
		std_dev_sum += (time - mean) * (time - mean);
	}

	double variance = std::sqrt(std_dev_sum / kNumRuns);

	printf("%6.3f +- %.3f us per call\n", mean * 1e6, variance * 1e6);
	fflush(stdout);
}

// ----------------------------------------------------------------------------

static void stream_strings(void)
{
	for (size_t i = 0; i < kNumIterations; ++i) {
		LOG(WARNING) << "Some long, complex message.";
	}
	google::FlushLogFiles(google::GLOG_INFO);
}

static void stream_float(void)
{
	for (size_t i = 0; i < kNumIterations; ++i) {
		LOG(WARNING) << std::setfill('0') << std::setw(6) << std::setprecision(3) << kPi;
	}
	google::FlushLogFiles(google::GLOG_INFO);
}

static void raw_string_float(void)
{
	for (size_t i = 0; i < kNumIterations; ++i) {
		RAW_LOG(WARNING, "Some long, complex message.");
	}
	google::FlushLogFiles(google::GLOG_INFO);
}

#if defined(BUILD_MONOLITHIC)
#define main loguru_glog_benchmark_main
#endif

extern "C"
int main(int argc, const char** argv)
{
    FLAGS_alsologtostderr = true;
	gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);

    bench("LOG(WARNING) << string (buffered):", stream_strings);
    bench("LOG(WARNING) << float  (buffered):", stream_float);

    FLAGS_logbufsecs = 0; // Flush all output in realtime
    bench("LOG(WARNING) << string (unbuffered):", stream_strings);
    bench("LOG(WARNING) << float  (unbuffered):", stream_float);
	return 0;
}
