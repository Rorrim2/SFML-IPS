#pragma once
#include "StateManager.h"
#include "Window.h"

class Game
{
public:
   Game();
   ~Game();

   void render();
   void update();
   void lateUpdate();

   inline void restartClock();

   //Event Manager
   void moveSprite(EventDetails* details);

   Window* getWindow();

private:

   sf::Time elaspedTime;
   sf::Clock clock;

   Window window;
   StateManager stateManager;
   SharedContext context;
};

