#include <omp.h>

#include <cassert>
#include <cmath>
#include <iostream>

#include "utils.h"

using std::cout, std::endl, std::vector;

template <typename T>
Matrix<T> dotMatrices(const Matrix<T>& a, const Matrix<T>& b) {
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
Matrix<T> dotMatricesWithTranspose(const Matrix<T>& a, const Matrix<T>& b) {
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

template <typename T>
Matrix<T> strassen(const Matrix<T>& a, const Matrix<T>& b) {
	assert((a.size() > 0 && b.size() > 0 && a[0].size() == b.size()) &&
		   "'A' cols num must be equal to 'B' rows num");

	size_t n = a.size();

	if (n <= 64) {
		return dotMatrices(a, b);
	}

	size_t newSize = n / 2;

	Matrix<T> a11(newSize, vector<T>(newSize));
	Matrix<T> a12(newSize, vector<T>(newSize));
	Matrix<T> a21(newSize, vector<T>(newSize));
	Matrix<T> a22(newSize, vector<T>(newSize));

	Matrix<T> b11(newSize, vector<T>(newSize));
	Matrix<T> b12(newSize, vector<T>(newSize));
	Matrix<T> b21(newSize, vector<T>(newSize));
	Matrix<T> b22(newSize, vector<T>(newSize));

	for (size_t i = 0; i < newSize; i++) {
		for (size_t j = 0; j < newSize; j++) {
			a11[i][j] = a[i][j];
			a12[i][j] = a[i][j + newSize];
			a21[i][j] = a[i + newSize][j];
			a22[i][j] = a[i + newSize][j + newSize];

			b11[i][j] = b[i][j];
			b12[i][j] = b[i][j + newSize];
			b21[i][j] = b[i + newSize][j];
			b22[i][j] = b[i + newSize][j + newSize];
		}
	}

	Matrix<T> p1 = strassen(addMatrices(a11, a22), addMatrices(b11, b22));
	Matrix<T> p2 = strassen(addMatrices(a21, a22), b11);
	Matrix<T> p3 = strassen(a11, subMatrices(b12, b22));
	Matrix<T> p4 = strassen(a22, subMatrices(b21, b11));
	Matrix<T> p5 = strassen(addMatrices(a11, a12), b22);
	Matrix<T> p6 = strassen(subMatrices(a21, a11), addMatrices(b11, b12));
	Matrix<T> p7 = strassen(subMatrices(a12, a22), addMatrices(b21, b22));

	Matrix<T> c11 = addMatrices(subMatrices(addMatrices(p1, p4), p5), p7);
	Matrix<T> c12 = addMatrices(p3, p5);
	Matrix<T> c21 = addMatrices(p2, p4);
	Matrix<T> c22 = addMatrices(subMatrices(addMatrices(p1, p3), p2), p6);

	Matrix<T> c(n, vector<T>(n));

	for (size_t i = 0; i < newSize; i++) {
		for (size_t j = 0; j < newSize; j++) {
			c[i][j] = c11[i][j];
			c[i][j + newSize] = c12[i][j];
			c[i + newSize][j] = c21[i][j];
			c[i + newSize][j + newSize] = c22[i][j];
		}
	}

	return c;
}

template <typename T>
Matrix<T> blockMultiply(const Matrix<T>& a, const Matrix<T>& b) {
	assert((a.size() > 0 && b.size() > 0 && a[0].size() == b.size()) &&
		   "'A' cols num must be equal to 'B' rows num, and block size must be greater than 0");

	size_t blockSize = (size_t)std::ceil(::sqrt((double)b.size()));
	blockSize = 100;

	size_t n = a.size();
	size_t m = b[0].size();
	size_t p = a[0].size();

	Matrix<T> c(n, vector<T>(m, 0));

	for (size_t i = 0; i < n; i += blockSize) {
		for (size_t j = 0; j < m; j += blockSize) {
			for (size_t k = 0; k < p; k += blockSize) {
				for (size_t ii = 0; ii < blockSize && i + ii < n; ii++) {
					for (size_t jj = 0; jj < blockSize && j + jj < m; jj++) {
						for (size_t kk = 0; kk < blockSize && k + kk < p; kk++) {
							c[i + ii][j + jj] += a[i + ii][k + kk] * b[k + kk][j + jj];
						}
					}
				}
			}
		}
	}

	return c;
}

template <typename T>
Matrix<T> dotMatricesParallel(const Matrix<T>& a, const Matrix<T>& b) {
	assert((a[0].size() == b.size()) && "'A' cols num must be equal to 'B' rows num");
	size_t n = a.size(), m = b[0].size(), k_max = a[0].size();
	Matrix<T> c = vector(n, vector<T>(m));

#pragma omp parallel for
	for (size_t i = 0; i < n; i++) {
		for (size_t j = 0; j < m; j++) {
			for (size_t k = 0; k < k_max; k++) {
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}

	return c;
}

// int main(int argc, char const *argv[]) {
int main() {
	size_t n = 1000;
	using test_type = unsigned int;

	cout << "Trivial" << endl;
	cout << doTest<test_type>(n, dotMatrices) << endl;

	cout << "Transposed" << endl;
	cout << doTest<test_type>(n, dotMatricesWithTranspose) << endl;

	cout << "Strassen" << endl;
	cout << doTest<test_type>(n, strassen) << endl;

	cout << "Blocks" << endl;
	cout << doTest<test_type>(n, blockMultiply) << endl;

	cout << "Parallel" << endl;
	cout << doTest<test_type>(n, dotMatricesParallel) << endl;
}