#pragma once

#include "Vec3.hpp"

class PC3Transformer
{
public:
	PC3Transformer(const int frame_width, const int frame_height)
		:
		_x_factor(float(frame_width) / 2.0f),
		_y_factor(float(frame_height) / 2.0f)
	{
	}
	// PC3 -> screen coordinate
	template<class Vertex>
	Vertex& Transform(Vertex& v) const
	{
		const float zInv = 1.0f / v.pos.z;
		// divide all position components and attributes by z
		// (we want to be interpolating our attributes in the
		//  same space where the x,y interpolation is taking
		//  place to prevent distortion)
		v *= zInv;
		// adjust position x,y from perspective normalized space
		// to screen dimension space after perspective divide
		v.pos.x = (v.pos.x + 1.0f) * _x_factor;
		v.pos.y = (-v.pos.y + 1.0f) * _y_factor;
		// store 1/z in z (we will need the interpolated 1/z
		// so that we can recover the attributes after interp.)
		v.pos.z = zInv;

		return v;
	}

	// PC3 -> screen coordinate
	template<class Vertex>
	Vertex GetTransformed(const Vertex& v) const
	{
		Vertex vertex{ v };
		return Transform(vertex);
	}
private:
	float _x_factor;
	float _y_factor;
};
