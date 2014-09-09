#pragma once

#include "ICollidable.h"
#include <vector>

enum CollisionType
{
	FULL = 1,
	CRAWL_THROUGH = 2
};

class StaticCollidable : public ICollidable
{
private:
	CollisionType type;

	static std::vector<std::vector<StaticCollidable*>> templateCollisionGroups;

	static void parseCollisionGroups();
	static int getCollisionGroupFromTileTypeCharacter(char tileType);
public:
	StaticCollidable(CollisionType type, float worldX, float worldY, int width, int height);

	static std::vector<StaticCollidable*> createCollidables(char tileType, float worldX, float worldY);

	float getX();
	float getY();
	
	int getWidth();
	int getHeight();

	CollisionType getType();
};