#include "GameUtils/CollisionManager.hpp"
#include "Util/Logger.hpp"
#include "Util/Input.hpp"

CollisionManager::CollisionManager(std::shared_ptr<Mario> mario, 
std::vector<std::shared_ptr<Block>> blocks, std::vector<std::shared_ptr<Pipe>> pipes, 
std::vector<CollisionBox> floor_boxes, glm::vec2 mapSize){
    this->mario = mario;
    this->blocks = blocks;
    this->pipes = pipes;
    this->floor_boxes = floor_boxes;
    map_barriers.push_back(mapSize.x/2*-1+20);
    map_barriers.push_back(mapSize.x/2-20);
}

void CollisionManager::UpdateProcess(double time, glm::vec2 CameraPosition){
    c_flag = false;
    BlockCollisionProcess(mario, 0);
    OtherCollisionProcess(mario, 0);
    if (!c_flag){mario->SetOnGround(false);}
    EnemyCollisionProcess();
    ItemCollisionProcess();

    for (int i=0;i<int(goombas.size());i++){
        if (EnemyInCameraRange(goombas[i], mario->GetBox().GetPosition()) && !goombas[i]->IsActive()){
            goombas[i]->SetActive();
        }

        if (goombas[i]->IsActive()){
            goombas[i]->Behavior();
            c_flag = false;
            if (goombas[i]->GetDeath() != 2){
                BlockCollisionProcess(goombas[i], 1);
                OtherCollisionProcess(goombas[i], 1);
                EECollisionProcess(goombas[i], i);
            }
            if (!c_flag){goombas[i]->SetOnGround(false);}
            goombas[i]->PhysicProcess(time);
        }
    }

    for (int i=0;i<int(koopas.size());i++){
        if (EnemyInCameraRange(koopas[i], mario->GetBox().GetPosition()) && !koopas[i]->IsActive()){
            koopas[i]->SetActive();
        }
        if (koopas[i]->IsActive()){
            koopas[i]->Behavior();
            c_flag = false;
            if (koopas[i]->GetDeath() != 2){
                BlockCollisionProcess(koopas[i], 1);
                OtherCollisionProcess(koopas[i], 1);
                EECollisionProcess(koopas[i], i);
            }
            if (!c_flag){koopas[i]->SetOnGround(false);}
            koopas[i]->PhysicProcess(time);
        }
    }

    for (int i=0;i<int(piranhas.size());i++){
        if (EnemyInCameraRange(piranhas[i], mario->GetBox().GetPosition()) && !piranhas[i]->IsActive()){
            piranhas[i]->SetActive();
        }
        if (piranhas[i]->IsActive()){
            piranhas[i]->PhysicProcess(time);
        }
    }

    for (int i=0;i<int(bowser.size());i++){
        if (EnemyInCameraRange(bowser[i], mario->GetBox().GetPosition()) && !bowser[i]->IsActive()){
            bowser[i]->SetActive();
        }
        if (bowser[i]->IsActive()){
            c_flag = false;
            bowser[i]->Behavior();
            if (bowser[i]->GetDeath() != 1){
                bowser[i]->FaceToMario(mario);
                BlockCollisionProcess(bowser[i], 1);
            }
            if (!c_flag){bowser[i]->SetOnGround(false);}
            bowser[i]->PhysicProcess(time);
        }        
    }

    for (int i=0;i<int(items.size());i++){
        items[i]->PhysicProcess(time);
        if (items[i]->IsReachedTop()){
            i_flag = false;
            BlockCollisionProcess(items[i]);
            OtherCollisionProcess(items[i]);
            if (!i_flag){items[i]->SetOnGround(false);}
        }
    }

    int fb_size = m_fireballs.size();
    for (int i = 0; i < fb_size; i++) {
        auto fb = m_fireballs.front();
        m_fireballs.pop();
        fb->Behavior(time);
        OtherCollisionProcess(fb,0);
        BlockCollisionProcess(fb);
        if (fb->OutOfRange(CameraPosition)) {
            fb->MarkRemove();
        }
        m_fireballs.push(fb);
    }

    int b_fb_size = b_fireballs.size();
    for (int i = 0; i < b_fb_size; i++) {
        auto fb = b_fireballs.front();
        b_fireballs.pop();
        fb->Behavior(time);
        OtherCollisionProcess(fb,1);
        if (fb->OutOfRange(CameraPosition)) {
            fb->MarkRemove();
        }
        b_fireballs.push(fb);
    }

    for (int i=0;i<int(blocks.size());i++){
        if (blocks[i]->IsJumping()){
            blocks[i]->PhysicProcess(time);
        }
    }

    RemoveMarkedObject();
}

void CollisionManager::BlockCollisionProcess(std::shared_ptr<Character> character, int mode){
    auto c_box = character->GetBox();
    auto c_ani = character->GetAnimationObject();
    glm::vec2 c_pos = c_ani->GetPosition();

    for (int i=0;i<int(blocks.size());i++){
        auto b_box = blocks[i]->GetBox();
        if (b_box.IsActive() && c_box.ifCollide(b_box)){
            CollisionBox::State c_state = c_box.GetCurrentState();
            b_box.ifCollide(c_box);
            if (mode == 0){
                if (c_state == CollisionBox::State::TOP && mario->GetVelocity().y > 0){
                    blocks[i]->ContactBehavior(mario->GetMarioMode() != Mario::Mode::SMALL, mario);
                    float new_y = (b_box.GetPosition().y - b_box.GetHeight()/2) - c_box.GetHeight()/2;
                    auto c_velo = character->GetVelocity();
                    c_ani->SetPosition({c_pos.x, new_y});
                    c_box.SetPosition({c_pos.x, new_y});
                    character->SetVelocity({c_velo.x, 0});

                    //Handling if a enemy is on top of the block
                    double range1 = b_box.GetPosition().x + b_box.GetWidth()/2;
                    double range2 = b_box.GetPosition().x - b_box.GetWidth()/2;
                    for (int j = 0;j<int(goombas.size());j++){
                        auto g_pos = goombas[j]->GetBox().GetPosition();
                        if (g_pos.y > b_box.GetPosition().y && (g_pos.x < range1 && g_pos.x > range2)){
                            goombas[j]->SetDeath(2);
                            if (goombas[j]->GetBox().GetPosition().x > b_box.GetPosition().x){
                                goombas[j]->SetVelocity(glm::vec2{100,200});
                            }
                            else{
                                goombas[j]->SetVelocity(glm::vec2{-100,200});
                            }

                            auto se = mario->Get_SE();
                            se->LoadMedia(RESOURCE_DIR"/SoundEffects/Enemy/kickkill.wav");
                            se->SetVolume(80);
                            se->Play();
                        }
                    }

                    for (int j = 0;j<int(koopas.size());j++){
                        auto k_pos = koopas[j]->GetBox().GetPosition();
                        if (k_pos.y > b_box.GetPosition().y && (k_pos.x < range1 && k_pos.x > range2)){
                            koopas[j]->SetDeath(2);
                            if (koopas[j]->GetBox().GetPosition().x > b_box.GetPosition().x){
                                koopas[j]->SetVelocity(glm::vec2{100,200});
                            }
                            else{
                                koopas[j]->SetVelocity(glm::vec2{-100,200});
                            }

                            auto se = koopas[i]->GetSE();
                            se->LoadMedia(RESOURCE_DIR"/SoundEffects/Enemy/kickkill.wav");
                            se->SetVolume(80);
                            se->Play();
                        }
                    }
                }
            }
            if (c_state == CollisionBox::State::BOTTOM){
                character->SetStandingOnBlock(b_box);
                character->SetOnGround(true);
                c_flag = true;
            }
            else if (c_state == CollisionBox::State::RIGHT || c_state == CollisionBox::State::LEFT){
                float new_x;
                blocks[i]->ContactBehavior(-1,mario);
                if (c_state == CollisionBox::State::RIGHT){
                    if (mode == 1){
                        character->SetFacingRight(false);
                    }
                    new_x = (b_box.GetPosition().x - b_box.GetWidth()/2) - c_box.GetWidth()/2;
                }
                else{
                    if (mode == 1){
                        character->SetFacingRight(true);
                    }
                    new_x = (b_box.GetPosition().x + b_box.GetWidth()/2) + c_box.GetWidth()/2;
                }
                c_ani->SetPosition({new_x, c_pos.y});
                c_box.SetPosition({new_x, c_pos.y});
            }
        }
    }
}

void CollisionManager::BlockCollisionProcess(std::shared_ptr<Item> item){
    auto i_box = item->GetBox();
    auto i_ani = item->GetAnimationObject();
    glm::vec2 i_pos = i_ani->GetPosition();
    for (int i=0;i<int(blocks.size());i++){
        auto b_box = blocks[i]->GetBox();
        if (b_box.IsActive() && i_box.ifCollide(b_box)){
            CollisionBox::State i_state = i_box.GetCurrentState();
            b_box.ifCollide(i_box);
            if (i_state == CollisionBox::State::BOTTOM){
                item->SetStandingOnBlock(b_box);
                item->SetOnGround(true);
                i_flag = true;
            }
            else if (i_state == CollisionBox::State::RIGHT || i_state == CollisionBox::State::LEFT){
                float new_x;
                if (i_state == CollisionBox::State::RIGHT){
                    new_x = (b_box.GetPosition().x - b_box.GetWidth()/2) - i_box.GetWidth()/2;
                }
                else{
                    new_x = (b_box.GetPosition().x + b_box.GetWidth()/2) + i_box.GetWidth()/2;
                }
                i_ani->SetPosition({new_x, i_pos.y});
                i_box.SetPosition({new_x, i_pos.y});
            }
        }
    }
}

void CollisionManager::BlockCollisionProcess(std::shared_ptr<Fireball> fb){
    auto f_box = fb->GetBox();
    auto f_ani = fb->GetAnimationObject();
    for (int i=0;i<int(blocks.size());i++){
        auto b_box = blocks[i]->GetBox();
        if (b_box.IsActive() && f_box.ifCollide(b_box)){
            CollisionBox::State f_state = f_box.GetCurrentState();
            b_box.ifCollide(f_box);
            if (f_state == CollisionBox::State::BOTTOM){
                glm::vec2 f_velo = fb->GetVelocity();
                f_velo.y = fb->GetJumppower();
                fb->SetVelocity(f_velo);
                break;
            }
            else if (f_state == CollisionBox::State::RIGHT || f_state == CollisionBox::State::LEFT){
                fb->SetExplode();
            }
        }
    }
}

void CollisionManager::OtherCollisionProcess(std::shared_ptr<Character> character, int mode){
    auto c_ani = character->GetAnimationObject();
    auto c_box = character->GetBox();
    glm::vec2 c_pos = c_box.GetPosition();
    
    for (int i=0;i<int(floor_boxes.size());i++){
        if (c_box.ifCollide(floor_boxes[i])){
            CollisionBox::State state = c_box.GetCurrentState(); 
            if (state == CollisionBox::State::BOTTOM){
                character->SetStandingOnBlock(floor_boxes[i]);
                character->SetOnGround(true);
                c_flag = true;
                break;
            }
            else if (state == CollisionBox::State::RIGHT || state == CollisionBox::State::LEFT){
                float new_x;
                if (state == CollisionBox::State::RIGHT){
                    new_x = (floor_boxes[i].GetPosition().x - floor_boxes[i].GetWidth()/2) - c_box.GetWidth()/2;
                }
                else{
                    new_x = (floor_boxes[i].GetPosition().x + floor_boxes[i].GetWidth()/2) + c_box.GetWidth()/2;
                }
                c_ani->SetPosition({new_x, c_pos.y});
                c_box.SetPosition({new_x, c_pos.y});     
            }
            else{
                float new_y = (floor_boxes[i].GetPosition().y - floor_boxes[i].GetHeight()/2) - c_box.GetHeight()/2;
                auto c_velo = character->GetVelocity();
                c_ani->SetPosition({c_pos.x, new_y});
                c_box.SetPosition({c_pos.x, new_y});
                character->SetVelocity({c_velo.x, 0});
            }  
        }
    }

    if (c_pos.x < map_barriers[0]){
        if (mode == 0){
            c_ani->SetPosition({map_barriers[0],c_pos.y});
        }
        else{
            character->MarkDestroy();
        }
    }
    else if (c_pos.x > map_barriers[1]){
        if (mode == 0){
            c_ani->SetPosition({map_barriers[1],c_pos.y});
        }
        else{
            character->MarkDestroy();
        }
    }

    for (int i=0;i<int(pipes.size());i++){
        auto p_box = pipes[i]->GetBox();
        if (pipes[i]->GetHeight() == 5){
            continue;
        }

        if (c_box.ifCollide(p_box)){
            CollisionBox::State c_state = c_box.GetCurrentState();
            if (c_state == CollisionBox::State::BOTTOM){
                character->SetStandingOnBlock(p_box);
                character->SetOnGround(true);
                c_flag = true;
                if (pipes[i]->CanBeEnter() && Util::Input::IsKeyPressed(Util::Keycode::S) && pipes[i]->GetHeight() != 4){
                    enter_secret_level = true;
                    pipeEnterLevel = pipes[i]->GetNextLevel();
                    auto m_pos = c_box.GetPosition();
                    m_pos.x = p_box.GetPosition().x;
                    c_box.SetPosition(m_pos);
                    character->GetAnimationObject()->SetPosition(m_pos);

                    auto se = mario->Get_SE();
                    se->LoadMedia(RESOURCE_DIR"/SoundEffects/Mario/pipepowerdown.wav");
                    se->SetVolume(80);
                    se->Play();
                }
            }
            else if (c_state == CollisionBox::State::RIGHT || c_state == CollisionBox::State::LEFT){
                float new_x;

                if (c_state == CollisionBox::State::RIGHT){
                    if (mode == 1){
                        character->SetFacingRight(false);
                    }
                    new_x = (p_box.GetPosition().x - p_box.GetWidth()/2) - c_box.GetWidth()/2;

                    if (pipes[i]->CanBeEnter() && Util::Input::IsKeyPressed(Util::Keycode::D) && character->IsOnGround()){
                        if (pipes[i]->GetHeight() == 4 || pipes[i]->GetHeight() == 6){
                            enter_secret_level = true;
                            pipeEnterLevel = pipes[i]->GetNextLevel();
                            auto m_pos = c_box.GetPosition();
                            m_pos.y = p_box.GetPosition().y;
                            c_box.SetPosition(m_pos);
                            character->GetAnimationObject()->SetPosition(m_pos);
                            auto se = mario->Get_SE();
                            se->LoadMedia(RESOURCE_DIR"/SoundEffects/Mario/pipepowerdown.wav");
                            se->SetVolume(80);
                            se->Play();
                        }
                    }
                }
                else{
                    if (mode == 1){
                        character->SetFacingRight(true);
                    }
                    new_x = (p_box.GetPosition().x + p_box.GetWidth()/2) + c_box.GetWidth()/2;
                }
                c_ani->SetPosition({new_x, c_pos.y});
                c_box.SetPosition({new_x, c_pos.y});
            }
        }
    }

    if (mode == 0){
        for (int i=0;i<int(firebars.size());i++){
            auto fbs = firebars[i]->GetFireballs();
            for (int j=0;j<int(fbs.size());j++){
                auto f_box = fbs[j]->GetBox();
                if (f_box.ifCollide(c_box)){
                    mario->Hurt();
                }
            }
        }
    }


    if (has_flag && c_box.ifCollide(flag->GetBox()) && flag->GetBox().IsActive()){
        level_clear = true;
    }
}

void CollisionManager::OtherCollisionProcess(std::shared_ptr<Item> item){
    auto i_ani = item->GetAnimationObject();
    auto i_box = item->GetBox();
    glm::vec2 i_pos = i_box.GetPosition();
    
    for (int i=0;i<int(floor_boxes.size());i++){
        if (i_box.ifCollide(floor_boxes[i])){
            CollisionBox::State state = i_box.GetCurrentState(); 
            if (state == CollisionBox::State::BOTTOM){
                item->SetStandingOnBlock(floor_boxes[i]);
                item->SetOnGround(true);
                i_flag = true;
                break;
            }
            else if (state == CollisionBox::State::RIGHT || state == CollisionBox::State::LEFT){
                float new_x;
                if (state == CollisionBox::State::RIGHT){
                    new_x = (floor_boxes[i].GetPosition().x - floor_boxes[i].GetWidth()/2) - i_box.GetWidth()/2;
                }
                else{
                    new_x = (floor_boxes[i].GetPosition().x + floor_boxes[i].GetWidth()/2) + i_box.GetWidth()/2;
                }
                i_ani->SetPosition({new_x, i_pos.y});
                i_box.SetPosition({new_x, i_pos.y});
            }
        }
    }

    for (int i=0;i<int(pipes.size());i++){
        auto p_box = pipes[i]->GetBox();
        if (i_box.ifCollide(p_box)){
            CollisionBox::State i_state = i_box.GetCurrentState();
            if (i_state == CollisionBox::State::BOTTOM){
                item->SetStandingOnBlock(p_box);
                item->SetOnGround(true);
                i_flag = true;
            }
            else if (i_state == CollisionBox::State::RIGHT || i_state == CollisionBox::State::LEFT){
                float new_x;
                if (i_state == CollisionBox::State::RIGHT){
                    item->SetDirection(0);
                    new_x = (p_box.GetPosition().x - p_box.GetWidth()/2) - i_box.GetWidth()/2;
                }
                else{
                    item->SetDirection(1);
                    new_x = (p_box.GetPosition().x + p_box.GetWidth()/2) + i_box.GetWidth()/2;
                }
                i_ani->SetPosition({new_x, i_pos.y});
                i_box.SetPosition({new_x, i_pos.y});
            }
        }
    }
}

void CollisionManager::OtherCollisionProcess(std::shared_ptr<Fireball> fb, int type){
    auto f_ani = fb->GetAnimationObject();
    auto f_box = fb->GetBox();
    
    for (int i=0;i<int(floor_boxes.size());i++){
        if (f_box.ifCollide(floor_boxes[i])){
            CollisionBox::State state = f_box.GetCurrentState(); 
            if (state == CollisionBox::State::BOTTOM){
                glm::vec2 f_velo = fb->GetVelocity();
                f_velo.y = fb->GetJumppower();
                fb->SetVelocity(f_velo);
                break;
            }
            else if (state == CollisionBox::State::RIGHT || state == CollisionBox::State::LEFT){
                fb->SetExplode();
            }
        }
    }

    for (int i=0;i<int(pipes.size());i++){
        auto p_box = pipes[i]->GetBox();
        if (f_box.ifCollide(p_box)){
            CollisionBox::State f_state = f_box.GetCurrentState();
            if (f_state == CollisionBox::State::BOTTOM){
                glm::vec2 f_velo = fb->GetVelocity();
                f_velo.y = fb->GetJumppower();
                fb->SetVelocity(f_velo);
                break;
            }
            else if (f_state == CollisionBox::State::RIGHT || f_state == CollisionBox::State::LEFT){
                fb->SetExplode();
            }
        }
    }

    if (type == 0){
        for (int i=0;i<int(goombas.size());i++){
            auto g_box = goombas[i]->GetBox();
            if (g_box.ifCollide(f_box) && !goombas[i]->IsDead()){
                if (f_box.GetPosition().x < g_box.GetPosition().x || g_box.GetCurrentState() == CollisionBox::State::LEFT){
                    goombas[i]->SetVelocity(glm::vec2{100,200});
                }
                else{
                    goombas[i]->SetVelocity(glm::vec2{-100,200});
                }
                goombas[i]->GetAnimationObject()->SetZIndex(60);
                goombas[i]->SetDeath(2);
                auto se = goombas[i]->GetSE();
                se->LoadMedia(RESOURCE_DIR"/SoundEffects/Enemy/kickkill.wav");
                se->SetVolume(80);
                se->Play();

                fb->SetExplode();
                mario->AddScore(100);
            }
        }

        for (int i=0;i<int(koopas.size());i++){
            auto k_box = koopas[i]->GetBox();
            if (k_box.ifCollide(f_box) && !koopas[i]->IsDead()){
                if (f_box.GetPosition().x < k_box.GetPosition().x || k_box.GetCurrentState() == CollisionBox::State::LEFT){
                    koopas[i]->SetVelocity(glm::vec2{100,200});
                }
                else{
                    koopas[i]->SetVelocity(glm::vec2{-100,200});
                }
                koopas[i]->GetAnimationObject()->SetZIndex(60);
                koopas[i]->SetDeath(2);
                auto se = koopas[i]->GetSE();
                se->LoadMedia(RESOURCE_DIR"/SoundEffects/Enemy/kickkill.wav");
                se->SetVolume(80);
                se->Play();

                fb->SetExplode();
                mario->AddScore(200);
            }
        }

        for (int i=0;i<int(piranhas.size());i++){
            auto p_box = piranhas[i]->GetBox();
            if (p_box.ifCollide(f_box) && !piranhas[i]->IsDead()){
                piranhas[i]->SetDeath(2);
                auto se = piranhas[i]->GetSE();
                se->LoadMedia(RESOURCE_DIR"/SoundEffects/Enemy/kickkill.wav");
                se->SetVolume(80);
                se->Play();

                fb->SetExplode();
                mario->AddScore(200);
            }
        }

        for (int i=0;i<int(bowser.size());i++){
            auto b_box = bowser[i]->GetBox();
            if (b_box.ifCollide(f_box) && !bowser[i]->IsDead()){
                bowser[i]->Hurt();
                fb->SetExplode();
                if (bowser[i]->IsDead()){
                    if (f_box.GetPosition().x < b_box.GetPosition().x || b_box.GetCurrentState() == CollisionBox::State::LEFT){
                        bowser[i]->SetVelocity(glm::vec2{100,200});
                    }
                    else{
                        bowser[i]->SetVelocity(glm::vec2{-100,200});
                    }
                    auto se = bowser[i]->GetSE();
                    se->LoadMedia(RESOURCE_DIR"/SoundEffects/Enemy/kickkill.wav");
                    se->SetVolume(80);
                    se->Play();

                    mario->AddScore(5000);
                }
            }
        }
    }
    else{
        auto m_box = mario->GetBox();
        if (m_box.ifCollide(f_box)){
            mario->Hurt();
        }
    }
}

void CollisionManager::EnemyCollisionProcess(){
    for (int i=0;i<int(goombas.size());i++){
        if (!goombas[i]->IsDead()){
            auto g_box = goombas[i]->GetBox();
            if (g_box.ifCollide(mario->GetBox())){
                CollisionBox::State g_state = g_box.GetCurrentState();
                if (g_state == CollisionBox::State::TOP && mario->GetBox().GetPosition().y > g_box.GetPosition().y){
                    goombas[i]->SetDeath(1);
                    auto m_velo = mario->GetVelocity();
                    m_velo.y = 500;
                    mario->SetVelocity(m_velo);
                    mario->AddScore(100);

                    auto se = goombas[i]->GetSE();
                    se->LoadMedia(RESOURCE_DIR"/SoundEffects/Enemy/stomp.wav");
                    se->SetVolume(80);
                    se->Play();
                }
                else if (g_state == CollisionBox::State::LEFT || g_state == CollisionBox::State::RIGHT){
                    mario->Hurt();
                }
            }
        }
    }   

    for (int i=0;i<int(koopas.size());i++){
        if (koopas[i]->GetDeath() != 2){
            auto k_box = koopas[i]->GetBox();
            if (k_box.ifCollide(mario->GetBox())){
                CollisionBox::State k_state = k_box.GetCurrentState();
                if (k_state == CollisionBox::State::TOP && mario->GetBox().GetPosition().y > k_box.GetPosition().y){
                    koopas[i]->SetDeath(1);
                    if (mario->GetBox().GetPosition().x < koopas[i]->GetBox().GetPosition().x){
                        koopas[i]->SetFacingRight(true);
                    }
                    else{
                        koopas[i]->SetFacingRight(false);
                    }
                    auto m_velo = mario->GetVelocity();
                    m_velo.y = 500;
                    mario->SetVelocity(m_velo);
                    mario->AddScore(100);

                    auto se = koopas[i]->GetSE();
                    se->LoadMedia(RESOURCE_DIR"/SoundEffects/Enemy/stomp.wav");
                    se->SetVolume(80);
                    se->Play();
                }
                else{
                    mario->Hurt();
                }
            }
        }
    }

    for (int i=0;i<int(piranhas.size());i++){
        if (piranhas[i]->GetDeath() != 2){
            auto p_box = piranhas[i]->GetBox();
            if (p_box.ifCollide(mario->GetBox())){
                mario->Hurt();
            }
        }
    }
}

void CollisionManager::EECollisionProcess(std::shared_ptr<Goomba> goomba, int index){
    if (goomba->IsDead()){
        return;
    }
    for (int i=0;i<int(goombas.size());i++){
        if (i != index && !goombas[i]->IsDead()){
            auto e_box = goombas[i]->GetBox();
            if (e_box.ifCollide(goomba->GetBox())){
                CollisionBox::State e_state = e_box.GetCurrentState();
                if (e_state == CollisionBox::State::LEFT || e_state == CollisionBox::State::RIGHT){
                    if (goomba->IsFacingRight()){
                        goomba->SetFacingRight(false);
                        goombas[i]->SetFacingRight(true);
                    }
                    else{
                        goomba->SetFacingRight(true);
                        goombas[i]->SetFacingRight(false);
                    }
                }
            }
        }
    }

    for (int i=0;i<int(koopas.size());i++){
        if (!koopas[i]->IsKicked()){
            auto e_box = koopas[i]->GetBox();
            if (e_box.ifCollide(goomba->GetBox())){
                CollisionBox::State e_state = e_box.GetCurrentState();
                if (e_state == CollisionBox::State::LEFT || e_state == CollisionBox::State::RIGHT){
                    if (goomba->IsFacingRight()){
                        goomba->SetFacingRight(false);
                        koopas[i]->SetFacingRight(true);
                    }
                    else{
                        goomba->SetFacingRight(true);
                        koopas[i]->SetFacingRight(false);
                    }
                }
            }
        }
        else{
            auto e_box = koopas[i]->GetBox();
            if (e_box.ifCollide(goomba->GetBox())){
                auto se = koopas[i]->GetSE();
                se->LoadMedia(RESOURCE_DIR"/SoundEffects/Enemy/kickkill.wav");
                se->SetVolume(80);
                se->Play();
                goomba->SetDeath(2);
            }
        }
    }
}

void CollisionManager::EECollisionProcess(std::shared_ptr<Koopa> koopa, int index){
    if (koopa->IsDead()){
        return;
    }
    for (int i=0;i<int(koopas.size());i++){
        if (i != index && !koopas[i]->IsKicked() && !koopas[i]->IsDead()){
            auto e_box = koopas[i]->GetBox();
            if (e_box.ifCollide(koopa->GetBox())){
                CollisionBox::State e_state = e_box.GetCurrentState();
                if (e_state == CollisionBox::State::LEFT || e_state == CollisionBox::State::RIGHT){
                    if (koopa->IsFacingRight()){
                        koopa->SetFacingRight(false);
                        koopas[i]->SetFacingRight(true);
                    }
                    else{
                        koopa->SetFacingRight(true);
                        koopas[i]->SetFacingRight(false);
                    }
                }
            }
        }
        else{
            auto e_box = koopas[i]->GetBox();
            if (i != index && e_box.ifCollide(koopa->GetBox()) && koopas[i]->IsActive()){
                auto se = koopa->GetSE();
                se->LoadMedia(RESOURCE_DIR"/SoundEffects/Enemy/kickkill.wav");
                se->SetVolume(80);
                se->Play();
                koopa->SetDeath(2);
            }
        }
    }
}

void CollisionManager::ItemCollisionProcess(){
    auto m_box = mario->GetBox();
    Mario::Mode m_mode = mario->GetMarioMode();
    for (int i=0;i<int(items.size());i++){
        auto i_box = items[i]->GetBox();
        if (m_box.ifCollide(i_box)){
            int effect = items[i]->GetEffect();
            if (effect == 1){
                if (m_mode == Mario::Mode::SMALL){
                    mario->StateUpdate(Mario::Mode::BIG);
                    auto se = mario->Get_SE();
                    se->LoadMedia(RESOURCE_DIR"/SoundEffects/Mario/powerup.wav");
                    se->SetVolume(80);
                    se->Play();
                }
                mario->AddScore(1000);
            }
            else if (effect == 2){
                if (m_mode != Mario::Mode::FIRE){
                    mario->StateUpdate(Mario::Mode::FIRE);
                    auto se = mario->Get_SE();
                    se->LoadMedia(RESOURCE_DIR"/SoundEffects/Mario/powerup.wav");
                    se->SetVolume(80);
                    se->Play();
                }
                mario->AddScore(1000);
            }
            else if (effect == 3){ //<- This is for invincible star, but I'm still avoiding it lol

            }
            else if (effect == 4){
                mario->AddLive();
                mario->AddScore(1000);
            }
            else if (effect == 5){
                game_clear = true;
            }
            else if (effect == 6){
                mario->AddCoin();
                mario->AddScore(200);
            }
            items[i]->MarkDestroy();
        }
    }
}

void CollisionManager::RemoveMarkedObject(){
    blocks.erase(std::remove_if(blocks.begin(), blocks.end(),
        [](const std::shared_ptr<Block>& block) {
            return block->IsMarkedDestroy(); 
        }),
        blocks.end());

    items.erase(std::remove_if(items.begin(), items.end(),
        [](const std::shared_ptr<Item>& item) {
            return item->IsMarkedDestroy(); 
        }),
        items.end());

    goombas.erase(std::remove_if(goombas.begin(), goombas.end(),
        [](const std::shared_ptr<Goomba>& enemy) {
            return enemy->IsMarkedDestroy(); 
        }),
        goombas.end());

    koopas.erase(std::remove_if(koopas.begin(), koopas.end(),
        [](const std::shared_ptr<Koopa>& enemy) {
            return enemy->IsMarkedDestroy(); 
        }),
        koopas.end());
    
    piranhas.erase(std::remove_if(piranhas.begin(), piranhas.end(),
        [](const std::shared_ptr<Piranha>& enemy) {
            return enemy->IsMarkedDestroy(); 
        }),
        piranhas.end());

    bowser.erase(std::remove_if(bowser.begin(), bowser.end(),
        [](const std::shared_ptr<Bowser>& enemy) {
            return enemy->IsMarkedDestroy(); 
        }),
        bowser.end());

    int fireballs_size = m_fireballs.size();
    for (int i = 0; i < fireballs_size; i++) {
        auto fb = m_fireballs.front();
        m_fireballs.pop();
        if (!fb->IsMarkedRemove()){
            m_fireballs.push(fb);
        }
    }

    int b_fireballs_size = b_fireballs.size();
    for (int i = 0; i < b_fireballs_size; i++) {
        auto fb = b_fireballs.front();
        b_fireballs.pop();
        if (!fb->IsMarkedRemove()){
            b_fireballs.push(fb);
        }
    }
}

void CollisionManager::AddItem(std::shared_ptr<Item> item){
    items.push_back(item);
}

void CollisionManager::SetMFireballs(std::queue<std::shared_ptr<Fireball>> fireballs){
    m_fireballs = fireballs;
}

void CollisionManager::SetBFireballs(std::queue<std::shared_ptr<Fireball>> fireballs){
    b_fireballs = fireballs;
}

void CollisionManager::SetGoombas(std::vector<std::shared_ptr<Goomba>> goombas){
    this->goombas = goombas;
}

void CollisionManager::SetPiranhas(std::vector<std::shared_ptr<Piranha>> piranhas){
    this->piranhas = piranhas;
}

void CollisionManager::SetKoopas(std::vector<std::shared_ptr<Koopa>> koopas){
    this->koopas = koopas;
}

void CollisionManager::SetBowser(std::vector<std::shared_ptr<Bowser>> bowser){
    this->bowser = bowser;
}

void CollisionManager::SetFlag(std::shared_ptr<Flag> flag){
    this->flag = flag;
    has_flag = true;
}

void CollisionManager::SetItem(std::vector<std::shared_ptr<Item>> items){
    this->items = items;
}

void CollisionManager::SetFirebar(std::vector<std::shared_ptr<FireBar>> firebars){
    for (int i=0;i<int(firebars.size());i++){
        blocks.push_back(firebars[i]);
    }
    this->firebars = firebars;
}

bool CollisionManager::IsLevelCleared(){
    return level_clear;
}

bool CollisionManager::EnterSecretLevel(){
    return enter_secret_level;
}

bool CollisionManager::IsGameCleared(){
    return game_clear;
}

bool CollisionManager::GetCFlag(){
    return c_flag;
}

void CollisionManager::SetCFlag(bool choice){
    c_flag = choice;
}

void CollisionManager::ResetStates(){
    enter_secret_level = false;
    level_clear = false;
    game_clear = false;
}

bool CollisionManager::EnemyInCameraRange(std::shared_ptr<Character> enemy, glm::vec2 mario_pos){
    bool Xoverlap = false;
    glm::vec2 pos = enemy->GetAnimationObject()->GetPosition();
    Xoverlap = mario_pos.x - 400 >= pos.x || //-4000 , -1200
            mario_pos.x + 400 >= pos.x;
    return Xoverlap;
}

void CollisionManager::UpdateLBarrier(std::string level){
    if (level == "1_1A" || level == "1_2B"){
        return;
    }
    if (map_barriers[0] + 372 > mario->GetBox().GetPosition().x){
        return;
    }
    if (level == "1_2" && mario->GetBox().GetPosition().x >= 3840){
        return;
    }
    if (level == "1_4" && mario->GetBox().GetPosition().x >= 2256){
        map_barriers[0] = 2256;
        return;
    }

    map_barriers[0] = mario->GetBox().GetPosition().x - 372;
}

void CollisionManager::SetRBarrier(float new_pos){
    map_barriers[1] = new_pos;
}

std::string CollisionManager::GetPipeEnterLevel(){
    return pipeEnterLevel;
}