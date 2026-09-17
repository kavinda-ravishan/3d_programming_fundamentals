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
		htrack(to_rad(hfov) / (float)gfx.GetFrameWidth()),
		vtrack(to_rad(hfov) / (float)gfx.GetFrameHeight()),
		Scene("Specular phong point shader scene free mesh")
	{
		itlist.AdjustToTrueCenter();
		mod_pos.z = itlist.GetRadius() * 1.6f;
		for (auto& v : lightIndicator.vertices)
		{
			v.color = Colors::White;
		}
	}
	virtual void Update(const int key, const float dt) override {
		switch (key) {
		case 'w':
			cam_pos += Vec4{ 0.0f,0.0f,1.0f,0.0f } * !cam_rot_inv * cam_speed * dt;
			break;
		case 'a':
			cam_pos += Vec4{ -1.0f,0.0f,0.0f,0.0f } * !cam_rot_inv * cam_speed * dt;
			break;
		case 's':
			cam_pos += Vec4{ 0.0f,0.0f,-1.0f,0.0f } * !cam_rot_inv * cam_speed * dt;
			break;
		case 'd':
			cam_pos += Vec4{ 1.0f,0.0f,0.0f,0.0f } * !cam_rot_inv * cam_speed * dt;
			break;

		case 't': 
			cam_rot_inv = cam_rot_inv * Mat4::RotationX(-cam_roll_speed * vtrack);
			break;
		case 'g':
			cam_rot_inv = cam_rot_inv * Mat4::RotationX(cam_roll_speed * vtrack);
			break;
		case 'f':
			cam_rot_inv = cam_rot_inv * Mat4::RotationY(-cam_roll_speed * vtrack);
			break;
		case 'h':
			cam_rot_inv = cam_rot_inv * Mat4::RotationY(cam_roll_speed * vtrack);
			break;

		case 'c':
			cam_pos += Vec4{ 0.0f,1.0f,0.0f,0.0f } * !cam_rot_inv * cam_speed * dt;
			break;
		case 'z':
			cam_pos += Vec4{ 0.0f,-1.0f,0.0f,0.0f } * !cam_rot_inv * cam_speed * dt;
			break;
		case 'q':
			cam_rot_inv = cam_rot_inv * Mat4::RotationZ(cam_roll_speed * dt);
			break;
		case 'e':
			cam_rot_inv = cam_rot_inv * Mat4::RotationZ(-cam_roll_speed * dt);
			break;
		default:
			break;
		}
	}
	virtual void Draw() override
	{
		pipeline.BeginFrame();

		const auto proj = Mat4::ProjectionHFOV(hfov, aspect_ratio, 0.5f, 4.0f);
		const auto view = Mat4::Translation(-cam_pos) * cam_rot_inv;
		// set pipeline transform
		pipeline.effect.vs.BindWorld(
			Mat4::RotationX(theta_x) *
			Mat4::RotationY(theta_y) *
			Mat4::RotationZ(theta_z) *
			Mat4::Translation(mod_pos)
		);
		pipeline.effect.vs.BindView(view);
		pipeline.effect.vs.BindProjection(proj);
		pipeline.effect.ps.SetLightPosition(l_pos * view);
		// render triangles
		pipeline.Draw(itlist);

		// draw light indicator with different pipeline
		// don't call beginframe on this pipeline b/c wanna keep zbuffer contents
		// (don't like this assymetry but we'll live with it for now)
		liPipeline.effect.vs.BindWorldView(Mat4::Translation(l_pos) * view);
		liPipeline.effect.vs.BindProjection(proj);
		liPipeline.Draw(lightIndicator);
	}
private:
	IndexedTriangleList<Vertex> itlist;
	IndexedTriangleList<SolidEffect::Vertex> lightIndicator = Sphere::GetPlain<SolidEffect::Vertex>(0.05f);
	std::shared_ptr<ZBuffer> pZb;
	Pipeline pipeline;
	LightIndicatorPipeline liPipeline;
	// fov
	static constexpr float aspect_ratio = 1.33333f;
	static constexpr float hfov = 95.0f;
	static constexpr float vfov = hfov / aspect_ratio;
	// camera stuff
	const float htrack;
	const float vtrack;
	static constexpr float cam_speed = 1.0f;
	static constexpr float cam_roll_speed = PI;
	Vec3 cam_pos = { 0.0f,0.0f,0.0f };
	Mat4 cam_rot_inv = Mat4::Identity();
	// model stuff
	Vec3 mod_pos = { 0.0f,0.0f,2.0f };
	float theta_x = 0.0f;
	float theta_y = 0.0f;
	float theta_z = 0.0f;
	// light stuff
	Vec4 l_pos = { 0.0f,0.0f,0.6f,1.0f };
};
