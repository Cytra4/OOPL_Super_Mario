#include "App.hpp"

//this will be for initialize the game (for now i guess)
void App::Start(){

    MarioInitial();
    
    background = std::make_shared<Background>(RESOURCE_DIR"/Maps/level1_1_pureBG.png");
    background->DrawBackground();

    testFloor = CollisionBox(glm::vec2{13.5f,-260.0f},768,40);

    m_Renderer.AddChild(background);

    m_CurrentState = State::UPDATE;
}