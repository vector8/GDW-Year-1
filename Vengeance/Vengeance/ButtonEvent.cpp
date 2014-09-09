#include "ButtonEvent.h"
#include "Button.h"

ButtonEvent::ButtonEvent(Button* source) : source(source)
{

}

ButtonEvent::~ButtonEvent()
{
	delete this->source;
	this->source = NULL;
}

std::string ButtonEvent::getCommand()
{
	return this->source->getName();
}