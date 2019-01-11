#include "SetDataState.h"

SetDataState::SetDataState(StateManager* stateManager) : BaseState(stateManager)
{
	this->text.push_back(sf::String("Enter Server IP: "));
	this->text.push_back(sf::String("Enter Server Port: "));
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
	this->enterText.setPosition(windowSize.x * 0.25, windowSize.y * 0.5);

	for (unsigned int i = 0; i < this->text.size(); ++i) {
		this->infoText[i] = sf::Text(this->text[i], font, 22);
		this->infoText[i].setPosition(windowSize.x * 0.1, windowSize.y * (i * 0.1 + 0.10));

		this->Outputs[i] = sf::Text();
		this->Outputs[i].setFont(font);
		this->Outputs[i].setCharacterSize(22);

		this->Outputs[i].setPosition(windowSize.x * 0.1, windowSize.y * (i * 0.1 + 0.15));
	}

	EventManager* evMgr = this->stateManager->getContext()->eventManager;
	evMgr->AddCallback(StateTypeE::SERVER_DATA, "EnterKey", &SetDataState::PressEnter, this);
	evMgr->AddCallback(StateTypeE::SERVER_DATA, "KeyEscape", &SetDataState::backToMenu, this);
	evMgr->AddCallback(StateTypeE::SERVER_DATA, "BackSpace", &SetDataState::PressBackSpace, this);
}

void SetDataState::onDestroy()
{
	EventManager* evMgr = this->stateManager->getContext()->eventManager;
	evMgr->RemoveCallback(StateTypeE::SERVER_DATA, "EnterKey");
	evMgr->RemoveCallback(StateTypeE::SERVER_DATA, "KeyEscape");
	evMgr->RemoveCallback(StateTypeE::SERVER_DATA, "BackSpace");

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

void SetDataState::activate()
{

}

void SetDataState::deactivate()
{

}

SetDataState::~SetDataState()
{
}
