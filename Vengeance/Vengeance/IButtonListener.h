#pragma once

#include "ButtonEvent.h"

class IButtonListener
{
public:
	virtual void buttonClicked(ButtonEvent* be) = 0;
};