#include "App.hpp"

void App::MarioInitial(){
    mario = std::make_shared<Mario>(450,450,glm::vec2{-112.5f, -140.5f},36.0f,48.0f);

    std::vector<std::string> mario_run;
    mario_run.reserve(3);
    for (int i = 0; i < 3; i++) {
        mario_run.emplace_back(RESOURCE_DIR"/Sprites/Mario/Small/mario_run" + std::to_string(i + 1) + ".png");
    }
    mario->AddNewAnimation(mario_run);

    std::vector<std::string> mario_jump;
    mario_jump.emplace_back(RESOURCE_DIR"/Sprites/Mario/Small/mario_jump.png");
    mario->AddNewAnimation(mario_jump);

    std::vector<std::string> mario_dead;
    mario_dead.emplace_back(RESOURCE_DIR"/Sprites/Mario/mario_dead.png");
    mario->AddNewAnimation(mario_dead);

    mario->SetZIndex(50);
    m_Renderer.AddChild(mario);
}