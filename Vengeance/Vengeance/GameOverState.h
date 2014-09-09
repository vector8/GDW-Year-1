#pragma once

#include "State.h"
#include "Button.h"
#include "StaticBackground.h"
#include "IButtonListener.h"

class GameOverState : public State, public IButtonListener
{
private:
	StaticBackground* background;

	Button *mainMenuBtn;

	void initialize();

public:
	GameOverState();
	~GameOverState();

	void draw();
	void update(double dt);

	void buttonClicked(ButtonEvent* be);

};