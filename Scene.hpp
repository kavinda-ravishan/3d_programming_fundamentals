#pragma once
#include "Graphics.hpp"

class Scene {
public:
	Scene(const int frame_width, const int frame_height) {}
	virtual ~Scene() = default;
	virtual const char* GetSceneName() = 0;

	virtual void Update(const int key, const float dt) = 0;
	virtual void Draw(Graphics& gfx) const = 0;
};
