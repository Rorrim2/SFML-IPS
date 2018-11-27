#pragma once
#include "BaseState.h"
#include "StateManager.h"
#include <SFML/Graphics/Text.hpp>

class HowToPlayState : public BaseState
{
public:
	HowToPlayState(StateManager *stateManager);
	~HowToPlayState();

	void onCreate() override;
	void onDestroy() override;

	void draw() override;
	void update(const sf::Time &time) override;

	void activate() override;
	void deactivate() override;

	void backToMenu(EventDetails *details);
	void MouseClick(EventDetails *details);
	void MouseLight(EventDetails* details);
private:
	sf::Font font;
	sf::Text exitText;
	sf::RectangleShape rect;

	sf::Vector2f buttonSize;
	sf::Vector2f buttonPos;
};

