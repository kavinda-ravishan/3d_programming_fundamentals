#pragma once

#include "Vec3.hpp"

class NDCScreenTransformer
{
public:
	NDCScreenTransformer(const int frame_width, const int frame_height)
		:
		_x_factor(float(frame_width) / 2.0f),
		_y_factor(float(frame_height) / 2.0f)
	{
	}
	template<class Vertex>
	Vertex& Transform(Vertex& v) const
	{
		// perform homo -> ndc on xyz / perspective-correct interpolative divide on all other attributes
		const float wInv = 1.0f / v.pos.w;
		v *= wInv;
		// additional divide for mapped z because it must be interpolated
		// adjust position x,y from perspective normalized space
		// to screen dimension space after perspective divide
		v.pos.x = (v.pos.x + 1.0f) * _x_factor;
		v.pos.y = (-v.pos.y + 1.0f) * _y_factor;
		// store 1/w in w (we will need the interpolated 1/w
		// so that we can recover the attributes after interp.)
		v.pos.w = wInv;

		return v;
	}
	template<class Vertex>
	Vertex GetTransformed(const Vertex& v) const
	{
		return Transform(Vertex(v));
	}
private:
	float _x_factor;
	float _y_factor;
};
