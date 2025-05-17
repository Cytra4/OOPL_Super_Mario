#include "Items/Axe.hpp"

Axe::Axe(std::string defaultPath, glm::vec2 position, float width, float height)
: Item(-1, 5, defaultPath, position, width, height){
    std::vector<std::string> axe_anim;
    for (int i=1;i<4;i++){
        axe_anim.push_back(RESOURCE_DIR"/Sprites/Blocks/axe" + std::to_string(i) + ".png");
    }
    auto ani_obj = GetAnimationObject();
    ani_obj->AddNewAnimation(axe_anim);
    ani_obj->SetAnimation(0,150);
    ani_obj->SetLooping(true);
    ani_obj->PlayAnimation();
    ani_obj->SetCurrentAnimation(0);
}