#include <chrono>
#include <ctime>
#include <iostream>
#include <vector>

using std::cout, std::endl, std::vector, std::rand;

template <typename T>
static unsigned long long calcTime(size_t size) {
	// you may want also measure allocation time
	// auto timeStart = std::chrono::high_resolution_clock::now();

	// init vectors
	// vectors are used because of needing really large memory allocation (arrays can't do that)
	vector<T> a(size), b(size), c(size);
	for (auto &vc : vector{a, b}) {
		for (auto &vci : vc) {
			vci = (T)rand();
		}
	}

	// main algo with time measure
	auto timeStart = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < size; i++) {
		c[i] = a[i] + b[i];
	}
	auto timeEnd = std::chrono::high_resolution_clock::now();
	return (timeEnd - timeStart).count();
}

// int main(int argc, char const *argv[]) {
int main() {
	std::srand((uint32_t)std::time(nullptr));
	// result in ns, divide by 1e9 to get seconds
	cout << (double)calcTime<int>(100'000'000) / 1e9 << endl;
	cout << (double)calcTime<float>(100'000'000) / 1e9 << endl;
	cout << (double)calcTime<double>(100'000'000) / 1e9 << endl;
}