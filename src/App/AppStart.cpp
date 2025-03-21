#include "App.hpp"

void App::Start(){

    MarioInitial();
    
    //This part that's all about map will all be moved to MapManager after I'm finished
    //Right now is just for testing
    background = std::make_shared<Background>(RESOURCE_DIR"/Maps/level1_1_pureBG.png");
    background->DrawBackground();

    testFloor = CollisionBox(glm::vec2{13.5f,-280.0f},76800,33);
    barrier = background->GetScaledSize().x/2;

    m_Renderer.AddChild(background);

    //Block testing
    glm::vec2 testing_pos = mario->GetAnimationObject()->GetPosition() + glm::vec2{50,60};
    brick = std::make_shared<Brick>(RESOURCE_DIR"/Sprites/Blocks/Brick1.png", testing_pos, 48, 48);
    testing_pos.x += 96;
    mystery_block = std::make_shared<MysteryBlock>(RESOURCE_DIR"/Sprites/Blocks/empty1.png", testing_pos, 48,48, 1);

    m_Renderer.AddChild(brick->GetAnimationObject());
    m_Renderer.AddChild(mystery_block->GetAnimationObject());

    CameraPosition = {0.0f,0.0f};

    m_CurrentState = State::UPDATE;
}