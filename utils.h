#include <chrono>
#include <ctime>
#include <functional>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

template <typename T>
unsigned long long calcTime(void (*function)()) {
	auto timeStart = std::chrono::high_resolution_clock::now();
	function();
	auto timeEnd = std::chrono::high_resolution_clock::now();
	return (timeEnd - timeStart).count();
}

template <typename T>
static void fillRand(Matrix<T> &matrix) {
	static bool isFuncInitialized = false;
	if (!isFuncInitialized) {
		std::srand((uint32_t)std::time(nullptr));
		isFuncInitialized = true;
	}

	for (auto &row : matrix) {
		for (auto &mi : row) {
			mi = (T)std::rand();
		}
	}
}

template <typename T>
unsigned long long doTest(size_t n, Matrix<T> (*function)(Matrix<T> &a, Matrix<T> &b)) {
	Matrix<T> a(n, std::vector<T>(n)), b(n, std::vector<T>(n));
	fillRand(a);
	fillRand(b);
	auto time = calcTime([]() { function(a, b); });
	return time;
}