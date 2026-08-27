#pragma once

#include "Vec3.hpp"

namespace open_3d {

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
		Vec3& Transform(Vec3& v) const
		{
			const float z_inv = 1 / v.z;

			v.x = ((v.x * z_inv) + 1.0f) * _x_factor;
			v.y = (-(v.y * z_inv) + 1.0f) * _y_factor;
			return v;
		}
		// PC3 -> screen coordinate
		Vec3 GetTransformed(const Vec3& v) const
		{
			Vec3 vec3{ v };
			return Transform(vec3);
		}
	private:
		float _x_factor;
		float _y_factor;
	};
} // namespace open_3d
