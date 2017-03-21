#include "stdafx.h"
#include "Vector4.h"

namespace Math
{
	template <class type>
	Vector4Base<type>::Vector4Base()
	{
	}

	template <class type>
	Vector4Base<type>::Vector4Base(type a) : Vector3Base(a), w(a)
	{
	}

	template <class type>
	Vector4Base<type>::Vector4Base(type a, type b, type c, type d) : Vector3Base(a, b, c), w(d)
	{
	}

	template <class type>
	Vector4Base<type>::Vector4Base(const Vector4Base& base) : Vector3Base(base), w(base.z)
	{
	}

	template <class type>
	Vector4Base<type>::~Vector4Base()
	{
	}

}