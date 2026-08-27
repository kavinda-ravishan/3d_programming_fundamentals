#pragma once

#include <vector>
#include "Vec3.hpp"

namespace open_3d {

	struct IndexedLineList
	{
		std::vector<Vec3> vertices;
		std::vector<size_t> indices;
	};

} // namespace open_3d
