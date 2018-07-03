#include "C_TimeAlive.hpp"

C_TimeAlive::C_TimeAlive(Object* owner) : Component(owner), timeAlive(0.f) {}

void C_TimeAlive::Update(float deltaTime)
{
    timeAlive += deltaTime;
}

float C_TimeAlive::Get() const
{
    return timeAlive;
}
