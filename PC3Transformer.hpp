#pragma once

#include "Vec.hpp"

namespace open_3d {

	class PC3Transformer
	{
	public:
		PC3Transformer(const int frame_width, const int frame_height)
			:
			xFactor(float(frame_width) / 2.0f),
			yFactor(float(frame_height) / 2.0f)
		{
		}
		// PC3 -> screen coordinate
		Vec3& Transform(Vec3& v) const
		{
			v.x = (v.x + 1.0f) * xFactor;
			v.y = (-v.y + 1.0f) * yFactor;
			return v;
		}
		// PC3 -> screen coordinate
		Vec3 GetTransformed(const Vec3& v) const
		{
			Vec3 vec3{ v };
			return Transform(vec3);
		}
	private:
		float xFactor;
		float yFactor;
	};
} // namespace open_3d
