#pragma once

#include "ICollidable.h"
#include <string>

enum TriggerType
{
	TIME,
	LOCATION
};

class Trigger : public ICollidable
{
private:
	int time;
	float worldX, worldY;
	int width, height;

	bool triggered;

	TriggerType type;

	std::string fileName;

public:
	Trigger(TriggerType type, std::string fileName, int timeOrWorldX, int worldY = 0, int width = 0, int height = 0);
	~Trigger();

	void setTime(int time);
	
	void setLocation(float worldX, float worldY);

	TriggerType getType();

	std::string getFileName();
	
	int getTime();

	float getLocationX();

	float getLocationY();

	bool isTimeType();

	bool isLocationType();

	bool hasTriggered();

	void setTriggered(bool b);
};