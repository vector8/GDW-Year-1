#pragma once

#include "MouseClickEvent.h"

class IMouseListener
{
public:
	virtual void mouseClicked(MouseClickEvent* mce) = 0;
};