#pragma once

#include "Scene.hpp"
#include "Cube.hpp"
#include "Mat.hpp"
#include "Pipeline.hpp"
#include "PhongPointEffect.hpp"
#include "SolidEffect.hpp"
#include "Sphere.hpp"

class PhongPointScene : public Scene
{
public:
	typedef ::Pipeline<PhongPointEffect> Pipeline;
	typedef ::Pipeline<SolidEffect> LightIndicatorPipeline;
	typedef Pipeline::Vertex Vertex;
public:
	PhongPointScene(Graphics& gfx, IndexedTriangleList<Vertex> tl)
		:
		itlist(std::move(tl)),
		pZb(std::make_shared<ZBuffer>(gfx.GetFrameWidth(), gfx.GetFrameHeight())),
		pipeline(gfx, pZb),
		liPipeline(gfx, pZb),
		Scene("phong point shader scene free mesh")
	{
		itlist.AdjustToTrueCenter();
		offset_z = itlist.GetRadius() * 1.6f;
		for (auto& v : lightIndicator.vertices)
		{
			v.color = Colors::White;
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

		case 'u':
			lpos_x = wrap_angle(lpos_x + delta_theta * dt);
			break;
		case 'i':
			lpos_y = wrap_angle(lpos_y + delta_theta * dt);
			break;
		case 'o':
			lpos_z = wrap_angle(lpos_z + delta_theta * dt);
			break;
		case 'j':
			lpos_x = wrap_angle(lpos_x - delta_theta * dt);
			break;
		case 'k':
			lpos_y = wrap_angle(lpos_y - delta_theta * dt);
			break;
		case 'l':
			lpos_z = wrap_angle(lpos_z - delta_theta * dt);
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
		pipeline.effect.ps.SetLightPosition({ lpos_x,lpos_y,lpos_z });
		// render triangles
		pipeline.Draw(itlist);

		// draw light indicator with different pipeline
		// don't call beginframe on this pipeline b/c wanna keep zbuffer contents
		// (don't like this assymetry but we'll live with it for now)
		liPipeline.effect.vs.BindTranslation({ lpos_x,lpos_y,lpos_z });
		liPipeline.effect.vs.BindRotation(Mat3::Identity());
		liPipeline.Draw(lightIndicator);
	}
private:
	IndexedTriangleList<Vertex> itlist;
	IndexedTriangleList<SolidEffect::Vertex> lightIndicator = Sphere::GetPlain<SolidEffect::Vertex>(0.05f);
	std::shared_ptr<ZBuffer> pZb;
	Pipeline pipeline;
	LightIndicatorPipeline liPipeline;
	static constexpr float delta_theta = PI;
	float offset_z = 2.0f;
	float theta_x = 0.0f;
	float theta_y = 0.0f;
	float theta_z = 0.0f;
	float lpos_x = 0.0f;
	float lpos_y = 0.0f;
	float lpos_z = 0.6f;
};
