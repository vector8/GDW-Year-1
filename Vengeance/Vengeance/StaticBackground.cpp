#include "StaticBackground.h"

StaticBackground::StaticBackground(std::string fileName, int width, int height)
{
	this->bgSprite = new Sprite(fileName);
	this->bgSprite->setNumberOfAnimations(1);
	this->bgSprite->setSpriteFrameSize(width, height);
	this->bgSprite->setPosition(0, 0);
	this->bgSprite->setLayerID(0);
	this->bgSprite->addSpriteAnimFrame(0, 0, 0);
}

StaticBackground::~StaticBackground()
{
	delete this->bgSprite;
	this->bgSprite = NULL;
}

void StaticBackground::draw()
{
	this->bgSprite->draw();
}