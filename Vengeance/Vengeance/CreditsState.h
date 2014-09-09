#pragma once

#include "State.h"
#include "Sprite.h"
#include "IKeyListener.h"

class CreditsState : public State, public IKeyListener
{
private:
	double initialTimer;
	bool active;

	Sprite *creditsSprite;

public:
	CreditsState();
	~CreditsState();

	void draw();
	void update(double dt);

	void reset();

	bool acceptingKeyEvents();
	void keyEventOccurred(KeyEvent* ke);
};