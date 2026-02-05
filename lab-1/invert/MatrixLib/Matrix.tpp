#pragma once
#include "ranges"
#include <algorithm>
#include <iomanip>

namespace MatrixLib
{
template <typename T>
Matrix<T>::Matrix(const size_t rows, const size_t columns)
	: m_rows{ rows }
, m_columns{ columns }
, m_data(rows * columns, 0)
{
}

template <typename T>
size_t Matrix<T>::getRows() const
{
	return m_rows;
}

template <typename T>
size_t Matrix<T>::getColumns() const
{
	return m_columns;
}

template <typename T>
const T& Matrix<T>::operator()(const size_t row, const size_t column) const
{
	if (row >= m_rows || column >= m_columns)
	{
		throw std::out_of_range("Matrix index out of bounds");
	}

	return m_data[row * m_columns + column];
}

template <typename T>
T& Matrix<T>::operator()(const size_t row, const size_t column)
{
	return const_cast<T&>(const_cast<const Matrix*>(this)->operator()(row, column));
}

template <typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix& rhs)
{
	if (m_rows != rhs.m_rows || m_columns != rhs.m_columns)
	{
		throw std::invalid_argument("Failed to sum matrices!");
	}

	std::transform(m_data.cbegin(), m_data.cend(),
		rhs.m_data.сbegin(),
		m_data.begin(),
		std::plus{});

	return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator-=(const Matrix& rhs)
{
	if (m_rows != rhs.m_rows || m_columns != rhs.m_columns)
	{
		throw std::invalid_argument("Failed to subtract matrices!");
	}

	std::transform(m_data.cbegin(), m_data.cend(),
		rhs.m_data.сbegin(),
		m_data.begin(),
		std::minus{});

	return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator*=(const Matrix& rhs)
{
	if (m_columns != rhs.m_rows)
	{
		throw std::invalid_argument("Failed to multiply matrices!");
	}

	Matrix result{ m_rows, rhs.m_columns };
	for (ssize_t i = 0; i < result.m_rows; ++i)
	{
		for (ssize_t j = 0; j < result.m_columns; ++j)
		{
			T sum{};
			for (ssize_t k = 0; k < m_columns; ++k)
			{
				sum += (*this)(i, k) * rhs(k, j);
			}

			result(i, j) = sum;
		}
	}

	*this = result;
	return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator*=(const T& scalar)
{
	std::for_each(m_data.begin(), m_data.end(), [&](T& x) { x *= scalar; });
	return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator/=(const T& scalar)
{
	std::for_each(m_data.begin(), m_data.end(), [&](T& x) { x /= scalar; });
	return *this;
}

template <typename T>
template <typename U>
bool Matrix<T>::operator==(const Matrix<U>& rhs) const
{
	if (m_rows != rhs.m_rows || m_columns != rhs.m_columns)
	{
		return false;
	}

	return std::equal(m_data.begin(), m_data.end(), rhs.m_data.begin());
}

template <typename T>
Matrix<T> Matrix<T>::transpose() const
{
	Matrix result(m_columns, m_rows);
	for (size_t i = 0; i < m_rows; ++i)
	{
		for (size_t j = 0; j < m_columns; ++j)
		{
			result(j, i) = (*this)(i, j);
		}
	}

	return result;
}

template <typename T>
T Matrix<T>::determinant() const
{
	if (m_rows != m_columns)
	{
		throw std::invalid_argument("Determinant is defined only for square matrices.");
	}

	const auto n = m_rows;

	Matrix temp(*this);
	T det = 1;

	for (size_t k = 0; k < n; ++k)
	{
		size_t maxRow = k;
		T maxVal = std::abs(temp(k, k));
		for (size_t i = k + 1; i < n; ++i)
		{
			if (std::abs(temp(i, k)) > maxVal)
			{
				maxVal = std::abs(temp(i, k));
				maxRow = i;
			}
		}

		if (maxVal == 0)
		{
			return 0;
		}

		if (maxRow != k)
		{
			for (size_t j = 0; j < n; ++j)
			{
				std::swap(temp(k, j), temp(maxRow, j));
			}
			det = -det;
		}

		det *= temp(k, k);

		for (size_t i = k + 1; i < n; ++i)
		{
			T factor = temp(i, k) / temp(k, k);
			for (size_t j = k; j < n; ++j)
			{
				temp(i, j) -= factor * temp(k, j);
			}
		}
	}

	return det;
}

template <typename T>
Matrix<T> Matrix<T>::inverse() const
{
	if (m_rows != m_columns)
	{
		throw std::invalid_argument("Matrix must be square");
	}

	if (this->determinant() <= 0)
	{
		throw std::runtime_error("Matrix is singular and cannot be inverted");
	}

	const size_t n = m_rows;
	Matrix augmented(n, n * 2);

	for (size_t i = 0; i < n; ++i)
	{
		for (size_t j = 0; j < n; ++j)
		{
			augmented(i, j) = (*this)(i, j);
		}
		augmented(i, n + i) = 1;
	}

	for (size_t k = 0; k < n; ++k)
	{
		size_t maxRow = k;
		for (size_t i = k + 1; i < n; ++i)
		{
			if (std::abs(augmented(i, k)) > std::abs(augmented(maxRow, k)))
			{
				maxRow = i;
			}
		}

		if (maxRow != k)
		{
			for (size_t j = 0; j < 2 * n; ++j)
			{
				std::swap(augmented(k, j), augmented(maxRow, j));
			}
		}

		T pivot = augmented(k, k);
		for (size_t j = 0; j < 2 * n; ++j)
		{
			augmented(k, j) /= pivot;
		}

		for (size_t i = 0; i < n; ++i)
		{
			if (i != k)
			{
				T factor = augmented(i, k);
				for (size_t j = 0; j < 2 * n; ++j)
				{
					augmented(i, j) -= factor * augmented(k, j);
				}
			}
		}
	}

	Matrix inversed(n, n);
	for (size_t i = 0; i < n; ++i)
	{
		for (size_t j = 0; j < n; ++j)
		{
			inversed(i, j) = augmented(i, n + j);
		}
	}

	return inversed;
}

template <typename T>
Matrix<T> operator+(const Matrix<T>& lhs, const Matrix<T>& rhs)
{
	Matrix<T> result = lhs;
	result += rhs;
	return result;
}

template <typename T>
Matrix<T> operator-(const Matrix<T>& lhs, const Matrix<T>& rhs)
{
	Matrix<T> result = lhs;
	result -= rhs;
	return result;
}

template <typename T>
Matrix<T> operator*(const Matrix<T>& lhs, const Matrix<T>& rhs)
{
	Matrix<T> result = lhs;
	result *= rhs;
	return result;
}

template <typename T>
Matrix<T> operator*(const Matrix<T>& m, const T& scalar)
{
	Matrix<T> result = m;
	result *= scalar;
	return result;
}

template <typename T>
Matrix<T> operator*(const T& scalar, const Matrix<T>& m)
{
	return m * scalar;
}

template <typename T>
Matrix<T> operator/(const Matrix<T>& m, const T& scalar)
{
	Matrix<T> result = m;
	result /= scalar;
	return result;
}

template <typename T>
Matrix<T> operator-(const Matrix<T>& m)
{
	Matrix<T> result = m;
	for (size_t i = 0; i < result.getRows(); ++i)
	{
		for (size_t j = 0; j < result.getColumns(); ++j)
		{
			result(i, j) = -result(i, j);
		}
	}

	return result;
}

template <typename T, typename U>
bool operator!=(const Matrix<T>& lhs, const Matrix<U>& rhs)
{
	return !(lhs == rhs);
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const Matrix<T>& m)
{
	out << std::fixed << std::setprecision(3);

	for (size_t i = 0; i < m.getRows(); ++i)
	{
		for (size_t j = 0; j < m.getColumns(); ++j)
		{
			out << std::setw(8) << m(i, j) << (j + 1 == m.getColumns() ? '\n' : ' ');
		}
	}

	return out;
}

template <typename T>
std::istream& operator>>(std::istream& in, Matrix<T>& m)
{
	for (size_t i = 0; i < m.getRows(); ++i)
	{
		for (size_t j = 0; j < m.getColumns(); ++j)
		{
			T value;
			if (!(in >> value))
			{
				throw std::runtime_error("Invalid input for matrix element");
			}
			m(i, j) = value;
		}
	}
	return in;
}

} // namespace MatrixLib