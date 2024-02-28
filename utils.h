#include <chrono>
#include <ctime>
#include <functional>
#include <vector>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

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
Matrix<T> addMatrices(const Matrix<T> &a, const Matrix<T> &b) {
	size_t rows = a.size();
	size_t cols = a[0].size();
	assert(rows == b.size() && cols == b[0].size() && "Matrices must have the same dimensions");

	Matrix<T> result(rows, std::vector<T>(cols));

	for (size_t i = 0; i < rows; ++i) {
		for (size_t j = 0; j < cols; ++j) {
			result[i][j] = a[i][j] + b[i][j];
		}
	}

	return result;
}

template <typename T>
Matrix<T> subMatrices(const Matrix<T> &a, const Matrix<T> &b) {
	size_t rows = a.size();
	size_t cols = a[0].size();
	assert(rows == b.size() && cols == b[0].size() && "Matrices must have the same dimensions");

	Matrix<T> result(rows, std::vector<T>(cols));

	for (size_t i = 0; i < rows; ++i) {
		for (size_t j = 0; j < cols; ++j) {
			result[i][j] = a[i][j] - b[i][j];
		}
	}

	return result;
}

template <typename T>
double doTest(size_t n, Matrix<T> (*function)(const Matrix<T> &a, const Matrix<T> &b)) {
	static bool isFuncInitialized = false;
	static Matrix<T> a(n, std::vector<T>(n)), b(n, std::vector<T>(n));
	if (!isFuncInitialized) {
		fillRand(a);
		fillRand(b);
		isFuncInitialized = true;
	}

	auto timeStart = std::chrono::high_resolution_clock::now();
	auto resultMatrix = function(a, b);
	auto timeEnd = std::chrono::high_resolution_clock::now();
	return (double)(timeEnd - timeStart).count() / 1e9;
}