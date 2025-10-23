#pragma once
#include <iostream>
#include <vector>

namespace MatrixLib
{

template <typename T>
class Matrix
{
public:
	Matrix(size_t rows, size_t columns);
	// Matrix(ssize_t rows, ssize_t columns, const T* data);
	Matrix(std::initializer_list<std::initializer_list<T>> list);

	[[nodiscard]] size_t getRows() const;
	[[nodiscard]] size_t getColumns() const;

	const T& operator()(ssize_t row, ssize_t column) const;
	T& operator()(ssize_t row, ssize_t column);

	Matrix& operator+=(const Matrix& rhs);
	Matrix& operator-=(const Matrix& rhs);
	Matrix& operator*=(const Matrix& rhs);
	Matrix& operator*=(const T& scalar);
	Matrix& operator/=(const T& scalar);

	template <typename U>
	bool operator==(const Matrix<U>& rhs) const;

	Matrix transpose() const;
	T determinant() const;
	Matrix inverse() const;

private:
	size_t m_rows, m_columns;
	std::vector<T> m_data;
};

template <typename T>
Matrix<T> operator+(const Matrix<T>& lhs, const Matrix<T>& rhs);

template <typename T>
Matrix<T> operator-(const Matrix<T>& lhs, const Matrix<T>& rhs);

template <typename T>
Matrix<T> operator*(const Matrix<T>& lhs, const Matrix<T>& rhs);

template <typename T>
Matrix<T> operator*(const Matrix<T>& m, const T& scalar);

template <typename T>
Matrix<T> operator*(const T& scalar, const Matrix<T>& m);

template <typename T>
Matrix<T> operator/(const Matrix<T>& m, const T& scalar);

template <typename T>
Matrix<T> operator-(const Matrix<T>& m);

template <typename T1, typename T2>
bool operator!=(const Matrix<T1>& lhs, const Matrix<T2>& rhs);

template <typename T>
std::ostream& operator<<(std::ostream& out, const Matrix<T>& m);

template <typename T>
std::istream& operator>>(std::istream& in, Matrix<T>& m);

} // namespace MatrixLib

#include "Matrix.tpp"