#ifndef FIREBALL_HPP
#define FIREBALL_HPP

#include "GameUtils/CollisionBox.hpp"
#include "Util/GameObject.hpp"
#include <vector>
#include <string>

//This is going to be Mario's fireball, but I'm still thinking if it could work with Bowser's fireball or the fireball block in 1-4
//(Maybe this could be a base class for all the fireballs, Fireball->MarioFireball, BowserFireball, but my brain hurts from thinking this)

enum class FireballType{MARIO,BOWSER,BLOCK};

class Fireball : public Util::GameObject{
private:
    //0. Roll 1. Explode
    std::vector<std::vector<std::string>> animations;
    FireballType type;
    CollisionBox box;
    glm::vec2 position;
    glm::vec2 velocity = {0.0f,0.0f};
    int speed;
    int currentAnimation = -1;
    bool exploded = false;
    bool remove = false;
    bool isFacingRight;
public:

    Fireball(int speed, float width, float height, bool facingRight,
        FireballType type, glm::vec2 pos, std::vector<std::vector<std::string>> paths);
    
    FireballType GetType();

    void PhysicProcess();

    bool OutOfRange(glm::vec2 camPos);

    void MarkRemove();
    
    bool IsMarkedRemove();

    void AnimationHandle();

    void SetAnimation(int index, int interval);

    void SetLooping(bool looping);

    void PlayAnimation();
};

#endif