#include "stdafx.h"
#include "Vector4.h"


Vector4::Vector4()
{
}

Vector4::Vector4(float a, float b, float c, float d) : Vector3(a, b, c), w(d)
{
}

Vector4::Vector4(const Vector4& base) : Vector3(base), w(base.z)
{
}

Vector4::~Vector4()
{
}
