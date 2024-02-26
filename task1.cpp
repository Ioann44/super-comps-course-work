#include <cassert>
#include <chrono>
#include <ctime>
#include <iostream>
#include <vector>

#include "utils.h"

using std::cout, std::endl, std::vector;

template <typename T>
Matrix<T> dotMatrices(const Matrix<T> &a, const Matrix<T> &b) {
	assert((a[0].size() == b.size()) && "'A' cols num must be equal to 'B' rows num");
	size_t n = a.size(), m = b[0].size(), k_max = a[0].size();
	Matrix<T> c = vector(n, vector<T>(m));

	for (size_t i = 0; i < n; i++) {
		for (size_t j = 0; j < m; j++) {
			for (size_t k = 0; k < k_max; k++) {
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}

	return c;
}

template <typename T>
Matrix<T> dotMatricesWithTranspose(const Matrix<T> &a, const Matrix<T> &b) {
	assert((a[0].size() == b.size()) && "'A' cols num must be equal to 'B' rows num");

	Matrix<T> btrans(b[0].size(), vector<T>(b.size()));
	for (size_t i = 0; i < b.size(); i++) {
		for (size_t j = 0; j < b[0].size(); j++) {
			btrans[j][i] = b[i][j];
		}
	}

	size_t n = a.size(), m = btrans[0].size(), k_max = a[0].size();
	Matrix<T> c = vector(n, vector<T>(m));

	for (size_t i = 0; i < n; i++) {
		for (size_t j = 0; j < m; j++) {
			for (size_t k = 0; k < k_max; k++) {
				c[i][j] += a[i][k] * btrans[j][k];
			}
		}
	}

	return c;
}

// int main(int argc, char const *argv[]) {
int main() {
	size_t n = 1000;
	// result in ns, divide by 1e9 to get seconds
	auto timeRes = doTest<unsigned int>(n, dotMatrices);
	cout << timeRes << endl;
	timeRes = doTest<unsigned int>(n, dotMatricesWithTranspose);
	cout << timeRes << endl;
}