#include "BaseState.h"
#include "StateManager.h"
#include <rapidjson/document.h>

class ConfirmState : public BaseState
{
public:
	ConfirmState(StateManager* stateManager);
	~ConfirmState();

	void onCreate() override;
	void onDestroy() override;

	void draw() override;
	void update(const sf::Time &time) override;

	void activate() override;
	void deactivate() override;

	void loadFromJson();

	void backToMenu(EventDetails* details);
	void PressEnter(EventDetails* details);
	void MouseClick(EventDetails* details);
	void HighlightShip(EventDetails* details);
	void KeyLeft(EventDetails* details);
	void KeyRight(EventDetails* details);

private:
	sf::Text enterText[2];
	std::map<std::string, std::string> jsonText;
	std::map<unsigned int, sf::Text> jsonInfo;

	sf::RectangleShape rects[3];
	sf::Text confText[3];
	std::vector<sf::String> conf;

	sf::Font font;

	int currentButton;
};

