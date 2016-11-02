#pragma once

class Vector2;

class Vector3 :	public Vector2
{
public:
	Vector3();
	Vector3(float a, float b, float c);
	Vector3(const Vector3& base);
	virtual ~Vector3();

public:
	float z;
};

