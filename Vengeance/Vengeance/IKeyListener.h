#pragma once

#include "KeyEvent.h"

class IKeyListener
{
public:
	virtual bool acceptingKeyEvents() = 0;
	virtual void keyEventOccurred(KeyEvent* ke) = 0;
};