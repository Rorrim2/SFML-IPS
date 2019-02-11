#include "ConfirmState.h"



ConfirmState::ConfirmState(StateManager* stateManager) : BaseState(stateManager)
{
	this->jsonText["IP"] = "";
	this->jsonText["Port"] = "";
	this->jsonText["PlayerName"] = "";
	this->jsonText["ShipType"] = "";

	this->conf.push_back("YES");
	this->conf.push_back("NO");
	this->conf.push_back("BACK TO MENU");
}


ConfirmState::~ConfirmState()
{
}


void ConfirmState::onCreate()
{
	//load
	loadFromJson();

	this->font.loadFromFile("Noturnal Hand.ttf");
	sf::Vector2u windowSize = this->stateManager->getContext()->window->getWindowSize();

	this->enterText[0].setString(sf::String("Recently you used this data to play:"));
	this->enterText[0].setFillColor(sf::Color(160, 100, 35));
	this->enterText[0].setFont(font);
	this->enterText[0].setCharacterSize(20);
	sf::FloatRect textRect = this->enterText[0].getLocalBounds();
	this->enterText[0].setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	this->enterText[0].setPosition(windowSize.x / 2.0f, windowSize.y * 0.15);

	this->enterText[1].setString(sf::String("Do you want to reuse it?"));
	this->enterText[1].setFillColor(sf::Color(160, 100, 35));
	this->enterText[1].setFont(font);
	this->enterText[1].setCharacterSize(20);
	textRect = this->enterText[1].getLocalBounds();
	this->enterText[1].setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	this->enterText[1].setPosition(windowSize.x / 2.0f, windowSize.y * 0.5);

	//jsonText
	unsigned int i = 0;
	for (auto it : this->jsonText) 
	{
		this->jsonInfo[i].setString(sf::String(it.first + ": " + it.second));
		this->jsonInfo[i].setFont(this->font);
		this->jsonInfo[i].setCharacterSize(20);
		this->jsonInfo[i].setPosition(windowSize.x * 0.25, windowSize.y * (i * 0.05 + 0.25));
		i++;
	}

	//buttons
	for (i = 0; i < 3; ++i) {
		sf::Vector2f buttonPosition;
		if (i == 2)
		{
			buttonPosition = sf::Vector2f(windowSize.x * 0.5, windowSize.y * 0.7);
			rects[i].setSize(sf::Vector2f(400.0f, 40.0f));
			rects[i].setOrigin(200.0f, 20.0f);
		}
		else
		{
			buttonPosition = sf::Vector2f(windowSize.x * (i * 0.3 + 0.35), windowSize.y * 0.6);
			rects[i].setSize(sf::Vector2f(200.0f, 40.0f));
			rects[i].setOrigin(100.0f, 20.0f);
		}
		
		this->rects[i].setFillColor(sf::Color(160, 100, 35));
		rects[i].setPosition(buttonPosition);

		confText[i].setFont(font);
		confText[i].setString(sf::String(conf[i]));
		confText[i].setCharacterSize(20);

		sf::FloatRect rect = confText[i].getLocalBounds();
		confText[i].setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
		confText[i].setPosition(buttonPosition);
	}

	this->currentButton = 0;

	EventManager* evMgr = this->stateManager->getContext()->eventManager;
	evMgr->AddCallback(StateTypeE::CONFIRM, "EnterKey", &ConfirmState::PressEnter, this);
	evMgr->AddCallback(StateTypeE::CONFIRM, "KeyEscape", &ConfirmState::backToMenu, this);
	evMgr->AddCallback(StateTypeE::CONFIRM, "Mouse_Move", &ConfirmState::HighlightShip, this);
	evMgr->AddCallback(StateTypeE::CONFIRM, "Mouse_Left", &ConfirmState::MouseClick, this);
	evMgr->AddCallback(StateTypeE::CONFIRM, "KeyLeft", &ConfirmState::KeyLeft, this);
	evMgr->AddCallback(StateTypeE::CONFIRM, "KeyRight", &ConfirmState::KeyRight, this);
}
void ConfirmState::onDestroy()
{
	EventManager* evMgr = this->stateManager->getContext()->eventManager;
	evMgr->RemoveCallback(StateTypeE::CONFIRM, "EnterKey");
	evMgr->RemoveCallback(StateTypeE::CONFIRM, "KeyEscape");
	evMgr->RemoveCallback(StateTypeE::CONFIRM, "Mouse_Move");
	evMgr->RemoveCallback(StateTypeE::CONFIRM, "Mouse_Left");
	evMgr->RemoveCallback(StateTypeE::CONFIRM, "KeyLeft");
	evMgr->RemoveCallback(StateTypeE::CONFIRM, "KeyRight");
}

void ConfirmState::loadFromJson()
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

	this->jsonText["IP"] = doc["IP"].GetString();
	this->jsonText["Port"] = doc["Port"].GetString();
	this->jsonText["PlayerName"] = doc["PlayerName"].GetString();
	this->jsonText["ShipType"] = doc["ShipType"].GetString();

	fclose(fp);
}

void ConfirmState::draw()
{
	sf::RenderWindow *wind = this->stateManager->getContext()->window->getRenderWindow();
	wind->draw(this->enterText[0]);
	wind->draw(this->enterText[1]);
	for (auto it : this->jsonInfo)
	{
		wind->draw(it.second);
	}
	for (int i = 0; i < 3; ++i) {
		wind->draw(this->rects[i]);
		wind->draw(this->confText[i]);
	}
}

void ConfirmState::update(const sf::Time &time)
{

}

void ConfirmState::activate()
{

}

void ConfirmState::deactivate()
{

}

//Event Manager
void ConfirmState::backToMenu(EventDetails* details)
{
	this->stateManager->switchTo(StateTypeE::MENU);
}

void ConfirmState::PressEnter(EventDetails* details)
{
	if (this->currentButton == 0)
	{
		this->stateManager->switchTo(StateTypeE::GAME);
	}
	else if (this->currentButton == 1)
	{
		this->stateManager->switchTo(StateTypeE::SERVER_DATA);
	}
	else if (this->currentButton == 2)
	{
		this->stateManager->switchTo(StateTypeE::MENU);
	}
}

void ConfirmState::MouseClick(EventDetails* details)
{
	sf::Vector2i mousePos = details->mouse;

	for (unsigned int i = 0; i < 3; ++i)
	{
		sf::Vector2f pos = this->rects[i].getPosition();
		sf::Vector2f halfSize(this->rects[i].getSize().x / 2.0f, this->rects[i].getSize().y / 2.0f);

		if (mousePos.x >= pos.x - halfSize.x &&
			mousePos.x <= pos.x + halfSize.x &&
			mousePos.y >= pos.y - halfSize.y &&
			mousePos.y <= pos.y + halfSize.y)
		{
			if (i == 0)
			{
				this->stateManager->switchTo(StateTypeE::GAME);
			}
			else if (i == 1)
			{
				this->stateManager->switchTo(StateTypeE::SERVER_DATA);
			}
			else if (i == 2)
			{
				this->stateManager->switchTo(StateTypeE::MENU);
			}
		}
	}
	
}

void ConfirmState::HighlightShip(EventDetails* details)
{
	sf::Vector2i mousePos = this->stateManager->getContext()->eventManager->GetMousePos(this->stateManager->getContext()->window->getRenderWindow());

	for (unsigned int i = 0; i < 3; ++i)
	{
		sf::Vector2f pos = this->rects[i].getPosition();
		sf::Vector2f halfSize(this->rects[i].getSize().x / 2.0f, this->rects[i].getSize().y / 2.0f);

		if (mousePos.x >= pos.x - halfSize.x &&
			mousePos.x <= pos.x + halfSize.x &&
			mousePos.y >= pos.y - halfSize.y &&
			mousePos.y <= pos.y + halfSize.y)
		{
			this->rects[i].setFillColor(sf::Color(130, 80, 40));
		}
		else
		{
			this->rects[i].setFillColor(sf::Color(160, 100, 35));
		}
	}
}

void ConfirmState::KeyLeft(EventDetails* details)
{
	this->currentButton--;
	if (this->currentButton < 0)
	{
		this->currentButton = 2;
	}

	for (unsigned int i = 0; i < 3; ++i)
	{
		if (this->currentButton == i)
		{
			this->rects[i].setFillColor(sf::Color(130, 80, 40));
		}
		else
		{
			this->rects[i].setFillColor(sf::Color(160, 100, 35));
		}
	}
}

void ConfirmState::KeyRight(EventDetails* details)
{
	this->currentButton++;
	if (this->currentButton > 2)
	{
		this->currentButton = 0;
	}

	for (unsigned int i = 0; i < 3; ++i)
	{
		if (this->currentButton == i)
		{
			this->rects[i].setFillColor(sf::Color(130, 80, 40));
		}
		else
		{
			this->rects[i].setFillColor(sf::Color(160, 100, 35));
		}
	}
}
