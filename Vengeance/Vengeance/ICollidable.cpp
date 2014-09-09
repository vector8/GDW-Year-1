#include "ICollidable.h"

BoundingBox ICollidable::getCollisionBounds()
{
	return this->bounds;
}

void ICollidable::setCollisionOrigin(float x, float y)
{
	this->bounds.originX = x;
	this->bounds.originY = y;
}

void ICollidable::setCollisionBounds(float originX, float originY, int width, int height)
{
	this->bounds.originX = originX;
	this->bounds.originY = originY;
	this->bounds.width = width;
	this->bounds.height = height;
}

bool ICollidable::hasCollided(ICollidable* c)
{
	// check if any of c's corners are contained in this, 
	return this->containsPoint(c->getCollisionBounds().originX, c->getCollisionBounds().originY) ||
		this->containsPoint(c->getCollisionBounds().originX + c->getCollisionBounds().width, c->getCollisionBounds().originY) ||
		this->containsPoint(c->getCollisionBounds().originX, c->getCollisionBounds().originY + c->getCollisionBounds().height) ||
		this->containsPoint(c->getCollisionBounds().originX + c->getCollisionBounds().width, c->getCollisionBounds().originY + c->getCollisionBounds().height) ||
	// or if any of this' corners are contained in c
		c->containsPoint(this->getCollisionBounds().originX, this->getCollisionBounds().originY) ||
		c->containsPoint(this->getCollisionBounds().originX + this->getCollisionBounds().width, this->getCollisionBounds().originY) ||
		c->containsPoint(this->getCollisionBounds().originX, this->getCollisionBounds().originY + this->getCollisionBounds().height) ||
		c->containsPoint(this->getCollisionBounds().originX + this->getCollisionBounds().width, this->getCollisionBounds().originY + this->getCollisionBounds().height);
}

bool ICollidable::containsPoint(float x, float y)
{
	return this->getCollisionBounds().originX <= x && (this->getCollisionBounds().originX + this->getCollisionBounds().width) >= x &&
		this->getCollisionBounds().originY <= y && (this->getCollisionBounds().originY + this->getCollisionBounds().height) >= y;
}