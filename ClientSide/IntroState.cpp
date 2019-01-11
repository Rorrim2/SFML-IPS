#include "IntroState.h"
#include <SFML/Graphics.hpp>
#define INTO_TIME 4.0f

IntroState::IntroState(StateManager *stateManager)
   :BaseState(stateManager)
{
}

IntroState::~IntroState()
{
}

void IntroState::onCreate()
{
   //Fonts and names
   this->font.loadFromFile("Noturnal Hand.ttf"); 
   this->introText.setFont(this->font);
   this->introText.setString("Press SPACE to continue");
   this->introText.setCharacterSize(16);

   sf::Vector2u windowSize = this->stateManager->getContext()->window->getWindowSize();

   sf::FloatRect textRect = this->introText.getLocalBounds();
   this->introText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
   this->introText.setPosition(windowSize.x / 2.0, windowSize.y / 2.0 + 20);

   //png with name of game
   this->introTexture.loadFromFile("logo.png"); 
   this->introSprite.setTexture(introTexture);
   this->introSprite.setColor(sf::Color(255, 255, 255, transparentNum));
   this->introSprite.setOrigin(introTexture.getSize().x / 2.0f, introTexture.getSize().y / 2.0f);
   this->introSprite.setPosition(windowSize.x / 2.0f, windowSize.y * 0.4);

   //EventManager - to skip intro by spacebar
   EventManager* evMgr = this->stateManager->getContext()->eventManager; //getting instance of eventmanager from statemanager
   evMgr->AddCallback(StateTypeE::INTRO, "IntroContinue", &IntroState::Continue, this);    
 
   this->transparentNum = 0;
   //this->timePassed = 0.f;
}

void IntroState::onDestroy() {
	EventManager* evMgr = stateManager->getContext()->eventManager;
	evMgr->RemoveCallback(StateTypeE::INTRO, "IntroContinue"); 
}

void IntroState::draw()
{
	this->stateManager->getContext()->window->draw(this->introSprite);
	if (this->time.asSeconds() >= INTO_TIME)
	{
		this->stateManager->getContext()->window->draw(this->introText);
	}
}

void IntroState::update(const sf::Time & time)
{
	this->time = this->clock.getElapsedTime();
   if (this->time.asSeconds() < INTO_TIME)
   {
      if (this->transparentNum < 255)
      {
         this->transparentNum++;
         this->introSprite.setColor(sf::Color(255, 255, 255, transparentNum));
      }
   }
}

void IntroState::activate()
{}

void IntroState::deactivate()
{}

void IntroState::Continue(EventDetails* details) {
      stateManager->switchTo(StateTypeE::MENU);
      stateManager->remove(StateTypeE::INTRO);
}