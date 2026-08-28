#pragma once
#include "Graphics.hpp"
#include "PC3Transformer.hpp"
#include "Cube.hpp"
#include "Mat3.hpp"
#include "Scene.hpp"

class SolidCubeScene : public Scene
{
public:
	SolidCubeScene(const int frame_width, const int frame_height)
        : Scene(frame_width, frame_height), _pc3t(frame_width, frame_height), _cube(1.0f)
    {}

    ~SolidCubeScene() = default;

    const char* GetSceneName() { return "Solid Cube"; }

	virtual void Update(const int key, const float dt) override {
        switch (key) {
        case 'c':
            _add_wireframe = !_add_wireframe;
            break;

        case 'q':
            _theta_x = open_3d::wrap_angle(_theta_x + _delta_theta * dt);
            break;
        case 'w':
            _theta_y = open_3d::wrap_angle(_theta_y + _delta_theta * dt);
            break;
        case 'e':
            _theta_z = open_3d::wrap_angle(_theta_z + _delta_theta * dt);
            break;
        case 'a':
            _theta_x = open_3d::wrap_angle(_theta_x - _delta_theta * dt);
            break;
        case 's':
            _theta_y = open_3d::wrap_angle(_theta_y - _delta_theta * dt);
            break;
        case 'd':
            _theta_z = open_3d::wrap_angle(_theta_z - _delta_theta * dt);
            break;

        case 'x':
            _offset_z += 2.0f * dt;
            break;
        case 'z':
            _offset_z -= 2.0f * dt;
            break;

        default:
            break;
        }
	}

	virtual void Draw(Graphics& gfx) const override {
        constexpr open_3d::Color colors[12] = {
            open_3d::Colors::White,
            open_3d::Colors::Blue,
            open_3d::Colors::Cyan,
            open_3d::Colors::Gray,
            open_3d::Colors::Green,
            open_3d::Colors::Magenta,
            open_3d::Colors::LightGray,
            open_3d::Colors::Red,
            open_3d::Colors::Yellow,
            open_3d::Colors::White,
            open_3d::Colors::Blue,
            open_3d::Colors::Cyan
        };

        // generate rotation matrix from euler angles
        const open_3d::Mat3 rot =
            open_3d::Mat3::RotationX(_theta_x) *
            open_3d::Mat3::RotationY(_theta_y) *
            open_3d::Mat3::RotationZ(_theta_z);
        auto triangles = _cube.GetTriangles();
        // transform from model space -> world (/view) space
        for (auto& v : triangles.vertices) {
            v *= rot;
            v += { 0.0f, 0.0f, _offset_z };
        }
        // backface culling test (must be done in world (/view) space)
        for (size_t i = 0; i < triangles.indices.size() / 3; i++) {
            const open_3d::Vec3& v0 = triangles.vertices[triangles.indices[i * 3]];
            const open_3d::Vec3& v1 = triangles.vertices[triangles.indices[i * 3 + 1]];
            const open_3d::Vec3& v2 = triangles.vertices[triangles.indices[i * 3 + 2]];

            triangles.call_flags[i] = (v1 - v0) % (v2 - v0) * v0 >= 0.0f;
        }
        // transform to screen space (includes perspective transform)
        for (auto& v : triangles.vertices) {
            _pc3t.Transform(v);
        }
        // draw the mf triangles!
        for (size_t i = 0; i < triangles.indices.size() / 3; i++) {
            // skip triangles previously determined to be back-facing
            if (!triangles.call_flags[i]) {
                gfx.DrawTriangle(
                    triangles.vertices[triangles.indices[i * 3]],
                    triangles.vertices[triangles.indices[i * 3 + 1]],
                    triangles.vertices[triangles.indices[i * 3 + 2]],
                    colors[i]
                );
            }
        }

        if (_add_wireframe) {
            auto lines = _cube.GetLines();
            for (auto& v : lines.vertices) {
                v *= rot;
                v += { 0.0f, 0.0f, _offset_z };

                _pc3t.Transform(v);
            }
            for (auto i = lines.indices.cbegin(); i != lines.indices.cend(); std::advance(i, 2)) {
                gfx.DrawLine(lines.vertices[*i], lines.vertices[*std::next(i)], open_3d::Colors::White);
            }
        }
	}

private:
    open_3d::PC3Transformer _pc3t;
    open_3d::Cube _cube;
    bool _add_wireframe{ false };
    static constexpr float _delta_theta = open_3d::PI;
    float _offset_z = 2.0f;
    float _theta_x = 0.0f;
    float _theta_y = 0.0f;
    float _theta_z = 0.0f;
};
