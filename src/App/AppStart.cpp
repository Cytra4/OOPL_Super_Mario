#include "App.hpp"

void App::Start(){

    MarioInitial();
    
    //This part that's all about map will all be moved to MapManager after I'm finished
    //Right now is just for testing
    background = std::make_shared<Background>(RESOURCE_DIR"/Maps/level1_1_pureBG.png");
    background->DrawBackground();

    testFloor = CollisionBox(glm::vec2{13.5f,-280.0f},76800,40);
    barrier = background->GetScaledSize().x/2;

    m_Renderer.AddChild(background);

    //Block testing
    glm::vec2 testing_pos = mario->GetAnimationObject()->GetPosition() + glm::vec2{50,60};
    brick = std::make_shared<Brick>(testing_pos, 48, 48);

    m_Renderer.AddChild(brick->GetAnimationObject());

    CameraPosition = {0.0f,0.0f};

    m_CurrentState = State::UPDATE;
}