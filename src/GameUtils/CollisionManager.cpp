#include "GameUtils/CollisionManager.hpp"
#include "Util/Logger.hpp"

CollisionManager::CollisionManager(std::shared_ptr<Mario> mario, 
std::vector<std::shared_ptr<Block>> blocks, std::vector<std::shared_ptr<Pipe>> pipes, 
std::vector<CollisionBox> floor_boxes, glm::vec2 mapSize){
    this->mario = mario;
    this->blocks = blocks;
    this->pipes = pipes;
    this->floor_boxes = floor_boxes;
    map_barrier = mapSize.x/2;
}

void CollisionManager::UpdateProcess(double time, glm::vec2 CameraPosition){
    c_flag = false;
    BlockCollisionProcess(mario, time, 0);
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
                BlockCollisionProcess(goombas[i], time, 1);
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
                BlockCollisionProcess(koopas[i], time, 1);
                OtherCollisionProcess(koopas[i], 1);
                EECollisionProcess(koopas[i], i);
            }
            if (!c_flag){koopas[i]->SetOnGround(false);}
            koopas[i]->PhysicProcess(time);
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
        OtherCollisionProcess(fb);
        BlockCollisionProcess(fb);
        if (fb->OutOfRange(CameraPosition)) {
            fb->MarkRemove();
        }
        m_fireballs.push(fb);
    }

    RemoveMarkedObject();
}

void CollisionManager::BlockCollisionProcess(std::shared_ptr<Character> character, double time, int mode){
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
                    for (int j = 0;j<int(goombas.size());j++){
                        if (b_box.ifCollide(goombas[j]->GetBox())){
                            goombas[j]->SetDeath(2);
                            if (goombas[j]->GetBox().GetPosition().x > b_box.GetPosition().x){
                                goombas[j]->SetVelocity(glm::vec2{100,200});
                            }
                            else{
                                goombas[j]->SetVelocity(glm::vec2{-100,200});
                            }
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
        if (blocks[i]->IsJumping()){
            blocks[i]->PhysicProcess(time);
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
        }
    }

    if (c_pos.x < map_barrier*-1+20){
        if (mode == 0){
            c_ani->SetPosition({map_barrier*-1+20,c_pos.y});
        }
        else{
            character->MarkDestroy();
        }
    }
    else if (c_pos.x > map_barrier-20){
        if (mode == 0){
            c_ani->SetPosition({map_barrier-20,c_pos.y});
        }
        else{
            character->MarkDestroy();
        }
    }

    for (int i=0;i<int(pipes.size());i++){
        auto p_box = pipes[i]->GetBox();
        if (c_box.ifCollide(p_box)){
            CollisionBox::State c_state = c_box.GetCurrentState();
            if (c_state == CollisionBox::State::BOTTOM){
                character->SetStandingOnBlock(p_box);
                character->SetOnGround(true);
                c_flag = true;
            }
            else if (c_state == CollisionBox::State::RIGHT || c_state == CollisionBox::State::LEFT){
                float new_x;

                if (c_state == CollisionBox::State::RIGHT){
                    if (mode == 1){
                        character->SetFacingRight(false);
                    }
                    new_x = (p_box.GetPosition().x - p_box.GetWidth()/2) - c_box.GetWidth()/2;
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

    //*MISSING THE HANDLE OF HIDDEN PIPE
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

void CollisionManager::OtherCollisionProcess(std::shared_ptr<Fireball> fb){
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

    //Handling fireball and enemies (I'm lazy to write another function for this)
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
            fb->SetExplode();
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
            fb->SetExplode();
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
                }
                else{
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
                }
                else{
                    mario->Hurt();
                }
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
        else{
            auto e_box = goombas[i]->GetBox();
            if (e_box.ifCollide(goomba->GetBox()) && goombas[i]->IsActive()){
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
        if (i != index && !koopas[i]->IsKicked()){
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
                }
                else{
                    //So this part will be handle the score
                    //*TO BE DONE
                }
            }
            else if (effect == 2){
                if (m_mode != Mario::Mode::FIRE){
                    mario->StateUpdate(Mario::Mode::FIRE);
                }
                else{
                    
                }
            }
            else if (effect == 3){ //<- This is for invincible star, but I'm still avoiding it

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

    int fireballs_size = m_fireballs.size();
    for (int i = 0; i < fireballs_size; i++) {
        auto fb = m_fireballs.front();
        m_fireballs.pop();
        if (!fb->IsMarkedRemove()){
            m_fireballs.push(fb);
        }
    }
}

void CollisionManager::AddItem(std::shared_ptr<Item> item){
    items.push_back(item);
}

void CollisionManager::SetFireballs(std::queue<std::shared_ptr<Fireball>> fireballs){
    m_fireballs = fireballs;
}

void CollisionManager::SetGoombas(std::vector<std::shared_ptr<Goomba>> goombas){
    this->goombas = goombas;
}

void CollisionManager::SetKoopas(std::vector<std::shared_ptr<Koopa>> koopas){
    this->koopas = koopas;
}

bool CollisionManager::EnemyInCameraRange(std::shared_ptr<Character> enemy, glm::vec2 mario_pos){
    bool Xoverlap = false;
    glm::vec2 pos = enemy->GetAnimationObject()->GetPosition();
    Xoverlap = mario_pos.x - 400 >= pos.x || //-4000 , -1200
            mario_pos.x + 400 >= pos.x;
    return Xoverlap;
}