#pragma once

#include <string>

class MouseClickEvent
{
private:
	int button, clickX, clickY;

public:
	MouseClickEvent(int button, int clickX, int clickY);
	~MouseClickEvent();

	int getButton();
	int getClickX();
	int getClickY();
};