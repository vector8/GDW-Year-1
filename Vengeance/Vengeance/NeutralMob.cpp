#include "NeutralMob.h"
#include "StateManager.h"

NeutralMob::NeutralMob(std::string spriteSheetName, float posX, float posY, std::string dialogueFilename) : worldX(posX), worldY(posY), dialogueFilename(dialogueFilename)
{
	this->sprite = new Sprite(spriteSheetName);

	this->sprite->setNumberOfAnimations(1);
	this->sprite->setSpriteFrameSize(SPRITE_SIZE, SPRITE_SIZE);
	this->sprite->setLayerID(2);

	this->sprite->addSpriteAnimFrame(0, 0, 0);
}

NeutralMob::~NeutralMob()
{
	delete this->sprite;
}

std::pair<float, float> NeutralMob::getWorldCoords()
{
	return std::pair<float, float>(this->worldX, this->worldY);
}

void NeutralMob::update(double dt)
{
	std::pair<float, float> screenCoords = StateManager::getCurrentLevel()->convertWorldCoordsToScreenCoords(this->getWorldCoords());
	this->sprite->setPosition(screenCoords.first, screenCoords.second);
}

void NeutralMob::draw()
{
	this->sprite->draw();
}

void NeutralMob::interact()
{
	if(this->dialogueFilename.size() > 0)
		StateManager::getInstance()->getGameplayState()->setDialogue(this->dialogueFilename);
}

float NeutralMob::getWidth()
{
	return this->sprite->getWidth();
}

float NeutralMob::getHeight()
{
	return this->sprite->getHeight();
}

int NeutralMob::getLayerID()
{
	return this->sprite->layerID;
}

bool NeutralMob::isFinished()
{
	return false;
}

bool NeutralMob::isVisible()
{
	return true;
}