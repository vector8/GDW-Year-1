#pragma once

#include "State.h"
#include "Button.h"
#include "StaticBackground.h"
#include <vector>
#include "IButtonListener.h"

// State that represents the game's main menu.
class MainMenuState : public State, public IButtonListener
{
private:
	StaticBackground* background;

	Button *continueBtn, *newBtn, *loadSaveBtn, *loadBtn, *optionsBtn, *creditsBtn, *quitBtn;

	std::vector<Button*> buttons;

	bool quitPressed;

	void initialize();

public:
	MainMenuState();
	~MainMenuState();

	void draw();
	void update(double dt);

	void buttonClicked(ButtonEvent* be);
};