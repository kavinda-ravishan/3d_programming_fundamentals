#pragma once

#include "Scene.hpp"
#include "Cube.hpp"
#include "Mat3.hpp"
#include "Pipeline.hpp"
#include "VertexFlatEffect.hpp"

class CubeFlatIndependentScene : public Scene
{
public:
	typedef Pipeline<VertexFlatEffect> Pipeline;
	typedef Pipeline::Vertex Vertex;
public:
	CubeFlatIndependentScene(Graphics& gfx)
		:
		itlist(Cube::GetIndependentFacesNormals<Vertex>()),
		pipeline(gfx),
		Scene("Cube flat vertex scene")
	{
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

		case 'u':
			phi_x = wrap_angle(phi_x + delta_theta * dt);
			break;
		case 'i':
			phi_y = wrap_angle(phi_y + delta_theta * dt);
			break;
		case 'o':
			phi_z = wrap_angle(phi_z + delta_theta * dt);
			break;
		case 'j':
			phi_x = wrap_angle(phi_x - delta_theta * dt);
			break;
		case 'k':
			phi_y = wrap_angle(phi_y - delta_theta * dt);
			break;
		case 'l':
			phi_z = wrap_angle(phi_z - delta_theta * dt);
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
		const Mat3 rot_phi =
			Mat3::RotationX(phi_x) *
			Mat3::RotationY(phi_y) *
			Mat3::RotationZ(phi_z);
		const Vec3 trans = { 0.0f,0.0f,offset_z };
		// set pipeline transform
		pipeline.effect.vs.BindRotation(rot);
		pipeline.effect.vs.BindTranslation(trans);
		pipeline.effect.vs.SetLightDirection(light_dir * rot_phi);
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
	float phi_x = 0.0f;
	float phi_y = 0.0f;
	float phi_z = 0.0f;
	Vec3 light_dir = { 0.2f,-0.5f,1.0f };
};
