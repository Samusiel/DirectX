#pragma once


namespace Math
{
	template <class type>
	class Vector3Base;

	template <class type>
	class Vector4Base : public Vector3Base<type>
	{
	public:
		Vector4Base();
		Vector4Base(type a);
		Vector4Base(type a, type b, type c, type d);
		Vector4Base(const Vector4Base& base);
		virtual ~Vector4Base();

	public:
		type w;
	};

}

