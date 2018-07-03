#ifndef C_Sight_hpp
#define C_Sight_hpp

#include <math.h>

#include "Component.hpp"
#include "ObjectCollection.hpp"

struct UFOData
{
    sf::Vector2f position;
    sf::Vector2f heading;
    float distance;
};

class C_Sight : public Component
{
public:
    C_Sight(Object* owner);
    
    void SetObjectCollection(ObjectCollection* collection);
    
    void LateUpdate(float deltaTime) override;
    
    const std::vector<UFOData>& GetAll();
    
    std::shared_ptr<UFOData> GetClosest();
    
    float GetRadius() const;
    
private:
    inline float Magnitude(const sf::Vector2f& v)
    {
        return sqrt((v.x * v.x) + (v.y * v.y));
    }
    
    inline sf::Vector2f Normalise(const sf::Vector2f& v)
    {
        float l = Magnitude(v);
        
        if (l != 0)
        {
            return v / l;
        }
        
        return v;
    }
    
    void CalculateAgentsInSight();
    
    float sightRadius;
    ObjectCollection* objectCollection;
    bool calculated;
    std::vector<UFOData> agentsInSight;
    std::shared_ptr<UFOData> closestAgent;
};

#endif /* C_Sight_hpp */
