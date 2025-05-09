#include "Items/FCoin.hpp"

FCoin::FCoin(std::string choice, std::string defaultPath, glm::vec2 pos, float width, float height)
:Item(-1, 4, defaultPath, pos, width, height){
    std::vector<std::string> coin_anim;
    for (int i=1;i<4;i++){
        coin_anim.push_back(RESOURCE_DIR"/Sprites/Blocks/coin" + choice + "_" + std::to_string(i) + ".png");
    }
    auto ani_obj = GetAnimationObject();
    ani_obj->AddNewAnimation(coin_anim);
    ani_obj->SetAnimation(0, 200);
    ani_obj->SetLooping(true);
    ani_obj->PlayAnimation();
    ani_obj->SetCurrentAnimation(0);
}