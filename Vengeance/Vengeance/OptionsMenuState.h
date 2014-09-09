#pragma once

#include "State.h"
#include "Button.h"
#include "StaticBackground.h"
#include <vector>
#include "IButtonListener.h"

// State that represents the game's options menu.
class OptionsMenuState : public State, public IButtonListener
{
private:
	StaticBackground* background;

	std::vector<Button*> buttons;

	void initialize();

public:
	OptionsMenuState();
	~OptionsMenuState();

	void draw();
	void update(double dt);

	void buttonClicked(ButtonEvent* be);
};