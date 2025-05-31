#include "App.hpp"

void App::Start(){
    if (!mario_initial){
        MarioInitial();
        mario_initial = true;
        mario->SetCoin(SManager->GetMCoin());
        mario->SetScore(SManager->GetMScore());
        mario->SetLive(SManager->GetMLive());
    }
    mario->GetAnimationObject()->SetVisible(true);
    
    SManager->ShowTimer();
    
    if (prev_level != level){
        glm::vec2 new_pos = MapDataHolder::GetMarioPosition(level);
        mario->GetAnimationObject()->SetPosition(new_pos);
        mario->GetBox().SetPosition(new_pos);
        if (prev_level == "1_1A"){
            mario->GetAnimationObject()->SetPosition(glm::vec2{2808,-120});
            mario->GetBox().SetPosition(glm::vec2{2808,-120});
        }
        else if (prev_level == "1_2B"){
            mario->GetAnimationObject()->SetPosition(glm::vec2{960,-168});
            mario->GetBox().SetPosition(glm::vec2{960,-168});
        }
    }

    if (!m_initialed){
        MManager = std::make_shared<MapManager>(level);
        m_initialed = true;
    }
    else{
        MManager->SwitchLevel(level);
        MManager->ClearMap(m_Renderer);
    }
    MManager->MapDataInitialize();
    MManager->DrawMap(m_Renderer);

    CManager = std::make_shared<CollisionManager>(mario, MManager->GetBlocks(), MManager->GetPipes(), 
    MManager->GetFloors(), MManager->GetBackground()->GetScaledSize());

    if (level != "1_1A"){
        CManager->SetGoombas(MManager->GetGoombas());
        CManager->SetKoopas(MManager->GetKoopas());
        CManager->SetFlag(MManager->GetFlag());
    }
    if (MManager->GetItems().size() > 0){
        CManager->SetItem(MManager->GetItems());
    }
    if (MManager->GetPiranhas().size() > 0){
        CManager->SetPiranhas(MManager->GetPiranhas());
    }
    if (MManager->GetFirebars().size() > 0){
        CManager->SetFirebar(MManager->GetFirebars());
    }
    rendered = true;

    CameraPosition = {0.0f,0.0f};
    camPassedFlag = false;
    
    if (level == "1_4"){
        CManager->SetRBarrier(MManager->GetBackground()->GetScaledSize().x/2 - 816);
        CManager->SetBowser(MManager->GetBowser());
    }

    if (level == "1_2A" && prev_level == "1_1"){
        SManager->ResetTimer();
    }
    else if (level == "1_4"){
        SManager->ResetTimer();
    }

    SManager->levelUpdate(level);
    SManager->PlayBGM();

    if (level == "1_2A"){
        m_CurrentState = State::LEVEL2_ANIMATION;
    }
    else{
        m_CurrentState = State::UPDATE;
    }
}