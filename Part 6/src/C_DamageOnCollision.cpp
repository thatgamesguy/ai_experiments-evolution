#include "C_DamageOnCollision.hpp"
#include "Object.hpp"

C_DamageOnCollision::C_DamageOnCollision(Object* owner) : Component(owner), energyReducedOnContact(100.f){}

void C_DamageOnCollision::Awake()
{
    energy = owner->GetComponent<C_Energy>();
    sight = owner->GetComponent<C_Sight>();
}

void C_DamageOnCollision::Update(float deltaTime)
{
    const std::vector<UFOData>& agentsInSight = sight->GetAll();
    const float agentRadius = sight->GetRadius();
    
    for (int i = 0; i < agentsInSight.size(); i++)
    {
        const UFOData& data = agentsInSight[i];
        
        if(data.distance <= agentRadius * 0.5f)
        {
            energy->Set(energy->Get() - energyReducedOnContact * deltaTime);
        }
    }
}
