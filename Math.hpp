#pragma once

#include <math.h>

namespace open_3d {

	constexpr float PI = 3.14159265f;

	template <typename T>
	inline auto sq(const T& x)
	{
		return x * x;
	}

} // namespace open_3d
