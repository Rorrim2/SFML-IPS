#include "BaseState.h"
#include "StateManager.h"

class GameOverState : public BaseState
{
public:
	GameOverState(StateManager *stateManager);
	~GameOverState(); 
	
	void onCreate() override;
	void onDestroy() override;

	void draw() override;
	void update(const sf::Time &time) override;

	void activate() override;
	void deactivate() override;


	void backToMenu(EventDetails *details);
private:
	sf::Sprite goSprite;
	sf::Texture goTexture;

	sf::Clock clock;
	sf::Time time;

	sf::IntRect rectSourceSprite;
	unsigned int imageWidth;
	unsigned int imageHeight;
};

