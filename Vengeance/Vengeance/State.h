#pragma once

#include "MiscGameResources.h"

// Abstract class that represents base class for all states in the game.
class State
{
public:
	virtual void draw() = 0;
	virtual void update(double dt) = 0;
};
