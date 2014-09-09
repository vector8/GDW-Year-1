#pragma once

#include "ICollidable.h"
#include <string>

class Exit : public ICollidable
{
private:
	float worldX, worldY;

	std::string destination;

public:
	Exit(std::string dest, float worldX, float worldY);
	~Exit();

	std::string getDestination();
};