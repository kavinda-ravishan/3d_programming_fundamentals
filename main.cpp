#include <memory>
#include <string>
#include <iostream>

#include "Graphics.hpp"
#include "SceneSolidCube.hpp"
#include "SceneCubeOrder.hpp"
#include "SceneConHex.hpp"
#include "SceneXMutual.hpp"
#include "SceneTexCube.hpp"
#include "SceneTexWrapCube.hpp"
#include "SceneFoldedCube.hpp"
#include "SceneFoldedCubeWrap.hpp"
#include "SceneCubeSkinned.hpp"

class Game {
public:
    Game(const std::string window_name, const int frame_width, const int frame_height)
        : _gfx(window_name, frame_width, frame_height) {
    
        const std::string sauron_img_path = "Images\\sauron.png";
        const std::string dice_skin_img_path = "Images\\dice_skin.png";
        const std::string office_skin_img_path = "Images\\office_skin.jpg";

        scenes.push_back(std::make_unique<SceneSolidCube>(frame_width, frame_height));
        scenes.push_back(std::make_unique<SceneCubeOrder>(frame_width, frame_height));
        scenes.push_back(std::make_unique<SceneConHex>(frame_width, frame_height));
        scenes.push_back(std::make_unique<SceneXMutual>(frame_width, frame_height));
        scenes.push_back(std::make_unique<SceneTexCube>(frame_width, frame_height, sauron_img_path, 1.0f));
        scenes.push_back(std::make_unique<SceneTexCube>(frame_width, frame_height, sauron_img_path, 2.0f));
        scenes.push_back(std::make_unique<SceneTexWrapCube>(frame_width, frame_height, sauron_img_path, 1.0f));
        scenes.push_back(std::make_unique<SceneTexWrapCube>(frame_width, frame_height, sauron_img_path, 2.0f));
        scenes.push_back(std::make_unique<SceneFoldedCube>(frame_width, frame_height, sauron_img_path));
        scenes.push_back(std::make_unique<SceneFoldedCubeWrap>(frame_width, frame_height, sauron_img_path));
        scenes.push_back(std::make_unique<SceneCubeSkinned>(frame_width, frame_height, dice_skin_img_path));
        scenes.push_back(std::make_unique<SceneCubeSkinned>(frame_width, frame_height, office_skin_img_path));
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
        }
        
        std::cout << "Cycling scene to : " << (*curr_scene)->GetSceneName() << "\n";
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
    const char* window_name = "3D Programming Fundamentals";
    
    try {
        Game game{window_name, frame_width, frame_height};
        game.Go();
    } catch(std::exception ex) {
        std::cout << "Exception occered : " << ex.what() << "\n";
    }

    return 0;
}
