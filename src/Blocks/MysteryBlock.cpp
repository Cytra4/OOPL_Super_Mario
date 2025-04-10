#include "Blocks/MysteryBlock.hpp"
#include "Util/Logger.hpp"

MysteryBlock::MysteryBlock(std::string defaultPath, glm::vec2 pos, float width, float height, int count) : 
Block(defaultPath, pos, width, height){
    init_pos = pos;
    top_pos = pos;
    top_pos.y += 20;
    itemCount = count;

    std::vector<std::string> mys_ani;
    int choice;
    if (defaultPath == RESOURCE_DIR"/Sprites/Blocks/empty1.png"){choice = 1;}
    else if (defaultPath == RESOURCE_DIR"/Sprites/Blocks/empty2.png"){choice = 2;}
    else{choice = 3;}

    for (int i=1;i<4;i++){
        mys_ani.push_back(RESOURCE_DIR"/Sprites/Blocks/mystery" + std::to_string(choice) + "_" + std::to_string(i) + ".png");
    }
    GetAnimationObject()->AddNewAnimation(mys_ani);
    GetAnimationObject()->SetAnimation(0,200);
    GetAnimationObject()->SetLooping(true);
    GetAnimationObject()->PlayAnimation();
    GetAnimationObject()->SetCurrentAnimation(0);
}

void MysteryBlock::SpawnItem(){
    glm::vec2 spawn_pos = GetAnimationObject()->GetPosition();
    spawn_pos.y += 15.0f;
    hasItem = true;
    if (itemType == "Mushroom"){
        int direction = int(rand() % 2);
        item = std::make_shared<Mushroom>(direction, 1, RESOURCE_DIR"/Sprites/Items/mushroom.png"
        , spawn_pos, 48.0f, 48.0f);
        item->GetAnimationObject()->SetZIndex(GetAnimationObject()->GetZIndex() - 1);
        item->SetStandingOnBlock(GetBox());
        item->SetOnGround(true);
    }
    else if (itemType == "Flower"){

    }
    else if (itemType == "Coin"){

    }
}

void MysteryBlock::PhysicProcess(double time){
    glm::vec2 brick_pos = GetAnimationObject()->GetPosition();
    glm::vec2 brick_velo = velocity;
    double deltaTime = time;

    glm::vec2 new_pos;
    new_pos.x = brick_pos.x;

    if (ReachedTop()){
        new_pos.y = brick_pos.y - deltaTime*brick_velo.y;
    }
    else{
        new_pos.y = brick_pos.y + deltaTime*brick_velo.y;
    }

    if (new_pos.y >= top_pos.y){
        SetReachedTop(true);
        new_pos.y = top_pos.y;
    }
    else if (new_pos.y <= init_pos.y){
        SetReachedTop(false);
        SetJump(false);
        new_pos.y = init_pos.y;
    }
    GetAnimationObject()->SetPosition(new_pos);
    GetBox().SetPosition(new_pos);
}

void MysteryBlock::ContactBehavior(int choice){
    choice=choice;
    if (itemCount > 0){
        SpawnItem();
        itemCount -= 1;
    }
    if (itemCount == 0){
        GetAnimationObject()->SetDefaultSprite(GetAnimationObject()->GetDefaultSprite());
        GetAnimationObject()->SetCurrentAnimation(-1);
        if (!IsJumping()){
            SetJump(true);
        }
    }
}