#pragma once

#include "Scene.hpp"
#include "Cube.hpp"
#include "Mat3.hpp"
#include "Pipeline.hpp"
#include "SolidGeometryEffect.hpp"

class CubeSolidGeometryScene : public Scene
{
public:
	typedef Pipeline<SolidGeometryEffect> Pipeline;
	typedef Pipeline::Vertex Vertex;
public:
	CubeSolidGeometryScene(Graphics& gfx)
		:
		itlist(Cube::GetPlain<Vertex>()),
		pipeline(gfx),
		Scene("Colored cube geometry solid face scene")
	{
		pipeline.effect.gs.BindColors(
			{ Colors::Red,Colors::Green,Colors::Blue,Colors::Magenta,Colors::Yellow,Colors::Cyan }
		);
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
		// generate rotation matrix from euler angles
		// translation from offset
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
private:
	IndexedTriangleList<Vertex> itlist;
	Pipeline pipeline;
	static constexpr float delta_theta = PI;
	float offset_z = 2.0f;
	float theta_x = 0.0f;
	float theta_y = 0.0f;
	float theta_z = 0.0f;
};
