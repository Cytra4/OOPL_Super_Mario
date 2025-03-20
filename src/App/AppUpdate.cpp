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
    auto m_ani = mario->GetAnimationObject();
    auto m_box = mario->GetBox();
    glm::vec2 m_pos = m_box.GetPosition();
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
    if (b_box.IsActive() && m_box.ifCollide(b_box)){
        CollisionBox::State m_state = m_box.GetCurrentState();
        b_box.ifCollide(m_box);
        if (m_state == CollisionBox::State::TOP){
            LOG_DEBUG("TOP");
            if (mario->GetMarioMode() != Mario::Mode::SMALL){
                LOG_DEBUG("TOP2");
                brick->ContactBehavior();
            }
            else{
                
            }
            float new_y = (m_box.GetPosition().y - m_box.GetHeight()/2);
            m_ani->SetPosition({m_pos.x, new_y});
            m_box.SetPosition({m_pos.x, new_y});
            auto m_velo = mario->GetVelocity();
            mario->SetVelocity({m_velo.x, 0});
            
        }
        else if (m_state == CollisionBox::State::BOTTOM){
            LOG_DEBUG("BOTTOM");
            mario->SetStandingOnBlock(brick->GetBox());
            mario->SetOnGround(true);
            flag = true;
        }
        else if (m_state == CollisionBox::State::RIGHT || m_state == CollisionBox::State::LEFT){
            float new_x;
            if (m_state == CollisionBox::State::RIGHT){
                LOG_DEBUG("RIGHT");
                new_x = (b_box.GetPosition().x - b_box.GetHeight()/2) - m_box.GetHeight()/2;
            }
            else{
                LOG_DEBUG("LEFT");
                new_x = (b_box.GetPosition().x + b_box.GetHeight()/2) + m_box.GetHeight()/2;
            }
            m_ani->SetPosition({new_x, m_pos.y});
            m_box.SetPosition({new_x, m_pos.y});
        }
    }
    if (brick->IsMarkedDestory()){m_Renderer.RemoveChild(brick->GetAnimationObject());}
    if (!flag){mario->SetOnGround(false);}

    //Barrier Test
    if (m_pos.x < barrier*-1+20){
        m_ani->SetPosition({barrier*-1+20,m_pos.y});
    }
    else if (m_pos.x > barrier-20){
        m_ani->SetPosition({barrier-20,m_pos.y});
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
    //LOG_DEBUG("Cam pos:{}",CameraPosition);

    FireballUpdate();
    CamPosAdjust();

    m_Renderer.Update(CameraPosition);
}