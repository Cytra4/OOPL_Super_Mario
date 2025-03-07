#include "Characters/Character.hpp"
#include "Util/Image.hpp"
#include "Util/Animation.hpp"
#include "Util/Logger.hpp"

Character::Character(std::string ImagePath, int movespeed, glm::vec2 position, float width, float height){
    this->movespeed = movespeed;
    defaultSprite = ImagePath;
    box = CollisionBox(position, width, height);
    SetDefaultSprite(defaultSprite);
    SetPosition(position);
}

std::string Character::GetDefaultSprite(){
    return defaultSprite;
}

void Character::SetDefaultSprite(std::string sprite){
    defaultSprite = sprite;
    m_Drawable = std::make_shared<Util::Image>(defaultSprite);
}

void Character::AddNewAnimation(std::vector<std::string> animation){
    animationPaths.push_back(animation);
}

void Character::SetAnimation(int index, int interval){
    std::vector<std::string> animation;
    if (animationPaths.size() < 1 || index >= int(animationPaths.size())){
        LOG_ERROR("ANIMATION SETTING FAILED");
    }
    else{
        animation = animationPaths[index];
        m_Drawable = std::make_shared<Util::Animation>(animation, false, interval, false, 0);
    }
}

std::vector<std::vector<std::string>>& Character::GetAnimationPaths(){
    return animationPaths;
}

void Character::SetLooping(bool looping){
    auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
    temp->SetLooping(looping);
}

bool Character::IfAnimationEnds(){
    auto animation = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
    return animation->GetCurrentFrameIndex() == animation->GetFrameCount() - 1;
}

bool Character::IsPlaying(){
    return std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->GetState() == Util::Animation::State::PLAY;
}

void Character::PlayAnimation(){
    auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
    temp->Play();
}

void Character::PauseAnimation(){
    auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
    temp->Pause();
}

glm::vec2 Character::GetPosition(){
    return m_Transform.translation;
}

void Character::SetVelocity(glm::vec2 velocity){
    this->velocity = velocity;
}

glm::vec2 Character::GetVelocity(){
    return velocity;
}

bool Character::GetVisibility(){
    return m_Visible;
}

void Character::SetPosition(const glm::vec2& Position){
    m_Transform.translation = Position;
}

CollisionBox& Character::GetBox(){
    return box;
}

CollisionBox Character::GetStandingOnBlock(){
    return standingOn;
}

void Character::SetStandingOnBlock(CollisionBox block){
    standingOn = block;
}

void Character::SetFacingRight(bool direction){
    facingRight = direction;
}

bool Character::IsFacingRight(){
    return facingRight;
}

void Character::SetOnGround(bool onGround){
    this->onGround = onGround;
}

bool Character::IsOnGround(){
    return onGround;
}

void Character::SetDead(bool dead){
    this->dead = dead;
}

bool Character::IsDead(){
    return dead;
}

void Character::SetHealth(int new_health){
    health = new_health;
}

int Character::GetHealth(){
    return health;
}