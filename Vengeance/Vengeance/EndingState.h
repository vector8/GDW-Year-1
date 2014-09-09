#pragma once

#include "State.h"
#include "Button.h"
#include "StaticBackground.h"
#include "IButtonListener.h"

class EndingState : public State, public IButtonListener
{
private:
	StaticBackground* background;

	Button *burnItBtn, *takeItBtn, *continueBtn;

	void initialize();

public:
	EndingState();
	~EndingState();

	void draw();
	void update(double dt);

	void buttonClicked(ButtonEvent* be);

};