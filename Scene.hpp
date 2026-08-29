#pragma once
#include <string>
#include "Graphics.hpp"

class Scene {
public:
	Scene(const std::string scene_name)
	: _scene_name(scene_name) {}
	virtual ~Scene() = default;

	virtual void Update(const int key, const float dt) = 0;
	virtual void Draw() = 0;

	const std::string GetSceneName() { return _scene_name; }
private:
	std::string _scene_name;
};
