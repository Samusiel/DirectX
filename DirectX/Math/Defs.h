#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

namespace Math
{
	template <typename T>
	struct TRect
	{
		Vector2Base<T> Min;
		Vector2Base<T> Max;

		TRect() : Min(std::numeric_limits<T>::max()), Max(-std::numeric_limits<T>::max()) {}

		TRect(const TRect<T> &r) : Min(r.Min), Max(r.Max) {}

		TRect(const Vector2Base<T> &_min, const Vector2Base<T> &_max) : Min(_min), Max(_max) {}

		TRect(const T _minX, const T _minY, const T _maxX, const T _maxY) : Min(_minX, _minY), Max(_maxX, _maxY) {}

		T Width() const
		{
			return (Max.x - Min.x);
		}

		T Height() const
		{
			return (Max.y - Min.y);
		}

		bool IsInsideOut() const
		{
			return (this->Width() < 0 || this->Height() < 0);
		}

		void Move(T x, T y)
		{
			int dx = x - Min.x;
			int dy = y - Min.y;
			Min.x += dx;
			Min.y += dy;
			Max.x += dx;
			Max.y += dy;
		}

		void Resize(T width, T height)
		{
			Max.x = Min.x + width;
			Max.y = Min.y + height;
		}
	};

	typedef Vector2Base<float> Vector2;
	typedef Vector3Base<float> Vector3;
	typedef Vector4Base<float> Vector4;

	typedef TRect<int> Rect;

}