#include "MenuState.h"

MenuState::MenuState(StateManager *stateManager) : BaseState(stateManager)
{
}


MenuState::~MenuState()
{
}

void MenuState::onCreate()
{
	font.loadFromFile("Noturnal Hand.ttf");
	menuText.setFont(font);
	menuText.setString(sf::String("MENU"));
	menuText.setCharacterSize(50);

	sf::Vector2u windowSize = this->stateManager->getContext()->window->getWindowSize();

	sf::FloatRect textRect = menuText.getLocalBounds();
	menuText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	menuText.setPosition(windowSize.x / 2.0f, windowSize.y * 0.1);

	buttonSize = sf::Vector2f(500.0f, 40.0f);
	buttonPos = sf::Vector2f(windowSize.x / 2.0f, windowSize.y * 0.5);
	buttonPadding = 4; //4px

	std::string str[3];
	str[0] = "PLAY";
	str[1] = "IDK WHAT IM DOING";
	str[2] = "EXIT";

	for (int i = 0; i < 3; ++i) 
	{
		sf::Vector2f buttonPosition(buttonPos.x, buttonPos.y + (i * (buttonSize.y + buttonPadding)));
		rects[i].setSize(buttonSize);
		rects[i].setFillColor(sf::Color::Blue);
		rects[i].setOrigin(buttonSize.x / 2.0f, buttonSize.y / 2.0f);
		rects[i].setPosition(buttonPosition);

		labels[i].setFont(font);
		labels[i].setString(sf::String(str[i]));
		labels[i].setCharacterSize(20);

		sf::FloatRect rect = labels[i].getLocalBounds();
		labels[i].setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
		labels[i].setPosition(buttonPosition);
	}

	EventManager* evMgr = stateManager->getContext()->eventManager;
	evMgr->AddCallback(StateTypeE::MENU, "Mouse_Left", &MenuState::MouseClick, this);
	//TODO for keys moving through menu
}

void MenuState::onDestroy()
{
	EventManager* evMgr = stateManager->getContext()->eventManager;
	evMgr->RemoveCallback(StateTypeE::MENU, "Mouse_Left");
}

void MenuState::update(const sf::Time &time)
{
}

void MenuState::draw() {
	sf::RenderWindow* window = stateManager->getContext()->window->getRenderWindow();
	window->draw(this->menuText);
	for (int i = 0; i < 3; ++i) {
		window->draw(rects[i]);
		window->draw(labels[i]);
	}
}


void MenuState::activate()
{
}

void MenuState::deactivate()
{
}

void MenuState::MouseClick(EventDetails* details)
{
}