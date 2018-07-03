#ifndef GeneticAlgorithm_hpp
#define GeneticAlgorithm_hpp

#include "WorkingDirectory.hpp"
#include "ResourceAllocator.hpp"
#include "ObjectCollection.hpp"
#include "C_Sprite.hpp"
#include "C_Animation.hpp"
#include "C_Velocity.hpp"
#include "C_ScreenWrapAround.hpp"
#include "C_NeuralNetwork.hpp"
#include "C_Energy.hpp"
#include "C_DamageOnCollision.hpp"
#include "C_DamageOnWallHit.hpp"

struct UFOGAData
{
    std::shared_ptr<C_NeuralNetwork> neuralNet;
    std::shared_ptr<C_Energy> energy;
};

class GeneticAlgorithm
{
public:
    GeneticAlgorithm(ObjectCollection& objects, WorkingDirectory& workingDir, ResourceAllocator<sf::Texture>& textureAllocator, Window& window);
    
    void Update(float deltaTime);
    
private:
    static bool PoolSort(std::shared_ptr<UFOGAData> a, std::shared_ptr<UFOGAData> b);
    
    std::shared_ptr<C_NeuralNetwork> SpawnUFO();
    void AddToPool(std::shared_ptr<UFOGAData> ufo);
    
    std::vector<std::shared_ptr<UFOGAData>> pool;
    ObjectCollection& objects;
    WorkingDirectory& workingDir;
    ResourceAllocator<sf::Texture>& textureAllocator;
    Window& window;
};

#endif /* GeneticAlgorithm_hpp */
