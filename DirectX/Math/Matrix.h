#pragma once

namespace Math
{

	class Matrix
	{
	public:
		Matrix() {};
		Matrix(const float *);
		Matrix(const Matrix&);
		Matrix(float _11, float _12, float _13, float _14,
			float _21, float _22, float _23, float _24,
			float _31, float _32, float _33, float _34,
			float _41, float _42, float _43, float _44);
		//--
		void transpose();

		//--
		void operator+=(const Matrix& matrix);
		void operator-=(const Matrix& matrix);
		void operator*=(const Matrix& matrix);

		//--
		friend Matrix operator+(const Matrix& m1, const Matrix& m2);
		friend Matrix operator-(const Matrix& m1, const Matrix& m2);
		friend Matrix operator*(const Matrix& m1, const Matrix& m2);

	public:
		union {
			struct {
				float        _11, _12, _13, _14;
				float        _21, _22, _23, _24;
				float        _31, _32, _33, _34;
				float        _41, _42, _43, _44;

			};
			float m[4][4];
		};
	};

}

