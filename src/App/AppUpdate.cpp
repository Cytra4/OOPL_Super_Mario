#include "App.hpp"

//Not sure if I'm gonna do all the behavior stuffs here, maybe will do it in another class?
void App::Update(){
    double deltaTime = m_Time.GetDeltaTimeMs();
    if (deltaTime > 0.02){
        deltaTime = 0.02;
    }

    //*A Important note here
    //CManager's UpdateProcess() *HAS* to be executed before PhysicProcess() of character
    //(Not sure if it's the same for other character, but for Mario it has to be this way)
    mario->Behavior();
    CManager->UpdateProcess(deltaTime, -CameraPosition);
    mario->PhysicProcess(deltaTime);
    MManager->OutOfRangeMarkDestroy(-CameraPosition);
    MManager->DestroyMarkedObject(m_Renderer);
    MManager->UpdateMap(m_Renderer, CManager, mario);
    SManager->UIUpdate(mario->GetScore(),mario->GetCoin(),0.03,level);

    if (mario->IsDead() || mario->GetBox().GetPosition().y < -400){
        m_CurrentState = State::MARIO_DEATH;
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::J)){
        mario->ShootFireball();
    }

    //Mario mode
    if (Util::Input::IsKeyPressed(Util::Keycode::Z)){
        mario->StateUpdate(Mario::Mode::SMALL);
    }
    else if (Util::Input::IsKeyPressed(Util::Keycode::X)){
        mario->StateUpdate(Mario::Mode::BIG);
    }
    else if (Util::Input::IsKeyPressed(Util::Keycode::C)){
        mario->StateUpdate(Mario::Mode::FIRE);
    }

    if (CManager->IsLevelCleared()){
        m_CurrentState = State::LEVEL_CLEAR;
        mario->AddScore(5000);
        CManager->ResetStates();
    }
    else if (CManager->EnterSecretLevel()){
        m_CurrentState = State::PIPE_ENTER;
        CManager->ResetStates();
    }
    else if (CManager->IsGameCleared()){
        mario->GetAnimationObject()->SetPosition(glm::vec2{2952,-24});
        mario->GetBox().SetPosition(glm::vec2{2952,-24});
        if (MManager->GetBowser().size() > 0){
            m_CurrentState = State::BRIDGE_CLEAR;
        }
        else{
            m_CurrentState = State::GAME_CLEAR;
        }
        CManager->ResetStates();
    }

    if (SManager->IsTimeUp()){
        m_CurrentState = State::MARIO_DEATH;
        mario->SetDead(true);
        mario->SetVelocity(glm::vec2{0.0f,1000.0f});
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    //LOG_DEBUG("CAMERA:{} MARIO:{}",CameraPosition,mario->GetBox().GetPosition());
    //LOG_DEBUG("MANAGER:{}  MARIO:{}",SManager->GetMLive(),mario->GetLive());
    
    CManager->UpdateLBarrier(level);
    CamPosAdjust();
    SManager->UIPositionUpdate(-CameraPosition);
    m_Renderer.Update(CameraPosition);
}