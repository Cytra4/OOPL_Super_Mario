#include "App.hpp"

void App::CamPosAdjust(){
    glm::vec2 mario_pos = mario->GetAnimationObject()->GetPosition();
    glm::vec2 map_size = MManager->GetBackground()->GetScaledSize();

    float leftBound = map_size.x/2 * -1;
    float rightBound = map_size.x/2;

    if (mario_pos.x < leftBound + 384){
        CameraPosition.x = leftBound + 384;
    }
    else if (mario_pos.x > rightBound - 384){
        CameraPosition.x = rightBound - 384;
    }
    else{
        CameraPosition.x = mario_pos.x;
    }

    CameraPosition *= -1;
}

void App::MarioDeath(double time){
    m_death_timer -= time;
    if (m_death_timer <= 0){
        if (mario->GetBox().GetPosition().y > -450){
            mario->PhysicProcess(time/1.5);
        }
        else{
            m_CurrentState = State::START;
            m_death_timer = 0.5f;
        }
    }
    mario->AnimationHandle();
    m_Renderer.Update(CameraPosition);
}