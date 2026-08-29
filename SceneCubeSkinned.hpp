#pragma once
#include <string>
#include "Scene.hpp"
#include "Cube.hpp"
#include "Mat3.hpp"
#include "Pipeline.hpp"

class SceneCubeSkinned : public Scene
{
public:
    typedef Pipeline::Vertex Vertex;
public:
    SceneCubeSkinned(Graphics& gfx, const std::string texture_path)
        :
        Scene(
            std::string("Folded Textured Clamped Cube ") + texture_path
        ), 
        itlist(Cube::GetSkinned<Vertex>()),
        pipeline(gfx) 
    {
        pipeline.BindTexture(texture_path);
    }

    ~SceneCubeSkinned() = default;

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

    virtual void Draw() override {
        // generate rotation matrix from euler angles
        // translation from offset
        const Mat3 rot =
            Mat3::RotationX(theta_x) *
            Mat3::RotationY(theta_y) *
            Mat3::RotationZ(theta_z);
        const Vec3 trans = { 0.0f,0.0f,offset_z };
        // set pipeline transform
        pipeline.BindRotation(rot);
        pipeline.BindTranslation(trans);
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
