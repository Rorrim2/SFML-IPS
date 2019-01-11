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
	menuText.setPosition(windowSize.x / 2.0f, windowSize.y * 0.2);

	buttonSize = sf::Vector2f(500.0f, 40.0f);
	buttonPos = sf::Vector2f(windowSize.x / 2.0f, windowSize.y * 0.5);
	buttonPadding = 4; //4px

	std::string str[3];
	str[0] = "PLAY";
	str[1] = "HOW TO PLAY";
	str[2] = "EXIT";

	rectIndex = 1;

	for (int i = 0; i < 3; ++i) 
	{
		sf::Vector2f buttonPosition(buttonPos.x, buttonPos.y + (i * (buttonSize.y + buttonPadding)));
		rects[i].setSize(buttonSize);
		if (i == rectIndex) {
			this->rects[this->rectIndex].setFillColor(sf::Color(160, 100, 35));
		}
		else {
			rects[i].setFillColor(sf::Color(130, 80, 40));
		}
		
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
	evMgr->AddCallback(StateTypeE::MENU, "KeyUp", &MenuState::KeyUp, this);
	evMgr->AddCallback(StateTypeE::MENU, "KeyDown", &MenuState::KeyDown, this);
   evMgr->AddCallback(StateTypeE::MENU, "EnterKey", &MenuState::PressEnter, this);
   evMgr->AddCallback(StateTypeE::MENU, "Mouse_Move", &MenuState::MouseEnteredOnOption, this);
}

void MenuState::onDestroy()
{
	EventManager* evMgr = stateManager->getContext()->eventManager;
	evMgr->RemoveCallback(StateTypeE::MENU, "Mouse_Left");
	evMgr->RemoveCallback(StateTypeE::MENU, "KeyUp");
	evMgr->RemoveCallback(StateTypeE::MENU, "KeyDown");
	evMgr->RemoveCallback(StateTypeE::MENU, "EnterKey");
   evMgr->RemoveCallback(StateTypeE::MENU, "Mouse_Move");
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
	if (this->stateManager->HasState(StateTypeE::SERVER_DATA) && this->labels[0].getString() == "PLAY") {
		this->labels[0].setString(sf::String("RESUME"));
		sf::FloatRect rect = this->labels[0].getLocalBounds();
		this->labels[0].setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f); 
		// has to do that because sf::drawable set diffrent object dimension
	}
}

void MenuState::deactivate()
{
}

void MenuState::KeyUp(EventDetails* details)
{
	if (rectIndex > 0)
	{
		this->rects[this->rectIndex].setFillColor(sf::Color(130, 80, 40));
		rectIndex--;
		this->rects[this->rectIndex].setFillColor(sf::Color(150, 100, 40));
	}
}

void MenuState::KeyDown(EventDetails* details)
{
	if (rectIndex < 2)
	{
		this->rects[this->rectIndex].setFillColor(sf::Color(130, 80, 40));
		rectIndex++;
		this->rects[this->rectIndex].setFillColor(sf::Color(150, 100, 40));
	}
}

void MenuState::PressEnter(EventDetails* details)
{
	if (this->rectIndex == 0)
	{
		this->stateManager->switchTo(StateTypeE::SERVER_DATA);
	}
	else if (this->rectIndex == 1)
	{
		this->stateManager->switchTo(StateTypeE::HOW_TO_PLAY);
	}
	else if (this->rectIndex == 2)
	{
		this->stateManager->getContext()->window->close();
	}
}

void MenuState::MouseEnteredOnOption(EventDetails* details)
{
   sf::Vector2i mousePos = this->stateManager->getContext()->eventManager->GetMousePos(this->stateManager->getContext()->window->getRenderWindow());
   float halfX = this->buttonSize.x / 2.0f;
   float halfY = this->buttonSize.y / 2.0f;

   for (int i = 0; i < 3; ++i)
   {
      if (mousePos.x >= this->rects[i].getPosition().x - halfX &&
         mousePos.x <= this->rects[i].getPosition().x + halfX &&
         mousePos.y >= this->rects[i].getPosition().y - halfY &&
         mousePos.y <= this->rects[i].getPosition().y + halfY)
      {
		  this->rects[i].setFillColor(sf::Color(150, 100, 40));
      }
      else
      {
         rects[i].setFillColor(sf::Color(130, 80, 40));
      }
   }
}

void MenuState::MouseClick(EventDetails* details)
{
	sf::Vector2i mousePos = details->mouse;
	float halfX = this->buttonSize.x / 2.0f;
	float halfY = this->buttonSize.y / 2.0f;
	for (int i = 0; i < 3; ++i) 
	{
		if (mousePos.x >= this->rects[i].getPosition().x - halfX &&
			mousePos.x <= this->rects[i].getPosition().x + halfX &&
			mousePos.y >= this->rects[i].getPosition().y - halfY &&
			mousePos.y <= this->rects[i].getPosition().y + halfY)
		{
			
			if (i == 0)
			{
				this->stateManager->switchTo(StateTypeE::SERVER_DATA);
			}
			else if (i == 1)
			{
				this->stateManager->switchTo(StateTypeE::HOW_TO_PLAY);
			}
			else if (i == 2)
			{
				this->stateManager->getContext()->window->close();
			}
		}
	}
}