#include "../stdafx.h"
#include "Vector3.h"

namespace Math
{

	Vector3::Vector3()
	{
	}

	Vector3::Vector3(float a, float b, float c) : Vector2(a, b), z(c)
	{
	}

	Vector3::Vector3(const Vector3& base) : Vector2(base), z(base.z)
	{
	}

	Vector3::~Vector3()
	{
	}

}
