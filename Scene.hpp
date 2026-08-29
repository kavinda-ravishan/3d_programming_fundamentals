#pragma once
#include <string>
#include "Graphics.hpp"

class Scene {
public:
	Scene(const int frame_width, const int frame_height, const std::string scene_name)
	: _scene_name(scene_name) {}
	virtual ~Scene() = default;

	virtual void Update(const int key, const float dt) = 0;
	virtual void Draw(Graphics& gfx) const = 0;

	const std::string GetSceneName() { return _scene_name; }
private:
	std::string _scene_name;
};
