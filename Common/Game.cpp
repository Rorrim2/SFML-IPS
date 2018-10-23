#include "Game.h"



Game::Game()
   : window("Tytu³", sf::Vector2u(700, 500)), stateManager(&context)
{
   this->context.window = &this->window;
   this->stateManager.switchTo(StateTypeE::INTRO);
}


Game::~Game()
{
}

void Game::render()
{
   this->window.beginDraw();
   this->stateManager.draw();
   this->window.endDraw();
}

void Game::update()
{
   window.update();
   this->stateManager.update(this->elaspedTime);
}

Window* Game::getWindow()
{
   return &this->window;
}


void Game::lateUpdate()
{
   this->stateManager.processRequest();
   restartClock();
}

inline void Game::restartClock()
{
   this->elaspedTime += this->clock.restart();
}
