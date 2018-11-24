#include "IntroState.h"
#include <SFML/Graphics.hpp>
#define INTO_TIME 50.0f

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

   //Fonts and names
   this->font.loadFromFile("Noturnal Hand.ttf"); 
   this->introText.setFont(this->font);
   this->introText.setString("Press SPACE to continue");
   this->introText.setCharacterSize(16);

   sf::Vector2u windowSize = this->stateManager->getContext()->window->getWindowSize();

   sf::FloatRect textRect = this->introText.getLocalBounds();
   this->introText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
   this->introText.setPosition(windowSize.x / 2.0, windowSize.y / 2.0 + 20);

   //TODO png with name of game
   introTexture.loadFromFile("logo.png"); 
   introSprite.setTexture(introTexture);
   introSprite.setOrigin(introTexture.getSize().x / 2.0f, introTexture.getSize().y / 2.0f);
   introSprite.setPosition(windowSize.x / 2.0f, windowSize.y * 0.4);

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
	this->stateManager->getContext()->window->draw(this->introSprite);
	if (this->timePassed < INTO_TIME)
	{
		this->stateManager->getContext()->window->draw(this->introText);
	}
      
	//TODO after update the sprite will be draw on the new position
}

void IntroState::update(const sf::Time & time)
{
   if (this->timePassed < INTO_TIME)
   {
      this->timePassed += time.asSeconds();
   }
   else
   {
      Continue(nullptr);
   }
}

void IntroState::activate()
{}

void IntroState::deactivate()
{}

void IntroState::Continue(EventDetails* details) {
   if (this->timePassed >= INTO_TIME) {
      stateManager->switchTo(StateTypeE::MENU);
      stateManager->remove(StateTypeE::INTRO);
   }
}