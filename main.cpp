#include <memory>
#include <string>
#include <iostream>

#include "Graphics.hpp"
#include "CubeSkinnedScene.hpp"
#include "CubeVertexColorScene.hpp"
#include "CubeSolidScene.hpp"
#include "DoubleCubeScene.hpp"
#include "VertexWaveScene.hpp"
#include "CubeVertexPositionColorScene.hpp"

class Game {
public:
    Game(const std::string window_name, const int frame_width, const int frame_height)
        : _gfx(window_name, frame_width, frame_height) {
    
        const std::string sauron_img_path = "Images\\sauron.png";
        const std::string dice_skin_img_path = "Images\\dice_skin.png";
        const std::string office_skin_img_path = "Images\\office_skin.jpg";

        _scenes.push_back(std::make_unique<CubeVertexPositionColorScene>(_gfx));
        _scenes.push_back(std::make_unique<VertexWaveScene>(_gfx, sauron_img_path));
        _scenes.push_back(std::make_unique<DoubleCubeScene>(_gfx));
        _scenes.push_back(std::make_unique<CubeSolidScene>(_gfx));
        _scenes.push_back(std::make_unique<CubeVertexColorScene>(_gfx));
        _scenes.push_back(std::make_unique<CubeSkinnedScene>(_gfx, office_skin_img_path));
        _scenes.push_back(std::make_unique<CubeSkinnedScene>(_gfx, dice_skin_img_path));
        _curr_scene = _scenes.begin();
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
        constexpr int time_per_frame_ms = static_cast<int>(dt * 1000);

        const int key_input = _gfx.GetInput(time_per_frame_ms);
        (*_curr_scene)->Update(key_input, dt);
        ManageInputs(key_input);
    }

    void ComposeFrame() {
        (*_curr_scene)->Draw();
    }

    void CycleScenes() {
        if (++_curr_scene == _scenes.end()) {
            _curr_scene = _scenes.begin();
        }
        
        std::cout << "Cycling scene to : " << (*_curr_scene)->GetSceneName() << "\n";
    }

    void ManageInputs(const int key) {

        constexpr float dt = 1.0f / 60.0f;

        switch (key) {
        case 27: // ESC key
        {
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
    
    std::vector<std::unique_ptr<Scene>> _scenes{};
    std::vector<std::unique_ptr<Scene>>::iterator _curr_scene{};
};

int main() {
    const int frame_width{ 640 };
    const int frame_height{ 640 };
    const char* window_name = "3D Programming Fundamentals";
    
    try {
        Game game{window_name, frame_width, frame_height};
        game.Go();
    } catch(std::exception ex) {
        std::cout << "Exception occered : " << ex.what() << "\n";
    }

    return 0;
}
