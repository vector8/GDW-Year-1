#include "Doodad.h"
#include "StateManager.h"

Doodad::Doodad(std::string spriteSheetName, float posX, float posY, int width, int height, bool collidable, std::string dialogueFilename) : worldX(posX), worldY(posY), collidable(collidable), dialogueFilename(dialogueFilename)
{
	this->sprite = new Sprite(spriteSheetName);

	this->sprite->initialize(1, width * DOODAD_SIZE, height * DOODAD_SIZE, posX, posY, 2);

	this->sprite->addSpriteAnimFrame(0, 0, 0);
}

Doodad::~Doodad()
{
	delete this->sprite;
}

std::pair<float, float> Doodad::getWorldCoords()
{
	return std::pair<float, float>(this->worldX, this->worldY);
}

void Doodad::update(double dt)
{
	std::pair<float, float> screenCoords = StateManager::getCurrentLevel()->convertWorldCoordsToScreenCoords(this->getWorldCoords());
	this->sprite->setPosition(screenCoords.first, screenCoords.second);
}

void Doodad::draw()
{
	this->sprite->draw();
}

void Doodad::interact()
{
	if(this->dialogueFilename.size() > 0)
		StateManager::getInstance()->getGameplayState()->setDialogue(this->dialogueFilename);
}

float Doodad::getWidth()
{
	return this->sprite->getWidth();
}

float Doodad::getHeight()
{
	return this->sprite->getHeight();
}

int Doodad::getLayerID()
{
	return this->sprite->layerID;
}

bool Doodad::isCollidable()
{
	return this->collidable;
}

bool Doodad::isFinished()
{
	return false;
}

bool Doodad::isVisible()
{
	return true;
}