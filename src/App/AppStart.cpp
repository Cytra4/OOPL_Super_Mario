#include "App.hpp"

void App::Start(){

    MarioInitial();
    
    MManager = std::make_shared<MapManager>(level);
    MManager->MapDataInitialize();
    MManager->DrawMap(m_Renderer);

    CManager = std::make_shared<CollisionManager>(mario, MManager->GetBlocks(), MManager->GetPipes(), 
    MManager->GetFloors(), MManager->GetBackground()->GetScaledSize());
    CManager->SetGoombas(MManager->GetGoombas());

    CameraPosition = {0.0f,0.0f};

    m_CurrentState = State::UPDATE;
}