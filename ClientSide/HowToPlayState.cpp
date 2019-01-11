#include "HowToPlayState.h"

HowToPlayState::HowToPlayState(StateManager *stateManager) : BaseState(stateManager)
{
}


HowToPlayState::~HowToPlayState()
{
}

void HowToPlayState::backToMenu(EventDetails *details)
{
	this->stateManager->switchTo(StateTypeE::MENU);
}

void HowToPlayState::MouseClick(EventDetails *details)
{
	sf::Vector2i mousePos = details->mouse;
	float halfX = this->buttonSize.x / 2.0f;
	float halfY = this->buttonSize.y / 2.0f;

	if (mousePos.x >= this->rect.getPosition().x - halfX &&
		mousePos.x <= this->rect.getPosition().x + halfX &&
		mousePos.y >= this->rect.getPosition().y - halfY &&
		mousePos.y <= this->rect.getPosition().y + halfY)
	{
		this->stateManager->switchTo(StateTypeE::MENU);
	}
}

void HowToPlayState::MouseLight(EventDetails* details) 
{
	sf::Vector2i mousePos = this->stateManager->getContext()->eventManager->GetMousePos(this->stateManager->getContext()->window->getRenderWindow());
	float halfX = this->buttonSize.x / 2.0f;
	float halfY = this->buttonSize.y / 2.0f;

	if (mousePos.x >= this->rect.getPosition().x - halfX &&
		mousePos.x <= this->rect.getPosition().x + halfX &&
		mousePos.y >= this->rect.getPosition().y - halfY &&
		mousePos.y <= this->rect.getPosition().y + halfY)
	{
		this->rect.setFillColor(sf::Color(150, 100, 40));
	}
	else
	{
		this->rect.setFillColor(sf::Color(130, 80, 40));
	}
}

void HowToPlayState::onCreate()
{
	sf::Vector2u windowSize = this->stateManager->getContext()->window->getWindowSize();

	buttonSize = sf::Vector2f(100.0f, 30.0f);
	buttonPos = sf::Vector2f(windowSize.x * 0.1, windowSize.y * 0.8);

	rect.setSize(buttonSize);
	rect.setOrigin(buttonSize.x / 2.0f, buttonSize.y / 2.0f);
	rect.setPosition(buttonPos);
	rect.setFillColor(sf::Color(130, 80, 40));

	font.loadFromFile("Noturnal Hand.ttf");
	exitText.setFont(font);
	exitText.setString(sf::String("EXIT"));
	exitText.setCharacterSize(20);

	sf::FloatRect textRect = exitText.getLocalBounds();
	exitText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	exitText.setPosition(windowSize.x * 0.1, windowSize.y * 0.8);

	//png
	this->htpTexture.loadFromFile("map.png");
	this->htpSprite.setTexture(this->htpTexture);
	this->htpSprite.scale(0.6, 0.6);
	this->htpSprite.setOrigin(htpTexture.getSize().x / 2.0f, htpTexture.getSize().y / 2.0f);
	this->htpSprite.setPosition(windowSize.x / 2.0f, windowSize.y * 0.4);


	EventManager* evMgr = stateManager->getContext()->eventManager;
	evMgr->AddCallback(StateTypeE::HOW_TO_PLAY, "KeyEscape", &HowToPlayState::backToMenu, this);
	evMgr->AddCallback(StateTypeE::HOW_TO_PLAY, "Mouse_Left", &HowToPlayState::MouseClick, this);
	evMgr->AddCallback(StateTypeE::HOW_TO_PLAY, "Mouse_Move", &HowToPlayState::MouseLight, this);
}

void HowToPlayState::onDestroy()
{
	EventManager* evMgr = stateManager->getContext()->eventManager;
	evMgr->RemoveCallback(StateTypeE::HOW_TO_PLAY, "KeyEscape");
	evMgr->RemoveCallback(StateTypeE::HOW_TO_PLAY, "Mouse_Left");
	evMgr->RemoveCallback(StateTypeE::HOW_TO_PLAY, "Mouse_Move");
}

void HowToPlayState::draw()
{
	sf::RenderWindow* window = stateManager->getContext()->window->getRenderWindow();
	window->draw(this->rect);
	window->draw(this->exitText);
	window->draw(this->htpSprite);
}

void HowToPlayState::update(const sf::Time &time)
{

}

void HowToPlayState::activate()
{

}

void HowToPlayState::deactivate()
{

}