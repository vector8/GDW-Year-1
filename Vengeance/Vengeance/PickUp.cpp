#include "PickUp.h"
#include "StateManager.h"
#include <assert.h>

PickUp::PickUp(PickUpTypes type, float worldX, float worldY) : type(type), worldX(worldX), worldY(worldY), visible(true), finished(false)
{
	std::string spriteFileName = "images/";

	switch(type)
	{
	case PISTOL:
		spriteFileName += "Pistol.png";
		break;
	case RIFLE:
		spriteFileName += "Rifle.png";
		break;
	case PISTOL_AMMO:
		spriteFileName += "PistolAmmoBox.png";
		break;
	case RIFLE_AMMO:
		spriteFileName += "RifleAmmoBox.png";
		break;
	case GRENADE:
		spriteFileName += "Grenade.png";
		break;
	case HAMBURGER:
		spriteFileName += "Burger.png";
		break;
	default:
		spriteFileName += "BrokenImage.png";
		break;
	}

	this->sprite = new Sprite(spriteFileName);

	if(type == RIFLE)
		this->sprite->initialize(1, 2*DOODAD_SIZE, DOODAD_SIZE, 1000, 1000, 2);
	else
		this->sprite->initialize(1, DOODAD_SIZE, DOODAD_SIZE, 1000, 1000, 2);

	this->sprite->addSpriteAnimFrame(0, 0, 0);

	this->setCollisionBounds(this->worldX, this->worldY, DOODAD_SIZE, DOODAD_SIZE);
}

PickUp::~PickUp()
{
	delete this->sprite;
	this->sprite = NULL;
}

void PickUp::update(double dt)
{
	std::pair<float, float> screenCoords = StateManager::getCurrentLevel()->convertWorldCoordsToScreenCoords(this->getWorldCoords());
	this->sprite->setPosition(screenCoords.first, screenCoords.second);
}

void PickUp::draw()
{
	this->sprite->draw();
}

float PickUp::getWidth()
{
	return TILE_SIZE;
}

float PickUp::getHeight()
{
	return TILE_SIZE;
}

int PickUp::getLayerID()
{
	return this->sprite->layerID;
}

void PickUp::pickUp()
{
	bool pickedUp = false;

	switch(this->type)
	{
	case PISTOL:
		Player::getInstance()->pickUpPistol(true);
		SoundManager::getInstance()->playSoundEffect(PISTOL_PICKUP);
		pickedUp = true;
		break;
	case RIFLE:
		Player::getInstance()->pickUpRifle(true);
		SoundManager::getInstance()->playSoundEffect(PISTOL_PICKUP);
		pickedUp = true;
		break;
	case PISTOL_AMMO:
		if(Player::getInstance()->getPistol()->getMagazineAmmo() + Player::getInstance()->getPistol()->getReserveAmmo() < Player::getInstance()->getPistol()->getMaxAmmo())
		{
			Player::getInstance()->getPistol()->addToReserveAmmo(Player::getInstance()->getPistol()->getMagazineCapacity());
			SoundManager::getInstance()->playSoundEffect(PISTOL_PICKUP);
			pickedUp = true;
		}
		break;
	case RIFLE_AMMO:
		if(Player::getInstance()->getRifle()->getMagazineAmmo() + Player::getInstance()->getRifle()->getReserveAmmo() < Player::getInstance()->getRifle()->getMaxAmmo())
		{
			Player::getInstance()->getRifle()->addToReserveAmmo(Player::getInstance()->getRifle()->getMagazineCapacity());
			SoundManager::getInstance()->playSoundEffect(PISTOL_PICKUP);
			pickedUp = true;
		}
		break;
	case GRENADE:
		if(Player::getInstance()->getGrenade()->getMagazineAmmo() < Player::getInstance()->getGrenade()->getMaxAmmo())
		{
			Player::getInstance()->getGrenade()->setMagazineAmmo(Player::getInstance()->getGrenade()->getMagazineAmmo() + 1);
			SoundManager::getInstance()->playSoundEffect(PISTOL_PICKUP);
			pickedUp = true;
		}
		break;
	case HAMBURGER:
		if(Player::getInstance()->getHamburger()->getMagazineAmmo() < Player::getInstance()->getHamburger()->getMaxAmmo())
		{
			Player::getInstance()->getHamburger()->setMagazineAmmo(Player::getInstance()->getHamburger()->getMagazineAmmo() + 1);
			pickedUp = true;
		}
		break;
	default:
		assert(false);
	}

	if(pickedUp)
		this->finished = true;
}

std::pair<float, float> PickUp::getWorldCoords()
{
	return std::pair<float, float>(this->worldX, this->worldY);
}

bool PickUp::isFinished()
{
	return this->finished;
}

bool PickUp::isVisible()
{
	return this->visible;
}

void PickUp::setFinished(bool b)
{
	this->finished = b;	
}