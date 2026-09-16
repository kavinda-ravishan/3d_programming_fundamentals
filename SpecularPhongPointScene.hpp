#pragma once

#include "Scene.hpp"
#include "Cube.hpp"
#include "Mat.hpp"
#include "Pipeline.hpp"
#include "SpecularPhongPointEffect.hpp"
#include "SolidEffect.hpp"
#include "Sphere.hpp"

class SpecularPhongPointScene : public Scene
{
public:
	typedef ::Pipeline<SpecularPhongPointEffect> Pipeline;
	typedef ::Pipeline<SolidEffect> LightIndicatorPipeline;
	typedef Pipeline::Vertex Vertex;
public:
	SpecularPhongPointScene(Graphics& gfx, IndexedTriangleList<Vertex> tl)
		:
		itlist(std::move(tl)),
		pZb(std::make_shared<ZBuffer>(gfx.GetFrameWidth(), gfx.GetFrameHeight())),
		pipeline(gfx, pZb),
		liPipeline(gfx, pZb),
		Scene("Specular phong point shader scene free mesh")
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

		const auto proj = Mat4::ProjectionHFOV(100.0f, 1.33333f, 1.0f, 10.0f);
		// set pipeline transform
		pipeline.effect.vs.BindWorld(
			Mat4::RotationX(theta_x) *
			Mat4::RotationY(theta_y) *
			Mat4::RotationZ(theta_z) *
			Mat4::Translation(0.0f, 0.0f, offset_z)
		);
		pipeline.effect.vs.BindProjection(proj);
		pipeline.effect.ps.SetLightPosition({ lpos_x,lpos_y,lpos_z });
		// render triangles
		pipeline.Draw(itlist);

		// draw light indicator with different pipeline
		// don't call beginframe on this pipeline b/c wanna keep zbuffer contents
		// (don't like this assymetry but we'll live with it for now)
		liPipeline.effect.vs.BindWorld(Mat4::Translation(lpos_x, lpos_y, lpos_z));
		liPipeline.effect.vs.BindProjection(proj);
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
