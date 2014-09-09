#pragma once

#include "State.h"
#include "Button.h"
#include "StaticBackground.h"
#include <vector>
#include "IButtonListener.h"

class LoadMenuState : public State, public IButtonListener
{
private:
	StaticBackground* background;

	int saveButtonPressed;
	bool needPrompt;

	std::vector<Button*> buttons;

	void initialize();

public:
	LoadMenuState();
	~LoadMenuState();

	void draw();
	void update(double dt);

	void buttonClicked(ButtonEvent* be);
};