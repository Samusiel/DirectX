#include "stdafx.h"
#include "Vector2.h"

Vector2::Vector2() : x(0), y(0)
{
}

Vector2::Vector2(float a, float b) : x(a), y(b)
{
}

Vector2::Vector2(const Vector2 & v2) : x(v2.x), y(v2.y)
{	
}

