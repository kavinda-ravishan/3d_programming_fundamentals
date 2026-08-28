#include <string>

#include "Graphics.hpp"
#include "PC3Transformer.hpp"
#include "Cube.hpp"
#include "Mat3.hpp"

class Game {
public:
    Game(const std::string window_name, const int frame_width, const int frame_height)
        : _gfx(window_name, frame_width, frame_height), _pc3t(frame_width, frame_height), _cube(1.0f) {}

    ~Game() = default;

    void Go() {

        InitModel();

        while (_main_loop_active) {

            _gfx.BeginFrame();

            UpdateModel();
            ComposeFrame();

            _gfx.EndFrame();
            ManageInputs(_gfx.GetInput());
        }

    }

private:
    Graphics _gfx;

    /********************************/
    /*  User Variables              */
    open_3d::PC3Transformer _pc3t;
    open_3d::Cube _cube;
    static constexpr float _delta_theta = open_3d::PI;
    float _offset_z = 2.0f;
    float _theta_x = 0.0f;
    float _theta_y = 0.0f;
    float _theta_z = 0.0f;
    /********************************/

    bool _main_loop_active{ true };

private:
    void ManageInputs(const int key) {

        constexpr float dt = 1.0f / 60.0f;

        switch (key) {
        case -1:
        case 27:
        {
            // ESC key or when window is closed
            _main_loop_active = false;
            break;
        }

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

    void InitModel() {}

    void UpdateModel() {}

    void ComposeFrame() {

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

        const open_3d::Mat3 rot =
            open_3d::Mat3::RotationX(_theta_x) *
            open_3d::Mat3::RotationY(_theta_y) *
            open_3d::Mat3::RotationZ(_theta_z);

        auto triangles = _cube.GetTriangles();
        for (auto& v : triangles.vertices) {
            v *= rot;
            v += { 0.0f, 0.0f, _offset_z };

            _pc3t.Transform(v);
        }
        for (auto i = triangles.indices.cbegin(); i != triangles.indices.cend(); std::advance(i, 3)) {
            _gfx.DrawTriangle(
                triangles.vertices[*i], 
                triangles.vertices[*std::next(i)], 
                triangles.vertices[*std::next(i, 2)], 
                colors[std::distance(triangles.indices.cbegin(), i) / 3]
            );
        }

        auto lines = _cube.GetLines();
        for (auto& v : lines.vertices) {
            v *= rot;
            v += { 0.0f, 0.0f, _offset_z };

            _pc3t.Transform(v);
        }
        for (auto i = lines.indices.cbegin(); i != lines.indices.cend(); std::advance(i, 2)) {
            _gfx.DrawLine(lines.vertices[*i], lines.vertices[*std::next(i)], open_3d::Colors::White);
        }

    }
};

int main() {
    const int frame_width{ 640 };
    const int frame_height{ 640 };
    
    Game game{"3D Programming Fundamentals", frame_width, frame_height};
    game.Go();

    return 0;
}
