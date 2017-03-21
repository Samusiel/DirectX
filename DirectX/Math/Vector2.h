#pragma once

namespace Math
{
	template <class type>
	class Vector2Base
	{
	public:
		Vector2Base();
		Vector2Base(type a);
		Vector2Base(type a, type b);
		Vector2Base(const Vector2Base & v2);

		operator type*();
	public:
		type x, y;
	};

}