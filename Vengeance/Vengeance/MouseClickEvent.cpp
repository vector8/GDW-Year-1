#include "MouseClickEvent.h"

MouseClickEvent::MouseClickEvent(int button, int clickX, int clickY) : button(button), clickX(clickX), clickY(clickY)
{}

MouseClickEvent::~MouseClickEvent()
{}

int MouseClickEvent::getButton()
{
	return this->button;
}

int MouseClickEvent::getClickX()
{
	return this->clickX;
}

int MouseClickEvent::getClickY()
{
	return this->clickY;
}