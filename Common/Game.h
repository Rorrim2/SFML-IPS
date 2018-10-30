#pragma once
#include "StateManager.h"
#include "Window.h"

class Game
{
public:
   Game(const std::string &title = "Game", StateTypeE startState = INTRO);
   ~Game();

   void render();
   void update();
   void lateUpdate();

   void run();

   Window* getWindow();

private:
   const sf::Time timePerFrame = sf::seconds(1.f / 60.f);
   const float timestamp = 1.0f / 60.0f;


   sf::Time elaspedTime;
   sf::Clock clock;

   Window window;
   StateManager stateManager;
   SharedContext context;
};

