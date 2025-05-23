#ifndef FIREBALL_HPP
#define FIREBALL_HPP

#include "pch.hpp"
#include "GameUtils/CollisionBox.hpp"
#include "GameUtils/AnimationObject.hpp"
#include <vector>
#include <string>

enum class FireballType{MARIO,BOWSER,BLOCK};

//Fireball class for mario's fireball, bowser's fireball and 1-4 fireball trap
class Fireball{
private:
    std::shared_ptr<AnimationObject> ani_obj;
    FireballType type;
    CollisionBox box;

    glm::vec2 velocity = {0.0f,0.0f};
    int speed;
    float jumpPower = 300.0f;
    float gravity = -500.0f;
    bool remove = false;
    bool explode = false;
    bool facingRight = false;

public:
    Fireball(FireballType type, std::string ImagePath, int speed, glm::vec2 position, float width, float height);

    FireballType GetType();

    void Behavior(double time);

    void PhysicProcess(double time);

    bool OutOfRange(glm::vec2 camPos);

    void AnimationHandle();

    void SetFacingRight(bool choice);

    bool IsFacingRight();

    void MarkRemove();

    bool IsMarkedRemove();

    std::shared_ptr<AnimationObject> GetAnimationObject();

    CollisionBox& GetBox();

    void SetVelocity(glm::vec2 new_velo);

    glm::vec2 GetVelocity();

    int GetJumppower();

    void SetExplode();
};

#endif

//Animations: 
//0.Roll
//1.Explode