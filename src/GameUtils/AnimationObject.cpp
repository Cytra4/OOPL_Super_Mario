#include "GameUtils/AnimationObject.hpp"
#include "Util/Image.hpp"
#include "Util/Animation.hpp"
#include "Util/Logger.hpp"

AnimationObject::AnimationObject(std::string defaultPath, glm::vec2 pos){
    defaultSprite = defaultPath;
    SetPosition(pos);
    SetDefaultSprite(defaultSprite);
}

void AnimationObject::SetPosition(const glm::vec2& Position){
    m_Transform.translation = Position;
}

glm::vec2 AnimationObject::GetPosition(){
    return m_Transform.translation;
}

std::string AnimationObject::GetDefaultSprite(){
    return defaultSprite;
}

void AnimationObject::SetDefaultSprite(std::string sprite){
    defaultSprite = sprite;
    m_Drawable = std::make_shared<Util::Image>(defaultSprite);
}

void AnimationObject::AddNewAnimation(std::vector<std::string> animation){
    animationPaths.push_back(animation);
}

void AnimationObject::SetAnimation(int index, int interval){
    std::vector<std::string> animation;
    if (animationPaths.size() < 1 || index >= int(animationPaths.size())){
        LOG_ERROR("ANIMATION SETTING FAILED");
    }
    else{
        animation = animationPaths[index];
        m_Drawable = std::make_shared<Util::Animation>(animation, false, interval, false, 0);
    }
}

std::vector<std::vector<std::string>>& AnimationObject::GetAnimationPaths(){
    return animationPaths;
}

void AnimationObject::SetFacingRight(bool right){
    facingRight = right;
}

bool AnimationObject::IsFacingRight(){
    return facingRight;
}

void AnimationObject::SetCurrentAnimation(int i){
    current_animation = i;
}

int AnimationObject::GetCurrentAnimation(){
    return current_animation;
}

void AnimationObject::SetLooping(bool looping){
    auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
    temp->SetLooping(looping);
}

void AnimationObject::SetScale(glm::vec2 scale){
    m_Transform.scale = scale;
}

bool AnimationObject::IfAnimationEnds(){
    auto animation = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
    return animation->GetCurrentFrameIndex() == animation->GetFrameCount() - 1;
}

bool AnimationObject::IsPlaying(){
    return std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->GetState() == Util::Animation::State::PLAY;
}

void AnimationObject::PlayAnimation(){
    auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
    temp->Play();
}

void AnimationObject::PauseAnimation(){
    auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
    temp->Pause();
}