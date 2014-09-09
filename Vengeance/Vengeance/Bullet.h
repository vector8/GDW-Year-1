#pragma once
#include "Entity.h"
#include "ICollidable.h"

class Bullet : public Entity, public ICollidable
{
private:
	float worldX, worldY;
	int range, layerID;
	double dx, dy;
	float distanceTravelled;
	bool visible, finished, fromPlayer;
	int damage;

public:
	Bullet(float posX, float posY, double dx, double dy, int range, int layerID, bool fromPlayer, int damage);
	~Bullet();

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