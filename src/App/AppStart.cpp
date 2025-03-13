#include "App.hpp"

//this will be for initialize the game (for now i guess)
void App::Start(){

    MarioInitial();
    
    //This part that's all about map will all be moved to a function like MapInitialize()
    //maybe all the map related objects will be moved to a Map class or smth (i'm too old to read all the shits in App.hpp)
    background = std::make_shared<Background>(RESOURCE_DIR"/Maps/level1_1_pureBG.png");
    background->DrawBackground();

    testFloor = CollisionBox(glm::vec2{13.5f,-260.0f},76800,40);
    barrier = background->GetScaledSize().x/2;

    m_Renderer.AddChild(background);

    CameraPosition = {0.0f,0.0f};

    m_CurrentState = State::UPDATE;
}