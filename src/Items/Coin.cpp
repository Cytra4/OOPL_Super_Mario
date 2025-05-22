#include "Items/Coin.hpp"

Coin::Coin(int type, std::string defaultPath, glm::vec2 pos, float width, float height)
:Item(-1, -1, defaultPath, pos, width, height){
    this->type = type;
    final_pos = pos + glm::vec2{0.0f,120.0f};
    init_pos = pos;
    std::vector<std::string> coin_anim;
    for (int i=1;i<5;i++){
        coin_anim.push_back(RESOURCE_DIR"/Sprites/Items/coin" + std::to_string(i) + ".png");
    }
    auto ani_obj = GetAnimationObject();
    ani_obj->AddNewAnimation(coin_anim);
    ani_obj->SetAnimation(0, 50);
    ani_obj->SetLooping(true);
    ani_obj->PlayAnimation();
    ani_obj->SetCurrentAnimation(0);
}

void Coin::PhysicProcess(double time){
    glm::vec2 coin_pos = GetAnimationObject()->GetPosition();
    double deltaTime = time;

    glm::vec2 new_pos;

    if (reachedTop){
        new_pos.x = coin_pos.x;
        new_pos.y = coin_pos.y - deltaTime*upSpeed*2;
        if (new_pos.y <= init_pos.y){
            new_pos.y = init_pos.y;
            MarkDestroy();
        }
    }
    else{
        new_pos.x = coin_pos.x;
        new_pos.y = coin_pos.y + deltaTime*upSpeed*2;
        if (new_pos.y >= final_pos.y){
            new_pos.y = final_pos.y;
            reachedTop = true;
        }
    }

    GetAnimationObject()->SetPosition(new_pos);
    GetBox().SetPosition(new_pos);
}