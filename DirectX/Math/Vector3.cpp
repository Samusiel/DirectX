#include "Vector3.h"

namespace Math
{
	template <class type>
	Vector3Base<type>::Vector3Base()
	{
	}

	template <class type>
	Vector3Base<type>::Vector3Base(type a) : Vector2Base(a), z(a)
	{
	}

	template <class type>
	Vector3Base<type>::Vector3Base(type a, type b, type c) : Vector2Base(a, b), z(c)
	{
	}

	template <class type>
	Vector3Base<type>::Vector3Base(const Vector3Base& base) : Vector2Base(base), z(base.z)
	{
	}

	template <class type>
	Vector3Base<type>::~Vector3Base()
	{
	}

}
