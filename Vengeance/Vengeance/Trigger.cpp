#include "Trigger.h"
#include <assert.h>
#include "MiscGameResources.h"

Trigger::Trigger(TriggerType type, std::string fileName, int timeOrWorldX, int worldY, int width, int height) : type(type), fileName(fileName), triggered(false)
{
	switch(type)
	{
	case TIME:
		this->setTime(timeOrWorldX);
		break;
	case LOCATION:
		this->setLocation((float) timeOrWorldX, (float) worldY);
		this->width = width;
		this->height = height;
		this->setCollisionBounds(this->worldX, this->worldY, this->width * TILE_SIZE, this->height * TILE_SIZE);
		break;
	default:
		assert(false);
	}
}

Trigger::~Trigger()
{

}

void Trigger::setTime(int time)
{
	assert(this->isTimeType());

	this->time = time;
}
	
void Trigger::setLocation(float worldX, float worldY)
{
	assert(this->isLocationType());

	this->worldX = worldX;
	this->worldY = worldY;
}

TriggerType Trigger::getType()
{
	return this->type;
}

std::string Trigger::getFileName()
{
	return this->fileName;
}
	
int Trigger::getTime()
{
	assert(this->isTimeType());

	return this->time;
}

float Trigger::getLocationX()
{
	assert(this->isLocationType());

	return this->worldX;
}

float Trigger::getLocationY()
{
	assert(this->isLocationType());

	return this->worldY;
}

bool Trigger::isTimeType()
{
	return this->type == TIME;
}

bool Trigger::isLocationType()
{
	return this->type == LOCATION;
}

bool Trigger::hasTriggered()
{
	return this->triggered;
}

void Trigger::setTriggered(bool b)
{
	this->triggered = b;
}