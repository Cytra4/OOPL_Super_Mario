#include "App.hpp"

void App::TitleScreen(){
    if (!s_initialed){
        SManager = std::make_shared<SystemManager>();
        s_initialed = true;
    }
    SManager->ShowTitleScreen(m_Renderer);
    SManager->ShowUI(m_Renderer);
    SManager->PauseBGM();
    SManager->HideTimer();
    CameraPosition = {0,0};
    if (Util::Input::IsKeyPressed(Util::Keycode::RETURN)) {
        SManager->ClearTitleScreen(m_Renderer);
        m_CurrentState = State::LOADING;
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    SManager->UIPositionUpdate(CameraPosition);
    m_Renderer.Update(CameraPosition);
}

void App::Loading(double time){
    SManager->ShowLoadingScreen(m_Renderer);
    SManager->HideTimer();
    load_timer -= time;
    if (load_timer <= 0){
        load_timer = 4.0f;
        SManager->ClearLoadingScreen(m_Renderer);
        m_CurrentState = State::START;
    }
    CameraPosition = {0,0};
    SManager->UIPositionUpdate(-CameraPosition);
    m_Renderer.Update(CameraPosition);
}

void App::CamPosAdjust(){
    glm::vec2 mario_pos = mario->GetAnimationObject()->GetPosition();
    glm::vec2 map_size = MManager->GetBackground()->GetScaledSize();

    float leftBound = map_size.x/2 * -1;
    float rightBound = map_size.x/2;

    if (mario_pos.x < leftBound + 384 && !camPassedFlag){
        CameraPosition.x = leftBound + 384;
    }
    else if (level == "1_4" && m_CurrentState != State::GAME_CLEAR && mario_pos.x > rightBound - 1200){
        CameraPosition.x = rightBound - 1200;
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
    if (m_death_timer == 0.5f){
        std::shared_ptr<Util::SFX> se = mario->Get_SE();
        se->LoadMedia(RESOURCE_DIR"/SoundEffects/Mario/death.wav");
        se->SetVolume(80);
        se->Play();

        SManager->PauseBGM();
    }
    m_death_timer -= time;
    if (m_death_timer <= 0){
        if (mario->GetBox().GetPosition().y > -500){
            mario->PhysicProcess(time/1.5);
        }
        else{
            int new_live = mario->GetLive() - 1;
            mario->SetLive(new_live);
            if (new_live > 0){
                SManager->MLiveUpdate(mario->GetLive());
                if (SManager->IsTimeUp()){
                    m_CurrentState = State::TIME_UP;
                }
                else{
                    m_CurrentState = State::LOADING;
                }
            }
            else{
                SManager->MLiveUpdate(mario->GetLive());
                if (SManager->IsTimeUp()){
                    m_CurrentState = State::TIME_UP;
                }
                else{
                    m_CurrentState = State::GAME_OVER;
                }
            }
            SManager->ResetTimer();
            m_death_timer = 0.5f;
            mario_initial = false;
        }
    }
    mario->AnimationHandle();
    m_Renderer.Update(CameraPosition);
}

void App::TimeUp(double time){
    SManager->HideTimer();
    SManager->ShowTimeUpScreen(m_Renderer);

    load_timer -= time;
    if (load_timer <= 0){
        if (SManager->GetMLive() > 0){
            m_CurrentState = State::LOADING;
        }
        else{
            m_CurrentState = State::GAME_OVER;
        }
        SManager->ClearTimeUpScreen(m_Renderer);
        load_timer = 4.0f;
    }

    CameraPosition = {0,0};
    SManager->UIPositionUpdate(-CameraPosition);
    m_Renderer.Update(CameraPosition);
}

void App::GameOver(double time){
    if (game_over_timer == 5.0f){
        SManager->PlayGameOver();
    }
    SManager->HideTimer();
    SManager->ShowGameOverScreen(m_Renderer);
    game_over_timer -= time;
    if (game_over_timer <= 0){
        game_over_timer = 5.0f;
        prev_level = "0";
        level = "1_1";
        SManager->ClearGameOverScreen(m_Renderer);
        SManager->ResetTimer();
        SManager->ResetMVariables();
        SManager->UIUpdate(0,0,0,level);
        m_CurrentState = State::TITLE_SCREEN;
    }
    CameraPosition = {0,0};
    SManager->UIPositionUpdate(-CameraPosition);
    m_Renderer.Update(CameraPosition);
}

void App::LevelClear(double time){
    auto ani_obj = mario->GetAnimationObject();
    if (ani_obj->GetCurrentAnimation() != 3){
        ani_obj->SetAnimation(3, 25);
        ani_obj->SetLooping(true);
        ani_obj->PlayAnimation();
        ani_obj->SetCurrentAnimation(3);
        SManager->UIUpdate(mario->GetScore(),mario->GetCoin(),0,level);

        auto se = mario->Get_SE();
        se->LoadMedia(RESOURCE_DIR"/SoundEffects/Mario/flagpole.wav");
        se->SetVolume(80);
        se->Play();
    }

    mario->SetVelocity({0.0f,-250.0f});
    if (ani_obj->GetPosition().y > -168){
        mario->PhysicProcess(time);
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

    if (flagDowned){
            m_CurrentState = State::CLEAR_WALK_TO_CASTLE;
            flagDowned = false;
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

        SManager->PlayComplete();
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
        SManager->UIPositionUpdate(-CameraPosition);
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
            SManager->MLiveUpdate(mario->GetLive());
            m_CurrentState = State::ADD_REMAIN_TIME_TO_SCORE;
        }
    }

    m_Renderer.Update(CameraPosition);
}

void App::AddTimeToScore(double time){
    if (SManager->GetTime() > 0){
        SManager->UIUpdate(mario->GetScore(),mario->GetCoin(),time,level);
        SManager->PlayBeep();
        mario->AddScore(50);
    }
    else{
        m_CurrentState = State::LOADING;
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

    if (ani_obj->GetPosition().x >= 96 && !played1_2){
        auto se = mario->Get_SE();
        se->LoadMedia(RESOURCE_DIR"/SoundEffects/Mario/pipepowerdown.wav");
        se->SetVolume(80);
        se->Play();
        played1_2 = true;
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
        played1_2 = false;
        m_CurrentState = State::START; 
    }

    SManager->UIPositionUpdate(-CameraPosition);
    m_Renderer.Update(CameraPosition);
}

void App::DeleteBridge(double time){
    bridge_delete_timer -= time;
    if (bridge_delete_timer <= 0){
        if (MManager->GetCastleBridge().size() > 0){
            MManager->RemoveBridge(m_Renderer);
            bridge_delete_timer = 0.1f;
        }
        else{
            auto bowser = MManager->GetBowser()[0];
            if (!bowser->IsDead()){
                auto se = bowser->GetSE();
                se->LoadMedia(RESOURCE_DIR"/SoundEffects/Enemy/bowserfall.wav");
                se->SetVolume(80);
                se->Play();
            }

            if (bowser->GetBox().GetPosition().y > -450){
                bowser->SetDead(true);
                bowser->SetOnGround(false);
                bowser->PhysicProcess(time);
            }
            else{
                m_CurrentState = State::GAME_CLEAR;
            }
        }
    }
    m_Renderer.Update(CameraPosition);
}

void App::GameClear(double time){
    CManager->SetRBarrier(MManager->GetBackground()->GetScaledSize().x/2-20);
    auto ani_obj = mario->GetAnimationObject();
    
    if (game_clear_timer == 9.0f){
        SManager->PlayComplete();
    }

    if (ani_obj->GetPosition().x < 3432){
        ani_obj->SetScale(glm::vec2{1,1});
        if (ani_obj->GetCurrentAnimation() != 0){
            ani_obj->SetAnimation(0, 25);
            ani_obj->SetLooping(true);
            ani_obj->PlayAnimation();
            ani_obj->SetCurrentAnimation(0);
        }
        auto m_velo = mario->GetVelocity();
        m_velo.x = 250.0f;
        mario->SetVelocity(m_velo);
        mario->PhysicProcess(time);
        CManager->SetCFlag(false);
        CManager->OtherCollisionProcess(mario, 0);
        if (!CManager->GetCFlag()){mario->SetOnGround(false);}
        CamPosAdjust();
    }
    else{
        mario->GetAnimationObject()->SetPosition(glm::vec2{3432,ani_obj->GetPosition().y});
        mario->GetBox().SetPosition(glm::vec2{3432,ani_obj->GetPosition().y});
        CameraPosition = glm::vec2{-3456,0};
        if (ani_obj->GetCurrentAnimation() != -1){
            ani_obj->SetDefaultSprite(ani_obj->GetDefaultSprite());
            ani_obj->SetCurrentAnimation(-1);
        }

        game_clear_timer -= time;
        if (game_clear_timer <= 0){
            game_clear_timer = 9.0f;
            SManager->ClearGameClearMessage(m_Renderer);
            SManager->TopScoreUpdate(mario->GetScore());
            SManager->ResetMVariables();
            SManager->ResetTimer();
            SManager->UIUpdate(0,0,0,"1_1");
            m_Renderer.RemoveChild(mario->GetAnimationObject());
            mario_initial = false;
            prev_level = "0";
            level = "1_1";
            m_CurrentState = State::TITLE_SCREEN;
        }
        else if (game_clear_timer <= 4.5){
            SManager->ShowGameClearMessage(m_Renderer,1);
        }
        else{
            SManager->ShowGameClearMessage(m_Renderer,0);
        }
    }

    SManager->UIPositionUpdate(-CameraPosition);
    m_Renderer.Update(CameraPosition);
}

std::string App::GetLevel(){
    return level;
}

std::shared_ptr<CollisionManager> App::GetCManager(){
    return CManager;
}