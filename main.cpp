#include <memory>
#include <string>
#include <iostream>

#include "Graphics.hpp"
#include "SolidCubeScene.hpp"

class Game {
public:
    Game(const std::string window_name, const int frame_width, const int frame_height)
        : _gfx(window_name, frame_width, frame_height) {
    
        scenes.push_back(std::make_unique<SolidCubeScene>(frame_width, frame_height));
        curr_scene = scenes.begin();
    }

    ~Game() = default;

    void Go() {


        while (_main_loop_active) {
            _gfx.BeginFrame();

            ComposeFrame();
            
            _gfx.EndFrame();

            UpdateModel();
        }

    }

private:
    void UpdateModel() {
        constexpr float dt = 1.0f / 60.0f;

        const int key_input = _gfx.GetInput();
        (*curr_scene)->Update(key_input, dt);
        ManageInputs(key_input);
    }

    void ComposeFrame() {
        (*curr_scene)->Draw(_gfx);
    }

    void CycleScenes() {
        if (++curr_scene == scenes.end()) {
            curr_scene = scenes.begin();
            std::cout << "Cycling scene to : " << (*curr_scene)->GetSceneName() << "\n";
        }
    }

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
        case 9: // TAB key
            CycleScenes();
            break;

        default:
            break;
        }
    }

private:
    Graphics _gfx;
    bool _main_loop_active{ true };
    
    std::vector<std::unique_ptr<Scene>> scenes;
    std::vector<std::unique_ptr<Scene>>::iterator curr_scene;
};

int main() {
    const int frame_width{ 640 };
    const int frame_height{ 640 };
    
    Game game{"3D Programming Fundamentals", frame_width, frame_height};
    game.Go();

    return 0;
}
