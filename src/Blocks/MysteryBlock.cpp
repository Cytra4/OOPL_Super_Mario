#include "Blocks/MysteryBlock.hpp"
#include "Util/Logger.hpp"

MysteryBlock::MysteryBlock(std::string itemType, std::string defaultPath, glm::vec2 pos, float width, float height, int count) : 
Block(defaultPath, pos, width, height){
    init_pos = pos;
    top_pos = pos;
    top_pos.y += 20;
    itemCount = count;
    this->itemType = itemType;

    std::vector<std::string> mys_ani;
    int choice;
    if (defaultPath == RESOURCE_DIR"/Sprites/Blocks/empty1.png"){choice = 1;}
    else if (defaultPath == RESOURCE_DIR"/Sprites/Blocks/empty2.png"){choice = 2;}
    else{choice = 4;}

    for (int i=1;i<4;i++){
        mys_ani.push_back(RESOURCE_DIR"/Sprites/Blocks/mystery" + std::to_string(choice) + "_" + std::to_string(i) + ".png");
    }
    GetAnimationObject()->AddNewAnimation(mys_ani);
    GetAnimationObject()->SetAnimation(0,200);
    GetAnimationObject()->SetLooping(true);
    GetAnimationObject()->PlayAnimation();
    GetAnimationObject()->SetCurrentAnimation(0);
}

void MysteryBlock::SpawnItem(std::shared_ptr<Mario> mario){
    glm::vec2 spawn_pos = GetAnimationObject()->GetPosition();
    spawn_pos.y += 15.0f;
    hasItem = true;

    auto m_mode = mario->GetMarioMode();
    if (itemType == "Coin"){
        item = std::make_shared<Coin>(0, RESOURCE_DIR"/Sprites/Items/coin1.png", spawn_pos, 24.0f, 48.0f);
        item->GetAnimationObject()->SetZIndex(GetAnimationObject()->GetZIndex() - 1);
        mario->AddCoin();
        mario->AddScore(200);
    }
    else if (itemType == "Mushroom2"){
        int direction = int(rand() % 2);
        item = std::make_shared<Mushroom>(direction, 4, RESOURCE_DIR"/Sprites/Items/mushroom2.png"
        , spawn_pos, 48.0f, 48.0f);
        item->GetAnimationObject()->SetZIndex(GetAnimationObject()->GetZIndex() - 1);
        item->SetStandingOnBlock(GetBox());
        item->SetOnGround(true);
    }
    else if (m_mode == Mario::Mode::SMALL){
        int direction = int(rand() % 2);
        item = std::make_shared<Mushroom>(direction, 1, RESOURCE_DIR"/Sprites/Items/mushroom.png"
        , spawn_pos, 48.0f, 48.0f);
        item->GetAnimationObject()->SetZIndex(GetAnimationObject()->GetZIndex() - 1);
        item->SetStandingOnBlock(GetBox());
        item->SetOnGround(true);
    }
    else if (m_mode == Mario::Mode::BIG || m_mode == Mario::Mode::FIRE){
        item = std::make_shared<FireFlower>(2, RESOURCE_DIR"/Sprites/Items/fireflower1.png"
            , spawn_pos, 48.0f, 48.0f);
        item->GetAnimationObject()->SetZIndex(GetAnimationObject()->GetZIndex() - 1);
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

void MysteryBlock::ContactBehavior(int choice, std::shared_ptr<Mario> mario){
    if (choice == -1){
        return;
    }
    
    if (itemCount > 0){
        SpawnItem(mario);
        itemCount -= 1;
    }
    if (itemCount == 0){
        GetAnimationObject()->SetDefaultSprite(GetAnimationObject()->GetDefaultSprite());
        GetAnimationObject()->SetCurrentAnimation(-1);
        if (!IsJumping() && empty == false){
            SetJump(true);
        }
        empty = true;
    }
}