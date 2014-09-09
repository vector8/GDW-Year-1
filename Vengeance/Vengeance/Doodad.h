#pragma once
#include "Entity.h"
#include "I_Interactable.h"

// Represents passive environment elements, such as a table or mailbox.
class Doodad : public Entity, public I_Interactable
{
private:
	float worldX, worldY;
	std::string dialogueFilename;

	bool collidable;

	Sprite* sprite;

public:
	Doodad(std::string spriteSheetName, float posX, float posY, int width, int height, bool collidable, std::string dialogueFilename = "");
	~Doodad();

	std::pair<float, float> getWorldCoords();

	void update(double dt);
	void draw();

	void interact();

	float getWidth();
	float getHeight();

	int getLayerID();

	bool isCollidable();

	bool isFinished();
	bool isVisible();
};