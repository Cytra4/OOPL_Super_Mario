#ifndef IENEMYATTRIBUTE_HPP
#define IENEMYATTRIBUTE_HPP

//The Attributes an enemy may have (Right now is all about death lol)

class IEnemyAttribute{
protected:
    //Enemy might have different reactions based on the death, ex: from Jump, from Fireball
    int death = 0;

    //The time enemy stays dead
    double deathTimer = 1.5f;
public:
    IEnemyAttribute() = default;

    virtual void SetDeath(int choice){choice = choice;};

    int GetDeath();

    void SetTimer(double new_time);
};

#endif