#include "Vector2.h"

namespace Math
{
	template <class type>
	Vector2Base<type>::Vector2Base() : x(0), y(0)
	{
	}

	template <class type>
	Vector2Base<type>::Vector2Base(type a, type b) : x(a), y(b)
	{
	}

	template <class type>
	Vector2Base<type>::Vector2Base(const Vector2Base & v2) : x(v2.x), y(v2.y)
	{
	}

}

