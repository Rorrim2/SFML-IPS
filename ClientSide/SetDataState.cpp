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
	this->shipSprites[ShipType::BLUE].setPosition(windowSize.x * 0.25, windowSize.y * 0.5);
	this->shipSprites[ShipType::YELLOW].setPosition(windowSize.x * 0.4, windowSize.y * 0.5);
	this->shipSprites[ShipType::GREEN].setPosition(windowSize.x * 0.55, windowSize.y * 0.5);

	this->ShipSize = this->shipSprites[ShipType::RED].getLocalBounds();
	this->boundingBox[ShipType::RED] = this->shipSprites[ShipType::RED].getPosition();
	this->boundingBox[ShipType::BLUE] = this->shipSprites[ShipType::BLUE].getPosition();
	this->boundingBox[ShipType::YELLOW] = this->shipSprites[ShipType::YELLOW].getPosition();
	this->boundingBox[ShipType::GREEN] = this->shipSprites[ShipType::GREEN].getPosition();

	for (auto it : this->shipSprites)
	{
		it.second.setColor(sf::Color(255, 255, 255, 127));
	}

	EventManager* evMgr = this->stateManager->getContext()->eventManager;
	evMgr->AddCallback(StateTypeE::SERVER_DATA, "EnterKey", &SetDataState::PressEnter, this);
	evMgr->AddCallback(StateTypeE::SERVER_DATA, "KeyEscape", &SetDataState::backToMenu, this);
	evMgr->AddCallback(StateTypeE::SERVER_DATA, "BackSpace", &SetDataState::PressBackSpace, this);
	evMgr->AddCallback(StateTypeE::SERVER_DATA, "Mouse_Move", &SetDataState::HighlightShip, this);
	evMgr->AddCallback(StateTypeE::SERVER_DATA, "Mouse_Left", &SetDataState::MouseClick, this);
	evMgr->AddCallback(StateTypeE::SERVER_DATA, "KeyLeft", &SetDataState::KeyLeft, this);
	evMgr->AddCallback(StateTypeE::SERVER_DATA, "KeyRight", &SetDataState::KeyRight, this);

	this->counter = -1;
}

void SetDataState::onDestroy()
{
	EventManager* evMgr = this->stateManager->getContext()->eventManager;
	evMgr->RemoveCallback(StateTypeE::SERVER_DATA, "EnterKey");
	evMgr->RemoveCallback(StateTypeE::SERVER_DATA, "KeyEscape");
	evMgr->RemoveCallback(StateTypeE::SERVER_DATA, "BackSpace");
	evMgr->RemoveCallback(StateTypeE::SERVER_DATA, "Mouse_Move");
	evMgr->RemoveCallback(StateTypeE::SERVER_DATA, "Mouse_Left");
	evMgr->RemoveCallback(StateTypeE::SERVER_DATA, "KeyLeft");
	evMgr->RemoveCallback(StateTypeE::SERVER_DATA, "KeyRight");

	//reset
	this->stateManager->getContext()->window->SetIsDataStateFlag();
}

void SetDataState::saveToJson()
{
	Window *wind = this->stateManager->getContext()->window;
	
	rapidjson::StringBuffer strbuff;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(strbuff);

	writer.StartObject();
	writer.Key("IP");
	writer.String(wind->getInput(0).c_str());
	writer.Key("Port");
	writer.String(wind->getInput(1).c_str());
	writer.Key("PlayerName");
	writer.String(wind->getInput(2).c_str());
	writer.Key("ShipType");
	writer.String(wind->getInput(3).c_str());
	writer.EndObject();

	std::string outputPath = "output.json";
	std::FILE *fp = fopen(outputPath.c_str(), "w");
	std::fputs(strbuff.GetString(), fp);
	
	fclose(fp);
}

void SetDataState::loadFromJson()
{
	std::string outputPath = "output.json";
	std::FILE *fp = fopen(outputPath.c_str(), "r");

	std::string buff;
	char sign;
	while (true) {
		sign = fgetc(fp);
		if (sign == EOF)
		{
			break;
		}
		buff += sign;
	}

	rapidjson::Document doc;
	doc.Parse<0>(buff.c_str());

	fclose(fp);
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

	if (this->stateManager->getContext()->window->getCounter() >= this->text.size() - 1)
	{
		for (auto it : this->shipSprites)
		{
			wind->draw(it.second);
		}
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
	wind->deleteChar();
}

void SetDataState::backToMenu(EventDetails* details)
{
	this->stateManager->switchTo(StateTypeE::MENU);
}

void SetDataState::HighlightShip(EventDetails* details)
{
	sf::Vector2i mousePos = this->stateManager->getContext()->eventManager->GetMousePos(this->stateManager->getContext()->window->getRenderWindow());

	for (auto it : this->boundingBox) 
	{
		if (mousePos.x >= it.second.x &&
			mousePos.x <= it.second.x + this->ShipSize.width &&
			mousePos.y >= it.second.y &&
			mousePos.y <= it.second.y + this->ShipSize.height)
		{
			this->shipSprites[it.first].setColor(sf::Color(255, 255, 255, 255));
		}
		else
		{
			this->shipSprites[it.first].setColor(sf::Color(255, 255, 255, 127));
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
