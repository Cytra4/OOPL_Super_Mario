#ifndef FIREBAR_HPP
#define FIREBAR_HPP

#include "Blocks/Block.hpp"
#include "Items/Fireball.hpp"
#include "Util/Renderer.hpp"

class FireBar : public Block{
private:
    std::vector<std::shared_ptr<Fireball>> fireballs;
    bool clock_wise;
    float angle;
    int rotation_speed = 2;
public:
    FireBar(std::string defaultPath, glm::vec2 pos, float width, float height, bool c_wise, float angle);

    virtual void PhysicProcess(double time) override;

    void FireballsInitial();

    void DrawFireballs(Util::Renderer& renderer);

    void RemoveFireballs(Util::Renderer& renderer);

    std::vector<std::shared_ptr<Fireball>> GetFireballs();
};

#endif