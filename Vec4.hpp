#pragma once

#include <algorithm>
#include "Math.hpp"
#include "Vec3.hpp"

template <typename T>
class _Vec4 : public _Vec3<T>
{
public:
	_Vec4() = default;
	_Vec4(T x, T y, T z, T w)
		:
		_Vec3<T>(x, y, z),
		w(w)
	{
	}
	_Vec4(const _Vec3<T>& v3, T w = (T)1.0)
		:
		_Vec3<T>(v3),
		w(w)
	{
	}
	template <typename T2>
	explicit operator _Vec4<T2>() const
	{
		return{ (T2)this->x,(T2)this->y,(T2)this->z,(T2)this->w };
	}
	//T		LenSq() const
	//{
	//	return sq( *this );
	//}
	//T		Len() const
	//{
	//	return sqrt( LenSq() );
	//}
	//_Vec3&	Normalize()
	//{
	//	const T length = Len();
	//	this->x /= length;
	//	this->y /= length;
	//	this->z /= length;
	//	return *this;
	//}
	//_Vec3	GetNormalized() const
	//{
	//	_Vec3 norm = *this;
	//	norm.Normalize();
	//	return norm;
	//}
	_Vec4	operator-() const
	{
		return _Vec4(-(this->x), -(this->y), -(this->z), -(this->w));
	}
	_Vec4& operator=(const _Vec4& rhs)
	{
		this->x = rhs.x;
		this->y = rhs.y;
		this->z = rhs.z;
		this->w = rhs.w;
		return *this;
	}
	_Vec4& operator+=(const _Vec4& rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;
		this->w += rhs.w;
		return *this;
	}
	_Vec4& operator-=(const _Vec4& rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		this->z -= rhs.z;
		this->w -= rhs.w;
		return *this;
	}
	//T		operator*( const _Vec4 &rhs ) const
	//{
	//	return x * rhs.x + y * rhs.y + z * rhs.z;
	//}
	_Vec4	operator+(const _Vec4& rhs) const
	{
		return _Vec4(*this) += rhs;
	}
	_Vec4	operator-(const _Vec4& rhs) const
	{
		return _Vec4(*this) -= rhs;
	}
	_Vec4& operator*=(const T& rhs)
	{
		this->x *= rhs;
		this->y *= rhs;
		this->z *= rhs;
		this->w *= rhs;
		return *this;
	}
	_Vec4	operator*(const T& rhs) const
	{
		return _Vec4(*this) *= rhs;
	}
	//_Vec4	operator%( const _Vec4& rhs ) const
	//{
	//	return _Vec4(
	//		y * rhs.z - z * rhs.y,
	//		z * rhs.x - x * rhs.z,
	//		x * rhs.y - y * rhs.x );
	//}
	_Vec4& operator/=(const T& rhs)
	{
		this->x /= rhs;
		this->y /= rhs;
		this->z /= rhs;
		this->w /= rhs;
		return *this;
	}
	_Vec4	operator/(const T& rhs) const
	{
		return _Vec4(*this) /= rhs;
	}
	bool	operator==(const _Vec4& rhs) const
	{
		return this->x == rhs.x && this->y == rhs.y && rhs.z == this->z && rhs.w == this->w;
	}
	bool	operator!=(const _Vec4& rhs) const
	{
		return !(*this == rhs);
	}
	// clamp to between 0.0 ~ 1.0
	_Vec4& Saturate()
	{
		this->x = std::min(1.0f, std::max(0.0f, this->x));
		this->y = std::min(1.0f, std::max(0.0f, this->y));
		this->z = std::min(1.0f, std::max(0.0f, this->z));
		this->w = std::min(1.0f, std::max(0.0f, this->w));
		return *this;
	}
	// clamp to between 0.0 ~ 1.0
	_Vec4	GetSaturated() const
	{
		_Vec4 temp(*this);
		temp.Saturate();
		return temp;
	}
	// x3 = x1 * x2 etc.
	_Vec4& Hadamard(const _Vec4& rhs)
	{
		this->x *= rhs.x;
		this->y *= rhs.y;
		this->z *= rhs.z;
		this->w *= rhs.w;
		return *this;
	}
	// x3 = x1 * x2 etc.
	_Vec4	GetHadamard(const _Vec4& rhs) const
	{
		_Vec4 temp(*this);
		temp.Hadamard(rhs);
		return temp;
	}
public:
	T w;
};

typedef _Vec4<float> Vec4;
typedef _Vec4<double> Ved4;
typedef _Vec4<int> Vei4;
