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
   

   //Fonts and names
   this->font.loadFromFile("Noturnal Hand.ttf"); 
   this->introText.setFont(this->font);
   this->introText.setString("Press SPACE to continue");
   this->introText.setCharacterSize(18);

   sf::Vector2u windowSize = this->stateManager->getContext()->window->getWindowSize();

   sf::FloatRect textRect = this->introText.getLocalBounds();
   this->introText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
   this->introText.setPosition(windowSize.x / 2.0, windowSize.y / 2.0);

   //TODO png with name of game
   //introTexture.loadFromFile(".png"); 
   //introSprite.setTexture(introTexture);
   //introSprite.setOrigin(introTexture.getSize().x / 2.0f, introTexture.getSize().y / 2.0f);
   //introSprite.setPosition(windowSize.x / 2.0f, 0);

   //EventManager - to skip intro by spacebar
   EventManager* evMgr = this->stateManager->getContext()->eventManager; //getting instance of eventmanager from statemanager
   evMgr->AddCallback(StateTypeE::INTRO, "IntroContinue", &IntroState::Continue, this);    
 
   this->timePassed = 0.f;
}

void IntroState::onDestroy() {
	EventManager* evMgr = stateManager->getContext()->eventManager;
	evMgr->RemoveCallback(StateTypeE::INTRO, "IntroContinue"); 
}

void IntroState::draw()
{
	if (this->timePassed >= 1000.0f)
	{
		this->stateManager->getContext()->window->draw(this->introText);
	}
      
	//TODO after update the sprite will be draw on the new position
}

void IntroState::update(const sf::Time & time)
{
   if (this->timePassed < 1000.0f)
   {
      this->timePassed += time.asSeconds();
	  //TODO If we want change position of the sprite, we must do it here
   }
}

void IntroState::activate()
{}

void IntroState::deactivate()
{}

void IntroState::Continue(EventDetails* details) {
	if (timePassed >= 1000.0f) {
		stateManager->switchTo(StateTypeE::MENU);
		stateManager->remove(StateTypeE::INTRO);
	}
}