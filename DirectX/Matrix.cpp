#include "stdafx.h"
#include "Matrix.h"
#include <emmintrin.h>

//------------------------------------------------------------------------------------
Matrix::Matrix(const float * matrix)
{
	for (size_t i = 0; i < 4; ++i)
		_mm_store_ps(m[i], *((__m128*)&matrix[4 * i]));
}

//------------------------------------------------------------------------------------
Matrix::Matrix(const Matrix& matrix)
{
	for (size_t i = 0; i < 4; ++i)
		_mm_store_ps(m[i], *((__m128*)matrix.m[i]));
}

//------------------------------------------------------------------------------------
Matrix::Matrix(	float f11, float f12, float f13, float f14,
				float f21, float f22, float f23, float f24,
				float f31, float f32, float f33, float f34,
				float f41, float f42, float f43, float f44)
{
	_11 = f11; _12 = f12; _13 = f13; _14 = f14;
	_21 = f21; _22 = f22; _23 = f23; _24 = f24;
	_31 = f31; _32 = f32; _33 = f33; _34 = f34;
	_41 = f41; _42 = f42; _43 = f43; _44 = f44;
}

//------------------------------------------------------------------------------------
void Matrix::operator+=(const Matrix& matrix)
{
	for (int i = 0; i < 4; ++i)
	{
		__m128* first = (__m128*)m[i];
		__m128* second = (__m128*)matrix.m[i];
		_mm_store_ps(m[i], _mm_add_ps(*first, *second));
	}
}

//------------------------------------------------------------------------------------
void Matrix::operator-=(const Matrix& matrix)
{
	for (int i = 0; i < 4; ++i)
	{
		__m128* first = (__m128*)m[i];
		__m128* second = (__m128*)matrix.m[i];
		_mm_store_ps(m[i], _mm_sub_ps(*first, *second));
	}
}

//------------------------------------------------------------------------------------
void Matrix::operator*=(const Matrix& matrix)
{
	auto tempMatrix = m;
	__m128 row1 = _mm_load_ps(matrix.m[0]);
	__m128 row2 = _mm_load_ps(matrix.m[1]);
	__m128 row3 = _mm_load_ps(matrix.m[2]);
	__m128 row4 = _mm_load_ps(matrix.m[3]);
	for (int i = 0; i < 4; ++i)
	{
		__m128 brod1 = _mm_set1_ps(tempMatrix[i][0]);
		__m128 brod2 = _mm_set1_ps(tempMatrix[i][1]);
		__m128 brod3 = _mm_set1_ps(tempMatrix[i][2]);
		__m128 brod4 = _mm_set1_ps(tempMatrix[i][3]);
		__m128 row = _mm_add_ps(
			_mm_add_ps(
				_mm_mul_ps(brod1, row1),
				_mm_mul_ps(brod2, row2)),
			_mm_add_ps(
				_mm_mul_ps(brod3, row3),
				_mm_mul_ps(brod4, row4)));
		_mm_store_ps(m[i], row);
	}
}

//------------------------------------------------------------------------------------
void Matrix::transpose()
{
	__m128 row1 = _mm_load_ps(m[0]);
	__m128 row2 = _mm_load_ps(m[1]);
	__m128 row3 = _mm_load_ps(m[2]);
	__m128 row4 = _mm_load_ps(m[3]);
	_MM_TRANSPOSE4_PS(row1, row2, row3, row4);
	_mm_store_ps(m[0], row1);
	_mm_store_ps(m[1], row2);
	_mm_store_ps(m[2], row3);
	_mm_store_ps(m[3], row4);
}

//------------------------------------------------------------------------------------
Matrix operator+(const Matrix& m1, const Matrix& m2)
{
	Matrix m = m1;
	m += m2;
	return m;
}

//------------------------------------------------------------------------------------
Matrix operator-(const Matrix& m1, const Matrix& m2)
{
	Matrix m = m1;
	m += m2;
	return m;
}

//------------------------------------------------------------------------------------
Matrix operator*(const Matrix& m1, const Matrix& m2)
{
	Matrix m = m1;
	m *= m2;
	return m;
}