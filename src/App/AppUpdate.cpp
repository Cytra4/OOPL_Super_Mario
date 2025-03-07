#include "App.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

//maybe all the controls won't be put here, but instead only execute the behavior function
void App::Update(){
    double deltaTime = m_Time.GetDeltaTimeMs();
    if (deltaTime > 0.02){
        deltaTime = 0.02;
    }

    mario->Behavior();
    if (mario->GetBox().ifCollide(testFloor)){
        if (mario->GetBox().GetCurrentState() == CollisionBox::State::COLLISION_ON_BOTTOM){
            mario->SetStandingOnBlock(testFloor);
        }
    }

    mario->PhysicProcess(deltaTime);

    if (Util::Input::IsKeyPressed(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    // LOG_DEBUG("Mario's pos:{}",mario->GetPosition());
    // LOG_DEBUG("Mario's Box pos:{}",mario->GetBox().GetPosition());

    if (Util::Input::IsKeyPressed(Util::Keycode::Z)){
        mario->StateUpdate(Mario::Mode::SMALL);
    }
    else if (Util::Input::IsKeyPressed(Util::Keycode::X)){
        mario->StateUpdate(Mario::Mode::BIG);
    }
    else if (Util::Input::IsKeyPressed(Util::Keycode::C)){
        mario->StateUpdate(Mario::Mode::FIRE);
    }

    m_Renderer.Update();
}