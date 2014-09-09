#pragma once

// Interface for all object that can be interacted with.
class I_Interactable
{
public:
	virtual void interact() = 0;

	virtual std::pair<float, float> getWorldCoords() = 0;

	virtual float getWidth() = 0; 
	virtual float getHeight() = 0; 
};