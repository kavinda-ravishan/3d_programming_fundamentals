#pragma once

#include <cassert>
#include <vector>
#include "Vec3.hpp"

namespace open_3d {
	struct IndexedTriangleList
	{
		IndexedTriangleList(std::vector<Vec3> verts_in, std::vector<size_t> indices_in)
			:
			vertices(std::move(verts_in)),
			indices(std::move(indices_in))
		{
			assert(vertices.size() > 2);
			assert(indices.size() % 3 == 0);
			call_flags.resize(indices.size() / 3, false);
		}
		std::vector<Vec3> vertices;
		std::vector<size_t> indices;
		std::vector<bool> call_flags;
	};
} // namespace open_3d
