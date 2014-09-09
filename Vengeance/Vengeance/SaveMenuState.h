#pragma once

#include "State.h"
#include "Button.h"
#include "StaticBackground.h"
#include <vector>
#include "IButtonListener.h"

class SaveMenuState : public State, public IButtonListener
{
private:
	StaticBackground* background;

	int saveButtonPressed;
	bool needPrompt;

	std::vector<Button*> buttons;

	void initialize();

public:
	SaveMenuState();
	~SaveMenuState();

	void draw();
	void update(double dt);

	void buttonClicked(ButtonEvent* be);
};