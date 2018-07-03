#include "C_Sight.hpp"
#include "Object.hpp"

C_Sight::C_Sight(Object* owner) : Component(owner), calculated(false), sightRadius(100.f){}

void C_Sight::SetObjectCollection(ObjectCollection* collection)
{
    objectCollection = collection;
}

void C_Sight::LateUpdate(float deltaTime)
{
    agentsInSight.clear();
    closestAgent = nullptr;
    calculated = false;
}

const std::vector<UFOData>& C_Sight::GetAll()
{
    if(!calculated)
    {
        CalculateAgentsInSight();
        calculated = true;
    }
    
    return agentsInSight;
}

std::shared_ptr<UFOData> C_Sight::GetClosest()
{
    if(!calculated)
    {
        CalculateAgentsInSight();
        calculated = true;
    }
    
    return closestAgent;
}

float C_Sight::GetRadius() const
{
    return sightRadius;
}

void C_Sight::CalculateAgentsInSight()
{
    std::vector<std::shared_ptr<Object>>& objects = objectCollection->GetObjects();
    
    float closestDist = MAXFLOAT;
    const int thisID = owner->instanceID->Get();
    const sf::Vector2f& thisPos = owner->transform->GetPosition();
    
    for (int i = 0; i < objects.size(); i++)
    {
        std::shared_ptr<Object> other = objects[i];
        
        if(thisID != other->instanceID->Get())
        {
            const sf::Vector2f otherPos = other->transform->GetPosition();
            
            sf::Vector2f heading = otherPos - thisPos;
            float distance = Magnitude(heading);
            
            if(distance < sightRadius)
            {
                UFOData data;
                data.position = otherPos;
                data.heading = heading;
                data.distance = distance;
                
                agentsInSight.push_back(data);
                
                if(distance < closestDist)
                {
                    closestAgent = std::make_shared<UFOData>(data);
                    closestDist = distance;
                }
            }
        }
    }
}
