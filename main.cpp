#include <string>
#include <iostream>
#include <vector>

#include "Graphics.hpp"
#include "PC3Transformer.hpp"
#include "Cube.hpp"

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

    open_3d::PC3Transformer _pc3t;
    open_3d::Cube _cube;

    bool _main_loop_active{ true };

private:
    void ManageInputs(const int key) {
        switch (key) {
        case -1:
        case 27:
        {
            // ESC key or when window is closed
            _main_loop_active = false;
            break;
        }
        case 'a':
            break;
        case 'd':
            break;
        case 's':
            break;
        case 'w':
            break;
        case 'q':
            break;
        case 'e':
            break;
        default:
            break;
        }
    }

    void InitModel() {
    }

    void UpdateModel() {
    }

    void ComposeFrame() {
        auto lines = _cube.GetLines();

        for (auto& v : lines.vertices) {
            v += {0.0f, 0.0f, 1.0f};
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
