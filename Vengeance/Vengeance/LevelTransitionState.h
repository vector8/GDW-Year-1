#pragma once

#include "State.h"
#include "Button.h"
#include "StaticBackground.h"
#include <vector>
#include "IButtonListener.h"
#include "Level.h"

class LevelTransitionState : public State, public IButtonListener
{
private:
	int time, kills, shots, damage;

	std::string levelFileName;

	StaticBackground* background;

	Button* continueBtn;

	Sprite *levelNameSprite, *completeSprite, *timeSprite, *killsSprite, *shotsSprite, *damageTakenSprite;

	void initialize();

public:
	LevelTransitionState(Level* l);
	~LevelTransitionState();

	void draw();
	void update(double dt);

	void buttonClicked(ButtonEvent* be);
};