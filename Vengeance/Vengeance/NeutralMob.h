#pragma once
#include "Entity.h"
#include "ICollidable.h"
#include "I_Interactable.h"

class NeutralMob : public Entity, public ICollidable, public I_Interactable
{
private:
	float worldX, worldY;
	std::string dialogueFilename;

	Sprite* sprite;

public:
	NeutralMob(std::string spriteSheetName, float posX, float posY, std::string dialogueFilename = "");
	~NeutralMob();

	std::pair<float, float> getWorldCoords();

	void update(double dt);
	void draw();

	void interact();

	float getWidth();
	float getHeight();

	int getLayerID();

	bool isFinished();
	bool isVisible();
};