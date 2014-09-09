#pragma once

struct BoundingBox
{
	// origin is located in bottom-right corner of box
	float originX, originY;
	int width, height;
};

class ICollidable
{
private:
	BoundingBox bounds;
public:
	BoundingBox getCollisionBounds();

	void setCollisionOrigin(float x, float y);

	void setCollisionBounds(float originX, float originY, int width, int height);

	bool hasCollided(ICollidable* c);

	// helper method for hasCollided
	bool containsPoint(float x, float y);
};