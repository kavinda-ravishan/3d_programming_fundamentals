#pragma once

#include "Scene.hpp"
#include "Cube.hpp"
#include "Mat3.hpp"
#include "Pipeline.hpp"
#include "SolidEffect.hpp"

class DoubleCubeScene : public Scene
{
public:
	typedef Pipeline<SolidEffect> Pipeline;
	typedef Pipeline::Vertex Vertex;
public:
	DoubleCubeScene(Graphics& gfx)
		:
		itlist(Cube::GetPlainIndependentFaces<Vertex>()),
		pipeline(gfx),
		Scene("Colored cube solid faces scene")
	{
		const Color colors[] = {
			Colors::Red,Colors::Green,Colors::Blue,Colors::Magenta,Colors::Yellow,Colors::Cyan
		};

		for (int i = 0; i < itlist.vertices.size(); i++)
		{
			itlist.vertices[i].color = colors[i / 4];
		}
	}
	virtual void Update(const int key, const float dt) override {
		switch (key) {
		case 'q':
			theta_x = wrap_angle(theta_x + delta_theta * dt);
			break;
		case 'w':
			theta_y = wrap_angle(theta_y + delta_theta * dt);
			break;
		case 'e':
			theta_z = wrap_angle(theta_z + delta_theta * dt);
			break;
		case 'a':
			theta_x = wrap_angle(theta_x - delta_theta * dt);
			break;
		case 's':
			theta_y = wrap_angle(theta_y - delta_theta * dt);
			break;
		case 'd':
			theta_z = wrap_angle(theta_z - delta_theta * dt);
			break;

		case 'x':
			offset_z += 2.0f * dt;
			break;
		case 'z':
			offset_z -= 2.0f * dt;
			break;

		default:
			break;
		}
	}
	virtual void Draw() override
	{
		pipeline.BeginFrame();
		// draw fixed cube
		{
			// generate rotation matrix from euler angles
			// rotate in opposition to mobile cube
			const Mat3 rot =
				Mat3::RotationX(-theta_x) *
				Mat3::RotationY(-theta_y) *
				Mat3::RotationZ(-theta_z);
			const Vec3 trans = { 0.0f,0.0f,2.0f };
			// set pipeline transform
			pipeline.effect.vs.BindRotation(rot);
			pipeline.effect.vs.BindTranslation(trans);
			// render triangles
			pipeline.Draw(itlist);
		}
		// draw mobile cube
		{
			// generate rotation matrix from euler angles
			const Mat3 rot =
				Mat3::RotationX(theta_x) *
				Mat3::RotationY(theta_y) *
				Mat3::RotationZ(theta_z);
			const Vec3 trans = { 0.0f,0.0f,offset_z };
			// set pipeline transform
			pipeline.effect.vs.BindRotation(rot);
			pipeline.effect.vs.BindTranslation(trans);
			// render triangles
			pipeline.Draw(itlist);
		}
	}
private:
	IndexedTriangleList<Vertex> itlist;
	Pipeline pipeline;
	static constexpr float delta_theta = PI;
	float offset_z = 2.0f;
	float theta_x = 0.0f;
	float theta_y = 0.0f;
	float theta_z = 0.0f;
};
