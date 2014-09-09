#pragma once

#include <string>

class Button;

class ButtonEvent
{
private:
	Button* source;

public:
	ButtonEvent(Button* source);
	~ButtonEvent();

	Button* getSource()
	{
		return source;
	}

	std::string getCommand();
};