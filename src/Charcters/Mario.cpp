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
    CollisionBox::State state = GetBox().GetCurrentState();
    CollisionBox b = GetStandingOnBlock();
    double deltaTime = time;

    glm::vec2 new_pos;
    new_pos.x = mario_pos.x + deltaTime*mario_velo.x;
    new_pos.y = mario_pos.y + deltaTime*mario_velo.y;
    ani_obj->SetPosition(new_pos);
    GetBox().SetPosition(new_pos);

    if (state != CollisionBox::State::COLLISION_ON_BOTTOM){
        SetOnGround(false);
    }
    else{
        SetOnGround(true);
    }
    
    if (state == CollisionBox::State::COLLISION_ON_LEFT || state == CollisionBox::State::COLLISION_ON_RIGHT){
        mario_velo.x = 0;
    }
    else if (state == CollisionBox::State::COLLISION_ON_TOP || state == CollisionBox::State::COLLISION_ON_BOTTOM){
        mario_velo.y = 0;
    }

    //after jump to the highest point, mario falls down 2x gravity
    if (mario_velo.y <= 0 && !IsOnGround()){
        jumpFallGravity = 3;
    }

    if (!IsOnGround()){
        mario_velo.y = mario_velo.y + deltaTime*-500*jumpFallGravity;
    }
    else{
        mario_velo.y = 0;
        jumpFallGravity = 1;

        float floorY = new_pos.y - b.GetHeight() / 2;
        new_pos.y = floorY + b.GetHeight() / 2;
    }

    SetVelocity(mario_velo);

    // if(new_pos.y <= -250){SetOnGround(true);}
    // else{SetOnGround(false);}

    // LOG_DEBUG("POSITION:{},{}",new_pos.x,new_pos.y);
    // LOG_DEBUG("VELOCITY:{},{}",mario_velo.x,mario_velo.y);
    // LOG_DEBUG(jumpFallGravity);
}

void Mario::AnimationHandle(){
    int new_animation = -1;
    if (IsDead()){
        new_animation = 2;
    }
    else if (!IsOnGround()){
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
    int health = GetHealth();
    if (health == 3){
        StateUpdate(Mode::BIG);
    }
    else if (health == 2){
        StateUpdate(Mode::SMALL);
    }
    else if (health == 1){
        SetDead(true);
    }
}

void Mario::StateUpdate(Mode new_mode){
    std::string new_state;
    switch(new_mode){
        case Mode::SMALL:
            new_state = "Small";
            SetHealth(1);
            canShootFireballs = false;
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
    switch(mario_mode){
        case Mode::SMALL:
            old_state = "Small";
            GetBox().SetWidth(36.0f);
            GetBox().SetHeight(48.0f);
            break;
        case Mode::BIG:
            old_state = "Big";
            GetBox().SetWidth(48.0f);
            GetBox().SetHeight(96.0f);
            break;
        case Mode::FIRE:
            old_state = "Fire";
            GetBox().SetWidth(48.0f);
            GetBox().SetHeight(96.0f);
            break;
        default:
            LOG_ERROR("Unexpected Mario Mode?");
            return;
    }

    //* ↑ The new collision box will just be set to the size of image for now

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
}

void Mario::ShootFireball(){
    if (canShootFireballs){
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
            f_pos += glm::vec2{50,0};
        }
        else{
            f_pos -= glm::vec2{50,0};
        }

        std::shared_ptr<Fireball> fireball = std::make_shared<Fireball>(FireballType::MARIO,RESOURCE_DIR"/Sprites/Mario/Fireball/fireball1.png"
        ,20,f_pos,24.0f,24.0f);

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