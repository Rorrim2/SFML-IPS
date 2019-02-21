#include "GameOverState.h"


GameOverState::GameOverState(StateManager *stateManager) : BaseState(stateManager)
{
}


GameOverState::~GameOverState()
{
}

void GameOverState::onCreate()
{
	sf::Vector2u windowSize = this->stateManager->getContext()->window->getWindowSize();

	this->rectSourceSprite = sf::IntRect(0, 0, 400, 100);
	this->imageHeight = 100;
	this->imageWidth = 400;

	//png
	this->goTexture.loadFromFile("gameover.png");
	this->goSprite = sf::Sprite(this->goTexture, rectSourceSprite);

	this->goSprite.setOrigin(this->imageWidth / 2.0f, this->imageHeight / 2.0f);
	this->goSprite.setPosition(windowSize.x / 2.0f, windowSize.y * 0.4);

	EventManager* evMgr = stateManager->getContext()->eventManager;
	evMgr->AddCallback(StateTypeE::GAME_OVER, "EnterKey", &GameOverState::backToMenu, this);
}

void GameOverState::onDestroy()
{
	EventManager* evMgr = stateManager->getContext()->eventManager;
	evMgr->RemoveCallback(StateTypeE::GAME_OVER, "EnterKey");
}

void GameOverState::draw()
{
	sf::RenderWindow* window = stateManager->getContext()->window->getRenderWindow();
	window->draw(this->goSprite);
}

void GameOverState::update(const sf::Time &time)
{
	this->time = this->clock.getElapsedTime();
	if (this->time.asSeconds() > 0.5)
	{
		if (this->rectSourceSprite.left == this->imageWidth && this->rectSourceSprite.top == this->imageHeight)
		{
			this->rectSourceSprite.left = 0;
			this->rectSourceSprite.top = 0;
		}
		else if (this->rectSourceSprite.left == this->imageWidth && this->rectSourceSprite.top == 0)
		{
			this->rectSourceSprite.left = 0;
			this->rectSourceSprite.top = this->imageHeight;
		}
		else
		{
			this->rectSourceSprite.left += this->imageWidth;
		}

		this->goSprite.setTextureRect(rectSourceSprite);
		this->clock.restart();
	}
}

void GameOverState::activate()
{

}

void GameOverState::deactivate()
{

}

void GameOverState::backToMenu(EventDetails *details)
{
	this->stateManager->switchTo(StateTypeE::MENU);
}