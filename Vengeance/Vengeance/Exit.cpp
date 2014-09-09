#include "Exit.h"
#include "MiscGameResources.h"

Exit::Exit(std::string dest, float worldX, float worldY) : destination(dest), worldX(worldX), worldY(worldY)
{
	this->setCollisionBounds(this->worldX, this->worldY, TILE_SIZE, TILE_SIZE);
}

Exit::~Exit()
{

}

std::string Exit::getDestination()
{
	return this->destination;
}