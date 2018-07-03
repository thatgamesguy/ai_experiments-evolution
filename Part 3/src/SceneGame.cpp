#include "SceneGame.hpp"

SceneGame::SceneGame(WorkingDirectory& workingDir, ResourceAllocator<sf::Texture>& textureAllocator, Window& window) : workingDir(workingDir), textureAllocator(textureAllocator), mapParser(textureAllocator), window(window) { }

void SceneGame::OnCreate()
{
    const unsigned int numOfUFOsToSpawn = 80;
    
    for (int i = 0; i < numOfUFOsToSpawn; i++)
    {
        SpawnUFO();
    }
}

void SceneGame::OnDestroy()
{
    
}

void SceneGame::ProcessInput()
{
    input.Update();
}

void SceneGame::Update(float deltaTime)
{
    objects.ProcessRemovals();
    objects.ProcessNewObjects();
    
    objects.Update(deltaTime);
}

void SceneGame::LateUpdate(float deltaTime)
{
    objects.LateUpdate(deltaTime);
}

void SceneGame::Draw(Window& window)
{
    objects.Draw(window);
}

void SceneGame::SpawnUFO()
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
    
    // Remove the below lines. Our neural network will not set the UFOs velocity.
    /*
    const float maxVelocity = 80.f;
    const float range = maxVelocity * 2.f;
    const float randVelX = range * ((((float) rand()) / (float) RAND_MAX)) - maxVelocity;
    const float randVelY = range * ((((float) rand()) / (float) RAND_MAX)) - maxVelocity;
    auto velocity = ufo->AddComponent<C_Velocity>();
    velocity->Set({randVelX, randVelY});
     */
    
    // Add these lines:
    ufo->AddComponent<C_Velocity>();
    auto sight = ufo->AddComponent<C_Sight>();
    sight->SetObjectCollection(&objects);
    auto neuralNet = ufo->AddComponent<C_NeuralNetwork>();
    neuralNet->SetWindowSize(windowSize);

    const sf::IntRect spriteRect = sprite->GetTextureRect();
    auto wrapAround = ufo->AddComponent<C_ScreenWrapAround>();
    wrapAround->SetScreenSize({(int)windowRect.width, (int)windowRect.height});
    wrapAround->SetSpriteHalfSize({(int)(spriteRect.width * 0.5f), (int)(spriteRect.height * 0.5f)});
    
    objects.Add(ufo); // Adds object to game.
}
