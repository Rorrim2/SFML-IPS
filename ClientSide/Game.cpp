#include "Game.h"



Game::Game(const std::string &title, StateTypeE startState )
   : window(title, sf::Vector2u(900, 800)), stateManager(&context)
{
   this->context.window = &this->window;
   this->context.eventManager = window.getEventManager();
   this->stateManager.switchTo(startState);
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
}



void Game::run()
{
   sf::Time timeSinceLastUpdate = sf::Time::Zero;
   while (this->window.isDone() == false)
   {
      this->elaspedTime = this->clock.restart();
      timeSinceLastUpdate += this->elaspedTime;
      while (timeSinceLastUpdate >= this->timePerFrame)
      {
         timeSinceLastUpdate -= this->timePerFrame;
         update();
      }
      render();
      lateUpdate();
   }
}
