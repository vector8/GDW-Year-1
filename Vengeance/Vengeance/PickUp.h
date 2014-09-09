#pragma once

#include "Entity.h"
#include "ICollidable.h"

enum PickUpTypes
{
	PISTOL,
	RIFLE,
	PISTOL_AMMO,
	RIFLE_AMMO,
	GRENADE,
	HAMBURGER
};

class PickUp : public Entity, public ICollidable
{
private:
	float worldX, worldY;

	bool visible, finished;
	PickUpTypes type;

	Sprite* sprite;

public:
	PickUp(PickUpTypes type, float worldX, float worldY);
	~PickUp();

	void update(double dt);
	void draw();

	float getWidth();
	float getHeight();

	int getLayerID();

	void pickUp();

	std::pair<float, float> PickUp::getWorldCoords();

	bool isFinished();
	bool isVisible();

	void setFinished(bool b);
};