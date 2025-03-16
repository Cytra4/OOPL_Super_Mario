#include "App.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

//Not sure if I'm gonna do all the behavior stuffs here, maybe will do it in another class?
void App::Update(){
    double deltaTime = m_Time.GetDeltaTimeMs();
    if (deltaTime > 0.02){
        deltaTime = 0.02;
    }

    bool flag = false;
    mario->Behavior();
    auto m_box = mario->GetBox();
    //Floor collision test
    if (m_box.ifCollide(testFloor)){
        if (m_box.GetCurrentState() == CollisionBox::State::BOTTOM){
            mario->SetStandingOnBlock(testFloor);
            mario->SetOnGround(true);
            flag = true;
        }
    }

    //Block collision test
    auto b_box = brick->GetBox();
    if (m_box.ifCollide(b_box)){
        LOG_DEBUG("SET");
        CollisionBox::State m_state = m_box.GetCurrentState();
        CollisionBox::State b_state = b_box.GetCurrentState();
        b_box.ifCollide(m_box);
        if (m_state == CollisionBox::State::TOP && b_state == CollisionBox::State::BOTTOM){
            if (mario->GetMarioMode() != Mario::Mode::SMALL){
                brick->ContactBehavior();
            }
        }
        else if (m_state == CollisionBox::State::BOTTOM){
            mario->SetStandingOnBlock(brick->GetBox());
            mario->SetOnGround(true);
            flag = true;
        }
    }
    if (brick->IsMarkedDestory()){m_Renderer.RemoveChild(brick->GetAnimationObject());}
    if (!flag){mario->SetOnGround(false);}

    //Barrier Test
    auto mario_ani = mario->GetAnimationObject();
    glm::vec2 m_pos = mario_ani->GetPosition();
    if (m_pos.x < barrier*-1+20){
        mario_ani->SetPosition({barrier*-1+20,m_pos.y});
    }
    else if (m_pos.x > barrier-20){
        mario_ani->SetPosition({barrier-20,m_pos.y});
    }

    mario->PhysicProcess(deltaTime);

    if (Util::Input::IsKeyPressed(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }


    //Mario mode testing
    if (Util::Input::IsKeyPressed(Util::Keycode::Z)){
        mario->StateUpdate(Mario::Mode::SMALL);
    }
    else if (Util::Input::IsKeyPressed(Util::Keycode::X)){
        mario->StateUpdate(Mario::Mode::BIG);
    }
    else if (Util::Input::IsKeyPressed(Util::Keycode::C)){
        mario->StateUpdate(Mario::Mode::FIRE);
    }
    //LOG_DEBUG("Mario's pos:{}",mario->GetPosition());
    //LOG_DEBUG("Cam pos:{}",CameraPosition);

    FireballUpdate();
    CamPosAdjust();

    m_Renderer.Update(CameraPosition);
}