#include "SetDataState.h"

SetDataState::SetDataState(StateManager* stateManager) : BaseState(stateManager)
{
	this->text.push_back(sf::String("Enter Server IP:"));
	this->text.push_back(sf::String("Enter Server Port:"));
	this->text.push_back(sf::String("Enter your name:"));
	this->text.push_back(sf::String("Choose your fighter:"));

	this->shipTextures[ShipType::RED].loadFromFile("red.png");
	this->shipTextures[ShipType::GREEN].loadFromFile("green.png");
	this->shipTextures[ShipType::BLUE].loadFromFile("blue.png");
	this->shipTextures[ShipType::YELLOW].loadFromFile("yellow.png");

	this->shipSprites[ShipType::RED].setTexture(this->shipTextures[ShipType::RED]);
	this->shipSprites[ShipType::GREEN].setTexture(this->shipTextures[ShipType::GREEN]);
	this->shipSprites[ShipType::BLUE].setTexture(this->shipTextures[ShipType::BLUE]);
	this->shipSprites[ShipType::YELLOW].setTexture(this->shipTextures[ShipType::YELLOW]);
}

void SetDataState::onCreate()
{
	//set true
	this->stateManager->getContext()->window->SetIsDataStateFlag();
	this->font.loadFromFile("Noturnal Hand.ttf");
	sf::Vector2u windowSize = this->stateManager->getContext()->window->getWindowSize();

	this->enterText.setString(sf::String("Press ENTER to continue"));
	this->enterText.setFont(font);
	this->enterText.setCharacterSize(22);
	this->enterText.setPosition(windowSize.x * 0.35, windowSize.y * 0.8);

	for (unsigned int i = 0; i < this->text.size(); ++i) {
		this->infoText[i] = sf::Text(this->text[i], font, 22);
		this->infoText[i].setPosition(windowSize.x * 0.1, windowSize.y * (i * 0.1 + 0.10));
		this->Outputs[i] = sf::Text();
		this->Outputs[i].setFont(font);
		this->Outputs[i].setCharacterSize(22);
		this->Outputs[i].setPosition(windowSize.x * 0.1, windowSize.y * (i * 0.1 + 0.15));
	}

	//ships
	this->shipSprites[ShipType::RED].setPosition(windowSize.x * 0.1 , windowSize.y * 0.5);
	this->shipSprites[ShipType::GREEN].setPosition(windowSize.x * 0.25, windowSize.y * 0.5);
	this->shipSprites[ShipType::BLUE].setPosition(windowSize.x * 0.4, windowSize.y * 0.5);
	this->shipSprites[ShipType::YELLOW].setPosition(windowSize.x * 0.55, windowSize.y * 0.5);

	this->ShipSize = this->shipSprites[ShipType::RED].getLocalBounds();
	this->boundingBox[0] = this->shipSprites[ShipType::RED].getOrigin();
	this->boundingBox[1] = this->shipSprites[ShipType::BLUE].getOrigin();
	this->boundingBox[2] = this->shipSprites[ShipType::YELLOW].getOrigin();
	this->boundingBox[3] = this->shipSprites[ShipType::GREEN].getOrigin();

	EventManager* evMgr = this->stateManager->getContext()->eventManager;
	evMgr->AddCallback(StateTypeE::SERVER_DATA, "EnterKey", &SetDataState::PressEnter, this);
	evMgr->AddCallback(StateTypeE::SERVER_DATA, "KeyEscape", &SetDataState::backToMenu, this);
	evMgr->AddCallback(StateTypeE::SERVER_DATA, "BackSpace", &SetDataState::PressBackSpace, this);
	evMgr->AddCallback(StateTypeE::SERVER_DATA, "Mouse_Move", &SetDataState::HighlightShip, this);
}

void SetDataState::onDestroy()
{
	EventManager* evMgr = this->stateManager->getContext()->eventManager;
	evMgr->RemoveCallback(StateTypeE::SERVER_DATA, "EnterKey");
	evMgr->RemoveCallback(StateTypeE::SERVER_DATA, "KeyEscape");
	evMgr->RemoveCallback(StateTypeE::SERVER_DATA, "BackSpace");
	evMgr->RemoveCallback(StateTypeE::SERVER_DATA, "Mouse_Move");

	//set false
	this->stateManager->getContext()->window->SetIsDataStateFlag();
}

void SetDataState::draw()
{
	sf::RenderWindow *wind = this->stateManager->getContext()->window->getRenderWindow();
	for (auto it : this->infoText) 
	{
		wind->draw(it.second);
	}
	for (auto it : this->Outputs)
	{
		wind->draw(it.second);
	}

	
	for (auto it : this->shipSprites)
	{
		wind->draw(it.second);
	}

	if (this->stateManager->getContext()->window->getCounter() == this->text.size())
	{
		wind->draw(this->enterText);
	}
	//std::cout << "draw" << std::endl;
}

void SetDataState::update(const sf::Time &time)
{
	Window *wind = this->stateManager->getContext()->window;
	this->Outputs[wind->getCounter()].setString(sf::String(wind->getInput()));
}

void SetDataState::PressEnter(EventDetails* details)
{	
	Window *wind = this->stateManager->getContext()->window;
	if (this->stateManager->getContext()->window->getCounter() == this->text.size())
	{
		this->stateManager->switchTo(StateTypeE::GAME);
	}
	wind->incrementCounter();
}

void SetDataState::PressBackSpace(EventDetails* details)
{
	Window *wind = this->stateManager->getContext()->window;
	if (wind->getInput().size() != 0) {
		wind->deleteChar();
	}
}

void SetDataState::backToMenu(EventDetails* details)
{
	this->stateManager->switchTo(StateTypeE::MENU);
}

void SetDataState::HighlightShip(EventDetails* details)
{
	sf::Vector2i mousePos = this->stateManager->getContext()->eventManager->GetMousePos(this->stateManager->getContext()->window->getRenderWindow());

	for (unsigned int i = 0; i < 4; ++i)
	{
		//std::cout << this->boundingBox[i].left << " " << this->boundingBox[i].top << " " << this->boundingBox[i].width << " " << this->boundingBox[i].height << std::endl;
		if (mousePos.x >= this->boundingBox[i].x &&
			mousePos.x <=  this->boundingBox[i].x + this->ShipSize.width &&
			mousePos.y >= this->boundingBox[i].y &&
			mousePos.y <= this->boundingBox[i].y + this->ShipSize.height)
		{
			//std::cout << this->boundingBox[i].left << " " << this->boundingBox[i].top << std::endl;
			this->shipSprites[ShipType::YELLOW].setColor(sf::Color(255, 255, 255, 127));
		}
		else
		{
			this->shipSprites[ShipType::YELLOW].setColor(sf::Color(255, 255, 255, 255));
		}
	}
}

void SetDataState::activate()
{

}

void SetDataState::deactivate()
{

}

SetDataState::~SetDataState()
{
}
