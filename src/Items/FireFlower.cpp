#include "Items/FireFlower.hpp"

FireFlower::FireFlower(int effect, std::string defaultPath, glm::vec2 pos, float width, float height)
:Item(-1, effect, defaultPath, pos, width, height){
    final_pos = pos + glm::vec2{0.0f,33.0f};
    std::vector<std::string> flower_anim;
    for (int i=1;i<5;i++){
        flower_anim.push_back(RESOURCE_DIR"/Sprites/Items/fireflower" + std::to_string(i) + ".png");
    }
    auto ani_obj = GetAnimationObject();
    ani_obj->AddNewAnimation(flower_anim);
    ani_obj->SetAnimation(0, 25);
    ani_obj->SetLooping(true);
    ani_obj->PlayAnimation();
    ani_obj->SetCurrentAnimation(0);
}

void FireFlower::PhysicProcess(double time){
    glm::vec2 flo_pos = GetAnimationObject()->GetPosition();
    double deltaTime = time;

    glm::vec2 new_pos;

    if (!reachedTop){
        new_pos.x = flo_pos.x;
        new_pos.y = flo_pos.y + deltaTime*upSpeed;
        if (new_pos.y >= final_pos.y){
            new_pos.y = final_pos.y;
            reachedTop = true;
        }
        GetAnimationObject()->SetPosition(new_pos);
        GetBox().SetPosition(new_pos);
    }
} 