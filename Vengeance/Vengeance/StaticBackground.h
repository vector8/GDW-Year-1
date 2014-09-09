#pragma once

#include "Sprite.h"

class StaticBackground
{
private:
	Sprite* bgSprite;

public:
	StaticBackground(std::string fileName, int width, int height);
	~StaticBackground();

	void draw();
};