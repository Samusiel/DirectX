#pragma once

class Vector2
{
public:
	Vector2();
	Vector2(float a, float b);
	explicit Vector2(const Vector2 & v2);

	operator float*();
public:
	float x, y;
};