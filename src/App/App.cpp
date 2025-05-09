#include "App.hpp"

void App::CamPosAdjust(){
    glm::vec2 mario_pos = mario->GetAnimationObject()->GetPosition();
    glm::vec2 map_size = MManager->GetBackground()->GetScaledSize();

    float leftBound = map_size.x/2 * -1;
    float rightBound = map_size.x/2;

    if (mario_pos.x < leftBound + 384 && !camPassedFlag){
        CameraPosition.x = leftBound + 384;
    }
    else if (mario_pos.x > rightBound - 384){
        CameraPosition.x = rightBound - 384;
    }
    else{
        if (CameraPosition.x*-1 > mario_pos.x){
            return;
        }
        camPassedFlag = true;
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
            mario_initial = false;
        }
    }
    mario->AnimationHandle();
    m_Renderer.Update(CameraPosition);
}

void App::LevelClear(double time){
    auto ani_obj = mario->GetAnimationObject();
    if (ani_obj->GetCurrentAnimation() != 3){
        ani_obj->SetAnimation(3, 25);
        ani_obj->SetLooping(true);
        ani_obj->PlayAnimation();
        ani_obj->SetCurrentAnimation(3);
    }

    mario->SetVelocity({0.0f,-250.0f});
    if (ani_obj->GetPosition().y > -168){
        mario->PhysicProcess(time);
    }
    else{
        if (flagDowned){
            m_CurrentState = State::CLEAR_WALK_TO_CASTLE;
            flagDowned = false;
        }
    }

    auto f_pos = MManager->GetFlag()->GetFlagAniObj()->GetPosition();
    if (f_pos.y > -192){
        f_pos.y -= 5.5;
        MManager->GetFlag()->GetFlagAniObj()->SetPosition(f_pos);
    }
    else{
        auto f_pos = MManager->GetFlag()->GetFlagAniObj()->GetPosition();
        f_pos.y = -192;
        MManager->GetFlag()->GetFlagAniObj()->SetPosition(f_pos);
        flagDowned = true;
    }

    m_Renderer.Update(CameraPosition);
}

void App::ClearWalkToCastle(double time){
    auto castle_pos = MapDataHolder::GetCastlePosition(level);
    
    auto ani_obj = mario->GetAnimationObject();
    ani_obj->SetScale(glm::vec2{1,1});
    if (ani_obj->GetCurrentAnimation() != 0){
        ani_obj->SetAnimation(0, 25);
        ani_obj->SetLooping(true);
        ani_obj->PlayAnimation();
        ani_obj->SetCurrentAnimation(0);
    }

    if (mario->GetBox().GetPosition().x < castle_pos.x){
        auto m_velo = mario->GetVelocity();
        m_velo.x = 250.0f;
        mario->SetVelocity(m_velo);
        mario->PhysicProcess(time);
        CManager->SetCFlag(false);
        CManager->BlockCollisionProcess(mario, 0);
        CManager->OtherCollisionProcess(mario, 0);
        if (!CManager->GetCFlag()){mario->SetOnGround(false);}
        CamPosAdjust();
    }
    else{
        ani_obj->SetVisible(false);
        glm::vec2 flag_finaldes = MapDataHolder::GetCastleFlagPosition(level) + glm::vec2{0,96.0f};
        std::shared_ptr<AnimationObject> c_flag = MManager->GetCastleFlag();
        auto c_pos = c_flag->GetPosition();
        if (c_pos.y < flag_finaldes.y){
            c_pos.y += 3;
            c_flag->SetPosition(c_pos);
        }
        else{
            c_flag->SetPosition(flag_finaldes);
            prev_level = level;
            level = MapDataHolder::GetNextLevel(level);
            m_CurrentState = State::START;
        }
    }

    m_Renderer.Update(CameraPosition);
}

void App::PipeEnter(double time, std::string type){
    auto ani_obj = mario->GetAnimationObject();
    ani_obj->SetDefaultSprite(ani_obj->GetDefaultSprite());
    ani_obj->SetCurrentAnimation(-1);
    if (level == "1_1" || (level == "1_2" && type != "1_2C")){
        mario->SetVelocity({0.0f,-250.0f});
    }
    else{
        mario->SetVelocity({250.0f,0.0f});
    }

    mario->SetOnGround(false);
    if (pipe_enter_timer > 0){
        pipe_enter_timer -= time;
        mario->PhysicProcess(time);
    }
    else{
        pipe_enter_timer = 0.5f;
        prev_level = level;
        if (level != "1_2"){
            level = MapDataHolder::GetSecretLevel(level);
        }
        else{
            level = type;
        }
        m_CurrentState = State::START; 
    }

    m_Renderer.Update(CameraPosition);
}

void App::Level2Animation(double time){
    auto ani_obj = mario->GetAnimationObject();
    if (ani_obj->GetCurrentAnimation() != 0){
        ani_obj->SetAnimation(0, 25);
        ani_obj->SetLooping(true);
        ani_obj->PlayAnimation();
        ani_obj->SetCurrentAnimation(0);
    }

    if (ani_obj->GetPosition().x < 192){
        auto m_velo = mario->GetVelocity();
        m_velo.x = 150.0f;
        mario->SetVelocity(m_velo);
        mario->PhysicProcess(time);
        CManager->SetCFlag(false);
        CManager->OtherCollisionProcess(mario, 0);
        if (!CManager->GetCFlag()){mario->SetOnGround(false);}
    }
    else{
        prev_level = "1_2A";
        level = "1_2";
        m_CurrentState = State::START; 
    }

    m_Renderer.Update(CameraPosition);
}

std::string App::GetLevel(){
    return level;
}

std::shared_ptr<CollisionManager> App::GetCManager(){
    return CManager;
}