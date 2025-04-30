#include "Characters/Mario.hpp"
#include "Util/Logger.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

Mario::Mario(int jump, int movespeed, glm::vec2 position, float width, float height) 
: Character(RESOURCE_DIR"/Sprites/Mario/Small/mario_default.png", movespeed, position, width, height){
    jumpPower = jump;
    mario_mode = Mode::SMALL;
}

Mario::Mode Mario::GetMarioMode(){
    return mario_mode;
}

void Mario::SetMarioMode(Mario::Mode mode){
    mario_mode = mode;
}

bool Mario::IsRunning(){
    return isRunning;
}

void Mario::Behavior(){
    if (!IsDead()){
        if (Util::Input::IsKeyPressed(Util::Keycode::W) && IsOnGround()){
            velocity.y = jumpPower;
            isJumping = true;
            isJumpKeyHeld = true;
            SetOnGround(false);
        }
    
        if (Util::Input::IsKeyPressed(Util::Keycode::A)){
            velocity.x = -1*movespeed;
            isRunning = true;
            SetFacingRight(false);
        }
        else if (Util::Input::IsKeyPressed(Util::Keycode::D)){
            velocity.x = movespeed;
            isRunning = true;
            SetFacingRight(true);
        }
        else{
            isRunning = false;
            velocity.x = 0;
        }

        if (Util::Input::IsKeyPressed(Util::Keycode::J)){
            ShootFireball();
        }
    }

    AnimationHandle();
}

void Mario::PhysicProcess(double time){
    glm::vec2 mario_pos = ani_obj->GetPosition();
    glm::vec2 mario_velo = GetVelocity();
    CollisionBox b = GetStandingOnBlock();
    double deltaTime = time;

    glm::vec2 new_pos;
    new_pos.x = mario_pos.x + deltaTime*mario_velo.x;
    new_pos.y = mario_pos.y + deltaTime*mario_velo.y;

    if (mario_mode == Mode::FIRE && canShootFireballs == false){
        fireball_CD_timer += time;
        if (fireball_CD_timer > 0.5f){
            canShootFireballs = true;
            fireball_CD_timer = 0;
        }
    }

    //Detect if jump key is released while jumping
    if (Util::Input::IsKeyUp(Util::Keycode::W) && isJumping){
        isJumpKeyHeld = false;
    }

    //If jump key is released and mario is still jumping up,
    //makes his fall gravity very high to make big and small
    if (!isJumpKeyHeld && mario_velo.y > 0) {
        jumpFallGravity = 10.0f;
    }
    //after jump to the highest point, mario falls down 3x gravity
    else if (mario_velo.y <= 0 && !IsOnGround()){
        jumpFallGravity = 2.5;
    }

    if (!canTakeDamage){
        iFrames -= deltaTime;
        if (iFrames <= 0){
            canTakeDamage = true;
            iFrames = 0.5f;
        }
    }

    //Fixing mario's position when he's standing on block 
    if (IsOnGround() && !IsDead()){
        mario_velo.y = 0;
        jumpFallGravity = 3;
        isJumping = false;

        float floorY = b.GetHeight();
        new_pos.y = GetBox().GetHeight()/2 + b.GetPosition().y + floorY/2; 
    }

    //Gravity affecting y velocity
    mario_velo.y = mario_velo.y + deltaTime*gravity*jumpFallGravity;

    ani_obj->SetPosition(new_pos);
    GetBox().SetPosition(new_pos);
    SetVelocity(mario_velo);

}

void Mario::AnimationHandle(){
    int new_animation = -1;
    if (IsDead()){
        new_animation = 2;
    }
    else if (isJumping){
        new_animation = 1;
    }
    else if (IsRunning()){
        new_animation = 0;
    }
    else{
        ani_obj->SetDefaultSprite(ani_obj->GetDefaultSprite());
        ani_obj->SetCurrentAnimation(-1);
    }

    int cur = ani_obj->GetCurrentAnimation();
    if (new_animation != -1 && new_animation != cur){
        ani_obj->SetAnimation(new_animation, 25);
        ani_obj->SetLooping(true);
        ani_obj->PlayAnimation();
        ani_obj->SetCurrentAnimation(new_animation);
    }

    glm::vec2 scale = (IsFacingRight()) ? glm::vec2(1,1) : glm::vec2(-1,1);
    ani_obj->SetScale(scale);
}

void Mario::Hurt(){
    if (canTakeDamage){
        int health = GetHealth();
        if (health == 3){
            StateUpdate(Mode::BIG);
        }
        else if (health == 2){
            StateUpdate(Mode::SMALL);
        }
        else if (health == 1){
            SetDead(true);
            SetVelocity(glm::vec2{0.0f,1000.0f});
        }
        canTakeDamage = false;
    }
}

void Mario::StateUpdate(Mode new_mode){
    std::string new_state;
    glm::vec2 new_pos = GetAnimationObject()->GetPosition();
    switch(new_mode){
        case Mode::SMALL:
            new_state = "Small";
            canShootFireballs = false;
            SetHealth(1);
            new_pos.y -= 24.0f;
            break;
        case Mode::BIG:
            new_state = "Big";
            canShootFireballs = false;
            SetHealth(2);
            break;
        case Mode::FIRE:
            new_state = "Fire";
            canShootFireballs = true;
            SetHealth(3);
            break;
        default:
            LOG_ERROR("Unexpected Mario Mode?");
            return;
    }
    AnimationUpdate(new_state);
    mario_mode = new_mode;
}

void Mario::AnimationUpdate(std::string new_mode){
    std::string old_state;
    glm::vec2 new_pos = GetAnimationObject()->GetPosition();
    Mode new_state;
    if (new_mode == "Small"){
        new_state = Mode::SMALL;
    }
    else if (new_mode == "Big"){
        new_state = Mode::BIG;
    }
    else if (new_mode == "Fire"){
        new_state = Mode::FIRE;
    }

    if (mario_mode == Mode::SMALL){
        old_state = "Small";
    }
    else if (mario_mode == Mode::BIG){
        old_state = "Big";
    }
    else if (mario_mode == Mode::FIRE){
        old_state = "Fire";
    }

    switch(new_state){
        case Mode::SMALL:
            GetBox().SetWidth(36.0f);
            GetBox().SetHeight(48.0f);
            break;
        case Mode::BIG:
            if (mario_mode == Mario::Mode::SMALL){
                new_pos.y += 24.0f;
            }
            GetBox().SetWidth(48.0f);
            GetBox().SetHeight(96.0f);
            break;
        case Mode::FIRE:
            if (mario_mode == Mario::Mode::SMALL){
                new_pos.y += 24.0f;
            }
            GetBox().SetWidth(48.0f);
            GetBox().SetHeight(96.0f);
            break;
        default:
            LOG_ERROR("Unexpected Mario Mode?");
            return;
    }
    GetAnimationObject()->SetPosition(new_pos);
    GetBox().SetPosition(new_pos);

    std::string dftSprite = ani_obj->GetDefaultSprite();
    std::vector<std::vector<std::string>>& animations = ani_obj->GetAnimationPaths();

    size_t pos = dftSprite.find(old_state);
    if (pos != std::string::npos){
        dftSprite.replace(pos, old_state.length(), new_mode);
        ani_obj->SetDefaultSprite(dftSprite);
    }

    for (int i = 0; i<int(animations.size()); i++){
        for (int j = 0; j<int(animations[i].size()); j++){
            size_t pos = animations[i][j].find(old_state);
            if (pos != std::string::npos){
                animations[i][j].replace(pos, old_state.length(), new_mode);
            }
        }
    }

    if (ani_obj->GetCurrentAnimation() != -1){
        ani_obj->SetAnimation(ani_obj->GetCurrentAnimation(), 25);
        ani_obj->SetLooping(true);
        ani_obj->PlayAnimation();
    }
}

void Mario::ShootFireball(){
    if (canShootFireballs){
        canShootFireballs = false;
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
        
        glm::vec2 f_pos = ani_obj->GetPosition();
        if (IsFacingRight()){
            f_pos += glm::vec2{30,0};
        }
        else{
            f_pos -= glm::vec2{30,0};
        }

        std::shared_ptr<Fireball> fireball = std::make_shared<Fireball>(FireballType::MARIO, 
        RESOURCE_DIR"/Sprites/Mario/Fireball/fireball1.png",500 * ani_obj->GetTransform().scale.x ,f_pos,24.0f,24.0f);

        fireball->GetAnimationObject()->AddNewAnimation(roll);
        fireball->GetAnimationObject()->AddNewAnimation(explode);

        fireballs.push_back(fireball);
    }
}

std::vector<std::shared_ptr<Fireball>> Mario::GetFireballs(){
    auto obj = fireballs;
    fireballs.clear();
    return obj;
}

void Mario::DestroyMarkedFireball(){
    fireballs.erase(std::remove_if(fireballs.begin(), fireballs.end(),
        [](const std::shared_ptr<Fireball>& fireball) {
            return fireball->IsMarkedRemove(); 
        }),
        fireballs.end());
}