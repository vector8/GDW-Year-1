#pragma once
#include "Entity.h"
#include "ICollidable.h"

#define GRENADE_FUSE_TIMER 3000
#define GRENADE_EXPLOSION_RADIUS 100

class Grenade : public Entity, public ICollidable
{
private:
	float worldX, worldY;
	double fuse;
	int range, layerID;
	double dx, dy;
	float distanceTravelled;
	bool visible, finished, fromPlayer;
	int damage;
	int currentRadius;

public:
	Grenade(float posX, float posY, double dx, double dy, int range, int layerID, bool fromPlayer, int damage);
	~Grenade();

	void update(double dt);
	void draw();

	std::pair<float, float> getWorldCoords();

	float getWidth();
	float getHeight();

	int getLayerID();

	void setVisible(bool v);

	bool isFinished();
	bool isVisible();
	bool isFromPlayer();
};