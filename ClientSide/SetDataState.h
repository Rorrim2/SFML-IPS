#pragma once
#include "BaseState.h"
#include "StateManager.h"
#include <SFML/Graphics/Text.hpp>

class SetDataState : public BaseState
{
public:
	SetDataState(StateManager* stateManager);
	~SetDataState();

	void onCreate() override;
	void onDestroy() override;

	void draw() override;
	void update(const sf::Time &time) override;

	void activate() override;
	void deactivate() override;

	void backToMenu(EventDetails* details);
	void PressEnter(EventDetails* details);
	void PressBackSpace(EventDetails* details);

private: 
	std::map<unsigned int, sf::Text> Outputs;
	std::map<unsigned int, sf::Text> infoText;
	std::vector<sf::String> text;
	sf::Text enterText;
	sf::Font font;
};

