#pragma once

class Vector3;

class Vector4 :	public Vector3
{
public:
	Vector4();
	Vector4(float a, float b, float c, float d);
	Vector4(const Vector4& base);
	virtual ~Vector4();

public:
	float w;
};

