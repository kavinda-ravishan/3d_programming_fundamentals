#pragma once
#include <string>
#include "Graphics.hpp"
#include "PC3Transformer.hpp"
#include "Cube.hpp"
#include "Mat3.hpp"
#include "Scene.hpp"

class SceneTexCube : public Scene
{
public:
    SceneTexCube(const int frame_width, const int frame_height, const std::string texture_path, const float tex_dim)
        : 
        Scene(
            frame_width, 
            frame_height, 
            std::string("Textured Cube ") + texture_path + 
            " Wrapping Dim: " + std::to_string(tex_dim)
        ), 
        _pc3t(frame_width, frame_height), 
        _tex(texture_path), 
        _cube(1.0f, tex_dim) {}

    ~SceneTexCube() = default;

    virtual void Update(const int key, const float dt) override {
        switch (key) {
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
        // NOTE: Changes had to be made because of Vec3 -> TexVertex
        // generate indexed triangle list
        auto triangles = _cube.GetTrianglesTex();
        // generate rotation matrix from euler angles
        const open_3d::Mat3 rot =
            open_3d::Mat3::RotationX(_theta_x) *
            open_3d::Mat3::RotationY(_theta_y) *
            open_3d::Mat3::RotationZ(_theta_z);
        // transform from model space -> world (/view) space
        for (auto& v : triangles.vertices)
        {
            v.pos *= rot;
            v.pos += { 0.0f, 0.0f, _offset_z };
        }
        // backface culling test (must be done in world (/view) space)
        for (size_t i = 0,
            end = triangles.indices.size() / 3;
            i < end; i++)
        {
            const open_3d::Vec3& v0 = triangles.vertices[triangles.indices[i * 3]].pos;
            const open_3d::Vec3& v1 = triangles.vertices[triangles.indices[i * 3 + 1]].pos;
            const open_3d::Vec3& v2 = triangles.vertices[triangles.indices[i * 3 + 2]].pos;
            triangles.cull_flags[i] = (v1 - v0) % (v2 - v0) * v0 > 0.0f;
        }
        // transform to screen space (includes perspective transform)
        for (auto& v : triangles.vertices)
        {
            _pc3t.Transform(v.pos);
        }
        // draw the mf triangles!
        for (size_t i = 0,
            end = triangles.indices.size() / 3;
            i < end; i++)
        {
            // skip triangles previously determined to be back-facing
            if (!triangles.cull_flags[i])
            {
                gfx.DrawTriangleTex(
                    triangles.vertices[triangles.indices[i * 3]],
                    triangles.vertices[triangles.indices[i * 3 + 1]],
                    triangles.vertices[triangles.indices[i * 3 + 2]],
                    _tex);
            }
        }
    }

private:
    open_3d::PC3Transformer _pc3t;
    Surface _tex;
    open_3d::Cube _cube;

    static constexpr float _delta_theta = open_3d::PI;
    float _offset_z = 2.0f;
    float _theta_x = 0.0f;
    float _theta_y = 0.0f;
    float _theta_z = 0.0f;
};
