#include "Window.h"

Window::Window()
{
   Window("Window", sf::Vector2u(640, 480));
   this->isSetDataState = false;
}

Window::Window(const std::string &title, const sf::Vector2u &size)
   : windowTitle(title), windowSize(size), fullscreen(false), done(false)
{
   create();
   setup(title, size);
}

void Window::create()
{
   auto style = this->isFullscreen() ? sf::Style::Fullscreen : sf::Style::Default;
   sf::ContextSettings settings;
   settings.antialiasingLevel = 4;
   this->window.create({ this->windowSize.x, this->windowSize.y, 32 }, this->windowTitle, style, settings);
   this->window.setVerticalSyncEnabled(true);
}

void Window::destroy()
{
   this->window.close();
}


Window::~Window()
{
   destroy();
}

void Window::close(EventDetails* details)
{
   this->done = true;
}


void Window::setup(const std::string title, const sf::Vector2u& size) {
	this->windowTitle = title;
	this->windowSize = size;
	this->fullscreen = false;
	this->done = false;
	this->isFocused = true;
	this->eventManager.AddCallback(StateTypeE(0),"Fullscreen_toggle", &Window::toggleFullscreen, this);
	this->eventManager.AddCallback(StateTypeE(0), "Window_close", &Window::close, this);
	this->create();
	this->counter = 0;
	this->itsTimeToSwap = 0;

	this->Inputs[this->counter].push_back('_');
	
	this->shipNames[ShipType::BLUE] = "TheBlueOne";
	this->shipNames[ShipType::RED] = "TheRedOne";
	this->shipNames[ShipType::GREEN] = "TheGreenOne";
	this->shipNames[ShipType::YELLOW] = "TheYellowOne";

	this->whichOne = ShipType::RED;
}

void Window::beginDraw()
{
   this->window.clear(sf::Color::Black);
}

void Window::endDraw()
{
   this->window.display();
}

//expanded by things from EventManager
void Window::update()
{
	this->itsTimeToSwap = (this->itsTimeToSwap++)%10;
   sf::Event event;
   while (this->window.pollEvent(event))
   {
	  if (event.type == sf::Event::LostFocus) {
		  this->isFocused = false;
		  this->eventManager.SetFocus(false);
	  }
	  else if (event.type == sf::Event::GainedFocus) {
		  this->isFocused = true;
		  this->eventManager.SetFocus(true);
	  }
	  else if (event.type == sf::Event::TextEntered) {
		  if (this->isSetDataState) {
			  if(this->counter != 3)
			  {
				  if (this->counter == 2)
				  {
					  if (event.text.unicode <= '~'  && event.text.unicode >= ' ')
					  {
						  this->Inputs[this->counter].insert(this->Inputs[this->counter].length() - 1, 1, (char)event.text.unicode);
					  }
				  }
				  else
				  {
					  if ((event.text.unicode <= '9'  && event.text.unicode >= '0') || event.text.unicode == '.')
					  {
						  this->Inputs[this->counter].insert(this->Inputs[this->counter].length() - 1, 1, (char)event.text.unicode);
					  }
				  }
			  }
		  }	  
	  }
	 // if(event.key.code == sf::Keyboard::F5 || event.key.code == sf::Keyboard::Escape) std::cout << event.type << std::endl;
	  this->eventManager.HandleEvent(event);
   }
   this->SwapTheSign();
   this->eventManager.Update();
}

bool Window::isDone()
{
   return this->done;
}

bool Window::isFullscreen()
{
   return this->fullscreen;
}

void Window::toggleFullscreen(EventDetails* details)
{
   this->fullscreen = !this->fullscreen;
   destroy();
   create();
}
//setdatastate
void Window::SetIsDataStateFlag()
{
	this->isSetDataState = !this->isSetDataState;
}

bool Window::GetIsDataStateFlag() 
{
	return this->isSetDataState;
}

unsigned int Window::getCounter()
{
	return this->counter;
}

//current modyfing string
std::string Window::getInput()
{
	return this->Inputs[this->counter];
}

std::string Window::getInput(unsigned int i)
{
	return this->Inputs[i];
}

//going to the next string + annihilating text cursor
void Window::incrementCounter()
{
	if (this->counter != 3)
	{
		this->Inputs[this->counter].erase(this->Inputs[this->counter].length() - 1, 1);
	}
	this->counter++;
	if (this->counter != 3)
	{
		this->Inputs[this->counter].push_back('_');
	}
}

//deletes letters from current modyfing string
void Window::deleteChar()
{
	if (this->getInput().length() != 1) {
		this->Inputs[this->counter].erase(this->Inputs[this->counter].length() - 1, 1);
	}
}

void Window::SetShip(ShipType shipType) {
	if (this->counter == 3)
	{
		this->Inputs[this->counter] = this->shipNames[shipType];
		this->whichOne = shipType;
	}
}


//change the sign of text cursor 
void Window::SwapTheSign()
{
	if (this->itsTimeToSwap % 10 == 0 && this->counter != 3)
	{
		if (this->Inputs[this->counter][this->Inputs[this->counter].length() - 1] == '_')
		{
			this->Inputs[this->counter][this->Inputs[this->counter].length() - 1] = ' ';
		}
		else
		{
			this->Inputs[this->counter][this->Inputs[this->counter].length() - 1] = '_';
		}
		
	}
}
//end things with setdatastate


sf::Vector2u Window::getWindowSize()
{
   return sf::Vector2u(this->windowSize);
}

EventManager* Window::getEventManager() {
	return &this->eventManager;
}
sf::RenderWindow* Window::getRenderWindow() {
	return &this->window;
}

void Window::draw(sf::Drawable & drawable)
{
   this->window.draw(drawable);
}

void Window::draw(sf::Vertex *verticles, size_t count, sf::PrimitiveType type)
{
   this->window.draw(verticles, count, type);
}

