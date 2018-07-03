
#include "GeneticAlgorithm.hpp"


GeneticAlgorithm::GeneticAlgorithm(ObjectCollection& objects, WorkingDirectory& workingDir, ResourceAllocator<sf::Texture>& textureAllocator, Window& window) : objects(objects), workingDir(workingDir), textureAllocator(textureAllocator), window(window), maxPoolSize(40)
{
    const unsigned int numOfUFOsToSpawn = 80;
    
    for (int i = 0; i < numOfUFOsToSpawn; i++)
    {
        SpawnUFO();
    }
}

void GeneticAlgorithm::Update(float deltaTime)
{
    std::vector<std::shared_ptr<Object>>& ufos = objects.GetObjects();
    
    for(auto& o : ufos)
    {
        auto energy = o->GetComponent<C_Energy>();
        
        if(energy != nullptr && energy->Get() <= 0)
        {
            auto neuralNetwork = o->GetComponent<C_NeuralNetwork>();
            
            if(neuralNetwork != nullptr)
            {
                std::shared_ptr<UFOGAData> gaData = std::make_shared<UFOGAData>();
                gaData->energy = energy;
                gaData->neuralNet = neuralNetwork;
                
                AddToPool(gaData);
            }
            
            o->QueueForRemoval();
        }
    }
}

void GeneticAlgorithm::AddToPool(std::shared_ptr<UFOGAData> ufo)
{
    pool.push_back(ufo);
    
    std::sort(pool.begin(), pool.end(), PoolSort);
    
    if(pool.size() > maxPoolSize)
    {
        pool.erase(pool.end() - 1);
    }
}

std::shared_ptr<C_NeuralNetwork> GeneticAlgorithm::SpawnUFO()
{
    std::shared_ptr<Object> ufo = std::make_shared<Object>(); // Create new object
    
    auto sprite = ufo->AddComponent<C_Sprite>(); // Add sprite component
    sprite->SetTextureAllocator(&textureAllocator); // This provides the means for the sprite component to load images by name.
    
    const std::string spritePreName = "ufo";
    const std::string spritePostName = ".png";
    const std::string ufoCount = std::to_string(1 + (std::rand() % (4 - 1 + 1)));
    sprite->Load(workingDir.Get() + spritePreName + ufoCount + spritePostName); // Builds random name, can be ufo1, ufo2, ufo3, or ufo4.
    sprite->SetPivot(0.5f, 0.5f);
    
    const sf::FloatRect windowRect = window.GetViewSpace();
    const sf::Vector2u windowSize(windowRect.width, windowRect.height);
    const int minX = 50;
    const int minY = 50;
    const int maxX = windowSize.x - minX;
    const int maxY = windowSize.y - minY;
    const int randX = minX + (std::rand() % (maxX - minX + 1));
    const int randY = minY + (std::rand() % (maxY - minY + 1));
    ufo->transform->SetPosition(randX, randY);

    ufo->AddComponent<C_Velocity>();
    auto sight = ufo->AddComponent<C_Sight>();
    sight->SetObjectCollection(&objects);
    auto neuralNet = ufo->AddComponent<C_NeuralNetwork>();
    neuralNet->SetWindowSize(windowSize);
    
    const sf::IntRect spriteRect = sprite->GetTextureRect();
    auto wrapAround = ufo->AddComponent<C_ScreenWrapAround>();
    wrapAround->SetScreenSize({(int)windowRect.width, (int)windowRect.height});
    wrapAround->SetSpriteHalfSize({(int)(spriteRect.width * 0.5f), (int)(spriteRect.height * 0.5f)});
    
    
    ufo->AddComponent<C_Energy>();
    auto wallHit = ufo->AddComponent<C_DamageOnWallHit>();
    wallHit->SetWindowSize(windowSize);
    ufo->AddComponent<C_DamageOnCollision>();
    
    objects.Add(ufo); // Adds object to game.
    
    
    return neuralNet;
}

bool GeneticAlgorithm::PoolSort(std::shared_ptr<UFOGAData> a, std::shared_ptr<UFOGAData> b)
{
    return a->energy->Get() > b->energy->Get();
}
