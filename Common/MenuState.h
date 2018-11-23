#pragma once
#include "BaseState.h"
#include "StateManager.h"
#include <SFML/Graphics/Text.hpp>

class MenuState : public BaseState
{
public:
	MenuState(StateManager *stateManager);
	~MenuState();

	void onCreate() override;
	void onDestroy() override;

	void draw() override;
	void update(const sf::Time &time) override;

	void activate() override;
	void deactivate() override;

	void MouseClick(EventDetails* details);
	//TODO with keys also
private:
	sf::Font font;
	sf::Text menuText;

	sf::Vector2f buttonSize;
	sf::Vector2f buttonPos;
	unsigned int buttonPadding;

	sf::RectangleShape rects[3];
	sf::Text labels[3];
};

