#include "App.hpp"

void App::MarioInitial(){
    mario = std::make_shared<Mario>(800,400,MapDataHolder::GetMarioPosition(level),36.0f,48.0f);
    auto mario_ani = mario->GetAnimationObject();

    std::vector<std::string> mario_run;
    mario_run.reserve(3);
    for (int i = 0; i < 3; i++) {
        mario_run.emplace_back(RESOURCE_DIR"/Sprites/Mario/Small/mario_run" + std::to_string(i + 1) + ".png");
    }
    mario_ani->AddNewAnimation(mario_run);

    std::vector<std::string> mario_jump;
    mario_jump.emplace_back(RESOURCE_DIR"/Sprites/Mario/Small/mario_jump.png");
    mario_ani->AddNewAnimation(mario_jump);

    std::vector<std::string> mario_dead;
    mario_dead.emplace_back(RESOURCE_DIR"/Sprites/Mario/mario_dead.png");
    mario_ani->AddNewAnimation(mario_dead);

    mario_ani->SetZIndex(50);

    m_Renderer.AddChild(mario_ani);
}