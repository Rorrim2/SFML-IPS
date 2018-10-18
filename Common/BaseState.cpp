#include "BaseState.h"	



BaseState::BaseState(StateManager *stateManager)
{
	this->stateManager = stateManager;
	this->transdencent = false;
	this->transparent = false;
}


BaseState::~BaseState()
{
}

void BaseState::setTransparent(bool enable)
{
	this->transparent = enable;
}

void BaseState::setTransdencent(bool enable)
{
	this->transdencent = enable;
}

bool BaseState::getTransparent()
{
	return this->transparent;
}

bool BaseState::getTransdencent()
{
	return this->transdencent;
}
