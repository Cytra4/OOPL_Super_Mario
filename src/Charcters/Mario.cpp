#include "Characters/Mario.hpp"
#include "Util/Logger.hpp"

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
    }

    AnimationHandle();
}

void Mario::PhysicProcess(double time){
    glm::vec2 mario_pos = GetPosition();
    glm::vec2 mario_velo = GetVelocity();
    CollisionBox::State state = GetBox().GetCurrentState();
    double deltaTime = time;

    glm::vec2 new_pos;
    new_pos.x = mario_pos.x + deltaTime*mario_velo.x;
    new_pos.y = mario_pos.y + deltaTime*mario_velo.y;
    SetPosition(new_pos);
    GetBox().SetPosition(new_pos);

    if (state != CollisionBox::State::COLLISION_ON_BOTTOM){
        SetOnGround(false);
    }
    else{
        SetOnGround(true);
        CollisionBox b = GetStandingOnBlock();
        float floorY = new_pos.y - b.GetHeight() / 2;
        new_pos.y = floorY + b.GetHeight() / 2;
    }
    
    if (state == CollisionBox::State::COLLISION_ON_LEFT || state == CollisionBox::State::COLLISION_ON_RIGHT){
        mario_velo.x = 0;
    }
    else if (state == CollisionBox::State::COLLISION_ON_TOP || state == CollisionBox::State::COLLISION_ON_BOTTOM){
        mario_velo.y = 0;
    }

    //after jump to the highest point, mario falls down 2x gravity
    if (mario_velo.y <= 0 && !IsOnGround()){
        jumpFallGravity = 2;
    }

    if (!IsOnGround()){
        mario_velo.y = mario_velo.y + deltaTime*-500*jumpFallGravity;
    }
    else{
        mario_velo.y = 0;
        jumpFallGravity = 1;
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
        SetDefaultSprite(GetDefaultSprite());
        current_animation = -1;
    }

    if (new_animation != -1 && new_animation != current_animation){
        SetAnimation(new_animation, 25);
        SetLooping(true);
        PlayAnimation();
        current_animation = new_animation;
    }

    m_Transform.scale = (IsFacingRight()) ? glm::vec2(1,1) : glm::vec2(-1,1);
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
    LOG_DEBUG("TRIGGERED");
    std::string new_state;
    switch(new_mode){
        case Mode::SMALL:
            new_state = "Small";
            SetHealth(1);
            break;
        case Mode::BIG:
            new_state = "Big";
            SetHealth(2);
            break;
        case Mode::FIRE:
            new_state = "Fire";
            SetHealth(3);
            break;
        default:
            LOG_ERROR("Unexpected Mario Mode?");
            return;
    }
    LOG_DEBUG("PASSED");
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

    //*The new collision box will just be set to the size of image for now

    std::string dftSprite = GetDefaultSprite();
    std::vector<std::vector<std::string>>& animations = GetAnimationPaths();

    size_t pos = dftSprite.find(old_state);
    if (pos != std::string::npos){
        dftSprite.replace(pos, old_state.length(), new_mode);
        SetDefaultSprite(dftSprite);
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