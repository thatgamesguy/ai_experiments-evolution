#include "C_Energy.hpp"

C_Energy::C_Energy(Object* owner) : Component(owner), value(100.f) {}

void C_Energy::Set(float newEnergy)
{
    value = newEnergy;
}

float C_Energy::Get() const
{
    return value;
}
