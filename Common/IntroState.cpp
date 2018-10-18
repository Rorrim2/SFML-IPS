#include "IntroState.h"
#include <SFML/Graphics.hpp>


IntroState::IntroState(StateManager *stateManager)
   :BaseState(stateManager)
{
}

IntroState::~IntroState()
{
}

void IntroState::onCreate()
{
   this->timePassed = 0.f;

   this->font.loadFromFile("Charmonman-Bold.ttf");
   this->introText.setFont(this->font);
   this->introText.setString("Gówno gra na ips!");
   this->introText.setCharacterSize(30);
   sf::Vector2u windowSize = this->stateManager->getContext()->window->getWindowSize();
   this->introText.setPosition(windowSize.x / 2, windowSize.y / 2);
}

void IntroState::onDestroy()
{
}

void IntroState::draw()
{
  // if(this->timePassed >= 5.f)
      this->stateManager->getContext()->window->draw(this->introText);
}

void IntroState::update(const sf::Time & time)
{
   if (this->timePassed < 5.f)
   {
      this->timePassed += time.asSeconds();
   }
}

void IntroState::activate()
{}

void IntroState::deactivate()
{}
