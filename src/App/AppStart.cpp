#include "App.hpp"

void App::Start(){

    MarioInitial();

    testFloor = CollisionBox(glm::vec2{13.5f,-280.0f},76800,33);

    //m_Renderer.AddChild(background);

    //Block testing
    glm::vec2 testing_pos = mario->GetAnimationObject()->GetPosition() + glm::vec2{50,60};
    testing_pos.x += 96;

    //MapManager test
    MManager = std::make_shared<MapManager>(level);
    MManager->MapDataInitialize();
    MManager->DrawMap(m_Renderer);

    //CollisionManager test
    std::vector<CollisionBox> floor_boxes;
    floor_boxes.push_back(testFloor);
    CManager = std::make_shared<CollisionManager>(mario, MManager->GetBlocks(), floor_boxes, MManager->GetBackground()->GetScaledSize());

    CameraPosition = {0.0f,0.0f};

    m_CurrentState = State::UPDATE;
}