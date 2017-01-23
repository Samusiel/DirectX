#pragma once

namespace Math
{
	template <class type>
	class Vector2Base;

	template <class type>
	class Vector3Base : public Vector2Base<type>
	{
	public:
		Vector3Base();
		Vector3Base(type a, type b, type c);
		Vector3Base(const Vector3Base& base);
		virtual ~Vector3Base();

	public:
		type z;
	};

}

