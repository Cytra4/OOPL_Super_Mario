#include "GameUtils/CollisionManager.hpp"
#include "Util/Logger.hpp"

CollisionManager::CollisionManager(std::shared_ptr<Mario> mario, std::vector<std::shared_ptr<Block>> blocks,
std::vector<std::shared_ptr<Pipe>> pipes, std::vector<CollisionBox> floor_boxes, glm::vec2 mapSize){
    this->mario = mario;
    this->blocks = blocks;
    this->pipes = pipes;
    this->floor_boxes = floor_boxes;
    map_barrier = mapSize.x/2;
}

void CollisionManager::UpdateProcess(double time){
    c_flag = false;
    BlockCollisionProcess(mario, time, 0);
    OtherCollisionProcess(mario);
    if (!c_flag){mario->SetOnGround(false);}
    EnemyCollisionProcess();
    ItemCollisionProcess();

    //Here should be 2 loops
    //1.Loops of enemies doing BlockCollisionProcess() and OtherCollisionProcess()
    //*TO BE DONE
    for (int i=0;i<int(items.size());i++){
        items[i]->PhysicProcess(time);
        if (items[i]->IsReachedTop()){
            i_flag = false;
            BlockCollisionProcess(items[i]);
            OtherCollisionProcess(items[i]);
            if (!i_flag){items[i]->SetOnGround(false);}
        }
    }

    RemoveMarkedObject();
    RemoveDeadEnemy();
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
                    blocks[i]->ContactBehavior(mario->GetMarioMode() != Mario::Mode::SMALL);
                    float new_y = (b_box.GetPosition().y - b_box.GetHeight()/2) - c_box.GetHeight()/2;
                    auto c_velo = character->GetVelocity();
                    c_ani->SetPosition({c_pos.x, new_y});
                    c_box.SetPosition({c_pos.x, new_y});
                    character->SetVelocity({c_velo.x, 0});
                    
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
                    new_x = (b_box.GetPosition().x - b_box.GetWidth()/2) - c_box.GetWidth()/2;
                }
                else{
                    new_x = (b_box.GetPosition().x + b_box.GetWidth()/2) + c_box.GetWidth()/2;
                }
                c_ani->SetPosition({new_x, c_pos.y});
                c_box.SetPosition({new_x, c_pos.y});
            }
        }
        if (blocks[i]->IsJumping()){blocks[i]->PhysicProcess(time);}
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

void CollisionManager::OtherCollisionProcess(std::shared_ptr<Character> character){
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
        c_ani->SetPosition({map_barrier*-1+20,c_pos.y});
    }
    else if (c_pos.x > map_barrier-20){
        c_ani->SetPosition({map_barrier-20,c_pos.y});
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
                    new_x = (p_box.GetPosition().x - p_box.GetWidth()/2) - c_box.GetWidth()/2;
                }
                else{
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

void CollisionManager::EnemyCollisionProcess(){

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
}

void CollisionManager::RemoveDeadEnemy(){

}

void CollisionManager::AddItem(std::shared_ptr<Item> item){
    items.push_back(item);
}