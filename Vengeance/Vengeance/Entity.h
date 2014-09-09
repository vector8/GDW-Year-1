#pragma once
#include "MiscGameResources.h"
#include "Sprite.h"

class Entity
{
public:
	virtual ~Entity() {};

	virtual void update(double dt) = 0;
	virtual void draw() = 0;

	virtual std::pair<float, float> getWorldCoords() = 0;

	virtual float getWidth() = 0;
	virtual float getHeight() = 0;

	virtual int getLayerID() = 0;

	virtual bool isFinished() = 0;
	virtual bool isVisible() = 0;
};