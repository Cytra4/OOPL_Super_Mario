#include "Blocks/FireBar.hpp"

FireBar::FireBar(std::string defaultPath, glm::vec2 pos, float width, float height, bool c_wise, float angle)
: Block(defaultPath, pos, width, height){
    clock_wise = c_wise;
    this->angle = angle;
    FireballsInitial();
    SetJump(true);
}

void FireBar::PhysicProcess(double time){
    angle += rotation_speed * time;

    float spacing = 24.0f; 
    
    glm::mat2 rotationMatrix = glm::mat2(
        cos(angle), -sin(angle),
        sin(angle),  cos(angle)
    );

    for (int i = 0; i < int(fireballs.size()); ++i) {
        glm::vec2 localPos = glm::vec2(i * spacing, 0.0f);

        glm::vec2 rotatedPos = rotationMatrix * localPos;
        rotatedPos += GetAnimationObject()->GetPosition();
        fireballs[i]->GetAnimationObject()->SetPosition(rotatedPos);
        fireballs[i]->GetBox().SetPosition(rotatedPos);
    }
}

void FireBar::FireballsInitial(){
    std::vector<std::vector<std::string>> paths;
    std::vector<std::string> roll;
    roll.reserve(4);
    for (int i=0;i<4;i++){
        roll.emplace_back(RESOURCE_DIR"/Sprites/Mario/Fireball/fireball" + std::to_string(i+1) + ".png");
    }
    std::vector<std::string> explode;
    explode.reserve(3);
    for (int i=0;i<3;i++){
        explode.emplace_back(RESOURCE_DIR"/Sprites/Mario/Fireball/fireball_explode" + std::to_string(i+1) + ".png");
    }

    glm::vec2 position = GetAnimationObject()->GetPosition();
    for (int i=0;i<6;i++){
        std::shared_ptr<Fireball> fb = std::make_shared<Fireball>(FireballType::BLOCK,RESOURCE_DIR"/Sprites/Mario/Fireball/fireball1.png",0,position,24.0f,24.0f);
        auto ani_obj = fb->GetAnimationObject();
        ani_obj->AddNewAnimation(roll);
        ani_obj->AddNewAnimation(explode);
        ani_obj->SetZIndex(60);

        ani_obj->SetAnimation(0, 25);
        ani_obj->SetLooping(true);
        ani_obj->PlayAnimation();
        ani_obj->SetCurrentAnimation(0);

        fireballs.push_back(fb);
        position.x += 24;
    }
}

void FireBar::DrawFireballs(Util::Renderer& renderer){
    for (int i=0;i<int(fireballs.size());i++){
        renderer.AddChild(fireballs[i]->GetAnimationObject());
    }
}

void FireBar::RemoveFireballs(Util::Renderer& renderer){
    for (int i=0;i<int(fireballs.size());i++){
        renderer.RemoveChild(fireballs[i]->GetAnimationObject());
    }
    fireballs.clear();
}

std::vector<std::shared_ptr<Fireball>> FireBar::GetFireballs(){
    return fireballs;
}