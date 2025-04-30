#include "GameUtils/IEnemyAttribute.hpp"

int IEnemyAttribute::GetDeath(){
    return death;
}

void IEnemyAttribute::SetTimer(double new_time){
    deathTimer = new_time;
}