#pragma once

#include "Math.hpp"

namespace open_3d {

	template <typename T>
	class _Vec3 : public _Vec2<T>
	{
	public:
		_Vec3() {}
		_Vec3(T x, T y, T z)
			:
			_Vec2<T>(x, y),
			z(z)
		{
		}
		_Vec3(const _Vec3& vect)
			:
			_Vec3(vect.x, vect.y, vect.z)
		{
		}
		template <typename T2>
		explicit operator _Vec3<T2>() const
		{
			return{ (T2)this->x,(T2)this->y,(T2)z };
		}
		T		LenSq() const
		{
			return sq(*this);
		}
		T		Len() const
		{
			return sqrt(LenSq());
		}
		_Vec3& Normalize()
		{
			const T length = Len();
			this->x /= length;
			this->y /= length;
			z /= length;
			return *this;
		}
		_Vec3 GetNormalized() const
		{
			_Vec3 norm = *this;
			norm.Normalize();
			return norm;
		}
		_Vec3 operator-() const
		{
			return _Vec3(-this->x, -this->y, -z);
		}
		_Vec3& operator=(const _Vec3& rhs)
		{
			this->x = rhs.x;
			this->y = rhs.y;
			z = rhs.z;
			return *this;
		}
		_Vec3& operator+=(const _Vec3& rhs)
		{
			this->x += rhs.x;
			this->y += rhs.y;
			z += rhs.z;
			return *this;
		}
		_Vec3& operator-=(const _Vec3& rhs)
		{
			this->x -= rhs.x;
			this->y -= rhs.y;
			z -= rhs.z;
			return *this;
		}
		T operator*(const _Vec3& rhs) const
		{
			return this->x * rhs.x + this->y * rhs.y + z * rhs.z;
		}
		_Vec3 operator+(const _Vec3& rhs) const
		{
			return _Vec3(*this) += rhs;
		}
		_Vec3 operator-(const _Vec3& rhs) const
		{
			return _Vec3(*this) -= rhs;
		}
		_Vec3& operator*=(const T& rhs)
		{
			this->x *= rhs;
			this->y *= rhs;
			z *= rhs;
			return *this;
		}
		_Vec3 operator*(const T& rhs) const
		{
			return _Vec3(*this) *= rhs;
		}
		_Vec3 operator%(const _Vec3& rhs) const
		{
			return _Vec3(
				this->y * rhs.z - this->z * rhs.y,
				this->z * rhs.x - this->x * rhs.z,
				this->x * rhs.y - this->y * rhs.x);
		}
		_Vec3& operator/=(const T& rhs)
		{
			this->x /= rhs;
			this->y /= rhs;
			z /= rhs;
			return *this;
		}
		_Vec3 operator/(const T& rhs) const
		{
			return _Vec3(*this) /= rhs;
		}
		bool operator==(const _Vec3& rhs) const
		{
			return this->x == rhs.x && this->y == rhs.y && rhs.z == z;
		}
		bool operator!=(const _Vec3& rhs) const
		{
			return !(*this == rhs);
		}
		_Vec3 InterpolateTo(const _Vec3& dest, T alpha) const
		{
			return *this + (dest - *this) * alpha;
		}
	public:
		T z;
	};

	typedef _Vec3<float> Vec3;
	typedef _Vec3<double> Ved3;
	typedef _Vec3<int> Vei3;
} // namespace open_3d