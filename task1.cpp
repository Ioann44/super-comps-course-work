#include <cassert>
#include <chrono>
#include <ctime>
#include <iostream>
#include <vector>

#include "utils.h"

using std::cout, std::endl, std::vector;

template <typename T>
Matrix<T> dotMatrices(Matrix<T> &a, Matrix<T> &b) {
	assert((a[0].size() == b.size()) && "'A' cols num must be equal to 'B' rows num");
	size_t n = a.size(), m = b[0].size(), k_max = a[0].size();
	Matrix<T> c = vector(n, vector<T>(m));

	for (size_t i = 0; i < n; i++) {
		for (size_t j = 0; j < m; j++) {
			for (size_t k = 0; k < k_max; k++) {
				c[i][j] += (unsigned long long)a[i][k] * b[k][j];
			}
		}
	}

	return c;
}

// int main(int argc, char const *argv[]) {
int main() {
	// result in ns, divide by 1e9 to get seconds
	auto timeRes = doTest<int>(100, dotMatrices);
	cout << (double)timeRes / 1e9 << endl;
}