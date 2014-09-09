#pragma once

#include "State.h"
#include "IKeyListener.h"
#include "Sprite.h"

#define SPLASH_FLIP_TIME 2000

// State that represents the game's main menu.
class SplashScreenState : public State, public IKeyListener
{
private:
	Sprite* splashSprite;

	int timer;

public:
	SplashScreenState();
	~SplashScreenState();

	void draw();
	void update(double dt);

	bool acceptingKeyEvents();
	void keyEventOccurred(KeyEvent* ke);
};