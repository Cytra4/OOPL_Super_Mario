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
    BlockCollisionProcess(mario, time);
    OtherCollisionProcess(mario);
    if (!c_flag){mario->SetOnGround(false);}
    EnemyCollisionProcess();
    ItemCollisionProcess();

    //Here should be 2 loops
    //1.Loops of enemies doing BlockCollisionProcess() and OtherCollisionProcess()
    //2.Loops of items doing BlockCOllisionProcess() and OtherCollisionProcess()
    //*TO BE DONE

    RemoveMarkedObject();
    RemoveDeadEnemy();
}

void CollisionManager::BlockCollisionProcess(std::shared_ptr<Character> character, double time){
    auto c_box = character->GetBox();
    auto c_ani = character->GetAnimationObject();
    glm::vec2 c_pos = c_ani->GetPosition();

    for (int i=0;i<int(blocks.size());i++){
        auto b_box = blocks[i]->GetBox();
        if (b_box.IsActive() && c_box.ifCollide(b_box)){
            CollisionBox::State c_state = c_box.GetCurrentState();
            b_box.ifCollide(c_box);
            if (c_state == CollisionBox::State::TOP && mario->GetVelocity().y > 0){
                blocks[i]->ContactBehavior(mario->GetMarioMode() != Mario::Mode::SMALL);
                float new_y = (b_box.GetPosition().y - b_box.GetHeight()/2) - c_box.GetHeight()/2;
                auto c_velo = character->GetVelocity();
                c_ani->SetPosition({c_pos.x, new_y});
                c_box.SetPosition({c_pos.x, new_y});
                character->SetVelocity({c_velo.x, 0});
                
            }
            else if (c_state == CollisionBox::State::BOTTOM){
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

void CollisionManager::EnemyCollisionProcess(){

}

void CollisionManager::ItemCollisionProcess(){

}

void CollisionManager::RemoveMarkedObject(){
    blocks.erase(std::remove_if(blocks.begin(), blocks.end(),
        [](const std::shared_ptr<Block>& block) {
            return block->IsMarkedDestroy(); 
        }),
        blocks.end());
}

void CollisionManager::RemoveDeadEnemy(){

}