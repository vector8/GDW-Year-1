#include "Player.h"
#include "Sprite.h"
#include "StateManager.h"
#include "Bullet.h"
#include "Exit.h"
#include "Trigger.h"
#include "SoundManager.h"
#include <algorithm>
#include "InputManager.h"
#include "PickUp.h"
#include <assert.h>
#include "Grenade.h"

Player::Player() : interactUp(true), reloadUp(true)
{
	this->initializePlayer();
}

Player::~Player()
{
	delete this->sprite;
	this->sprite = NULL;
}

void Player::initializePlayer()
{
	this->score = 0;
	this->maxHealth = 100;
	this->health = 100;

	this->worldX = 0;
	this->worldY = 0;

	this->sprinting = false;
	this->speed = 100;
	this->energy = 100;

	this->direction = FRONT;
	this->passiveAnimation = FRONT_IDLE;
	this->activeAnimation = NUM_PLAYER_ANIMATIONS;
	this->activeAnimationTicks = 0;

	this->initializeSprite();

	this->pipe = new Weapon(true, PIPE_DAMAGE, MELEE_RANGE);

	this->pistol = new Weapon(false, PISTOL_DAMAGE, GUN_RANGE, PISTOL_MAG_CAP, PISTOL_MAX_AMMO);
	this->pistol->setMagazineAmmo(7);
	this->pistol->setReserveAmmo(21);

	this->rifle = new Weapon(false, RIFLE_DAMAGE, GUN_RANGE, RIFLE_MAG_CAP, RIFLE_MAX_AMMO);
	this->rifle->setMagazineAmmo(30);
	this->rifle->setReserveAmmo(30);

	this->grenade = new Weapon(false, GRENADE_DAMAGE, GRENADE_RANGE, GRENADE_CAP, GRENADE_CAP);
	this->grenade->setMagazineAmmo(0);
	this->grenade->setReserveAmmo(0);

	this->hamburger = new Weapon(false, HAMBURGER_HEALING, 0, HAMBURGER_CAP, HAMBURGER_CAP);
	this->hamburger->setMagazineAmmo(0);
	this->hamburger->setReserveAmmo(0);

	this->currentWeapon = this->pipe;

	this->pickedUpPistol = false;
	this->pickedUpRifle = false;

	this->visible = true;

	// initialize bounding box
	this->setCollisionBounds(this->worldX + COL_MARGIN, this->worldY + COL_MARGIN_Y, SPRITE_SIZE - (2*COL_MARGIN), SPRITE_SIZE - (COL_MARGIN));
}

void Player::initializeSprite()
{
	this->sprite = new Sprite("images/player_sheet.png");
	this->sprite->initialize(NUM_PLAYER_ANIMATIONS, SPRITE_SIZE, SPRITE_SIZE, 0, 0, 2);

	for(int i = 0; i < NUM_PLAYER_ANIMATIONS; i++)
	{
		this->sprite->addSpriteAnimRow(i, 0, 2048 - ((i + 1) * SPRITE_SIZE), SPRITE_SIZE, 0, 4);
	}
}

Player* Player::getInstance()
{
	static Player p;

	return &p;
}

// dt is in milliseconds!
void Player::update(double dt)
{
	static double frameTimer = 0;

	static double weaponTimer = 250;

	InputManager* input = InputManager::getInstance();

	frameTimer += dt;

	if(!input->isLeftMouseDown())
		weaponTimer = 250;
	else if(weaponTimer < 250)
		weaponTimer += dt;

	//////////////////
	// Keyboard Input
	//////////////////
	this->sprinting = input->isKeyDownForAction(SPRINT);
	this->prone = input->isKeyDownForAction(PRONE);
	
	if(this->sprinting)
	{
		float delta = 50 * (dt / 1000);

		if(this->energy >= delta)
		{
			this->speed = 200;
			this->energy -= delta;
		}
		else
			this->speed = 100;
	}
	else
	{
		this->speed = 100;
		this->energy += 25 * (dt / 1000);

		if(this->energy > 100)
			this->energy = 100;
	}

	if(input->isKeyDownForAction(MOVE_RIGHT) && !input->isKeyDownForAction(MOVE_LEFT))
	{
		dx = this->speed;
	}
	else if(input->isKeyDownForAction(MOVE_LEFT) && !input->isKeyDownForAction(MOVE_RIGHT))
	{
		dx = -this->speed;
	}
	else
	{
		dx = 0;
	}

	if(input->isKeyDownForAction(MOVE_UP) && !input->isKeyDownForAction(MOVE_DOWN))
	{
		dy = this->speed;
	}
	else if(input->isKeyDownForAction(MOVE_DOWN) && !input->isKeyDownForAction(MOVE_UP))
	{
		dy = -this->speed;
	}
	else
	{
		dy = 0;
	}

	if(!input->isKeyDownForAction(INTERACT))
		this->interactUp = true;

	// switch weapon based on which hotbar is selected
	int hotbar = StateManager::getInstance()->getGameplayState()->getSelectedHotbar();

	switch(hotbar)
	{
	case 1:
		this->setCurrentWeapon(this->getPipe());
		break;
	case 2:
		if(this->hasPistol())
			this->setCurrentWeapon(this->getPistol());
		else
			this->setCurrentWeapon(NULL);
		break;
	case 3:
		if(this->hasRifle())
			this->setCurrentWeapon(this->getRifle());
		else
			this->setCurrentWeapon(NULL);
		break;
	case 4:
		if(this->getGrenade()->getMagazineAmmo() > 0)
			this->setCurrentWeapon(this->getGrenade());
		else
			this->setCurrentWeapon(NULL);
		break;
	case 5:
		if(this->getHamburger()->getMagazineAmmo() > 0)
			this->setCurrentWeapon(this->getHamburger());
		else
			this->setCurrentWeapon(NULL);
		break;
	}

	if(this->currentWeapon != NULL && this->reloadUp && input->isKeyDownForAction(RELOAD))
	{
		this->reloadUp = false;
		
		if(!this->currentWeapon->isMelee())
			this->currentWeapon->reload();
	}
	else if(!input->isKeyDownForAction(RELOAD))
		this->reloadUp = true;


	///////////////
	// Mouse Input
	///////////////

	// calculate angle between line from sprite to cursor and horizontal line from sprite
	double theta, magnitude;
	int relativeCursorX, relativeCursorY;

	relativeCursorX = input->getMouseX() - (this->sprite->positionX + this->sprite->centerX);
	relativeCursorY = input->getMouseY() - (this->sprite->positionY + this->sprite->centerY);

	magnitude = std::sqrt((double)((relativeCursorX * relativeCursorX) + (relativeCursorY * relativeCursorY)));

	// theta in radians
	theta = std::acos((double)relativeCursorX / magnitude);

	//convert to degrees
	theta = theta * 180.0 / M_PI;

	if(relativeCursorY < 0)
		theta = -1 * theta;

	// set animation based on direction character is facing and whether they are moving.
	if(abs(theta) < 45 || abs(theta) > 135) 
	{
		// sideways facing
		this->direction = SIDE;
		if(this->prone)
			this->passiveAnimation = SIDE_CRAWL;
		else if(dx != 0 || dy != 0)
			this->passiveAnimation = SIDE_WALK;
		else
			this->passiveAnimation = SIDE_IDLE;
	}
	else if(theta >= 45 && theta <= 135)
	{
		// back facing
		this->direction = BACK;
		if(this->prone)
			this->passiveAnimation = BACK_CRAWL;
		else if(dx != 0 || dy != 0)
			this->passiveAnimation = BACK_WALK;
		else
			this->passiveAnimation = BACK_IDLE;
	}
	else if(theta <= -45 && theta >= -135)
	{
		// front facing
		this->direction = FRONT;
		if(this->prone)
			this->passiveAnimation = FRONT_CRAWL;
		else if(dx != 0 || dy != 0)
			this->passiveAnimation = FRONT_WALK;
		else
			this->passiveAnimation = FRONT_IDLE;
	}

	// flip sprite depending on x-coord of cursor relative to center of sprite
	if(std::abs(theta) > 90)
		this->sprite->flipHoriz = true;
	else
		this->sprite->flipHoriz = false;

	if(this->currentWeapon != NULL)
	{
		// fire weapon if attack key is down
		if(input->isKeyDownForAction(ATTACK) && weaponTimer >= 250 && (this->currentWeapon->isMelee() || this->currentWeapon->getMagazineAmmo() > 0))
		{
			if(this->currentWeapon == this->hamburger)
			{
				if(this->getHealth() < this->getMaxHealth())
				{
					if(this->getHealth() + this->currentWeapon->getDamage() > this->getMaxHealth())
						this->setHealth(this->getMaxHealth());
					else
						this->setHealth(this->getHealth() + this->currentWeapon->getDamage());
					this->currentWeapon->fire();

					SoundManager::getInstance()->playSoundEffect(HAMBURGER_EAT);
				}
			}
			else
			{
				// to find the velocity of the bullet/grenade, find the norm of the relative cursor coordinates and multiply by a constant
				double projectileDX, projectileDY;

				double speed;
				
				if(this->currentWeapon == this->grenade)
					speed = 200.0;
				else
					speed = 500.0;

				projectileDX = speed * ((double)relativeCursorX / magnitude);
				projectileDY = speed * ((double)relativeCursorY / magnitude);

				if(this->currentWeapon == this->grenade)
				{
					Grenade* g;
					g = new Grenade(this->worldX + SPRITE_SIZE/2, this->worldY + SPRITE_SIZE/2, projectileDX, projectileDY, magnitude, 3, true, this->currentWeapon->getDamage());
					g->setVisible(true);
					StateManager::getCurrentLevel()->addEntityAfterUpdate(g);
				}
				else
				{
					Bullet* b;
					b = new Bullet(this->worldX + SPRITE_SIZE/2, this->worldY + SPRITE_SIZE/2, projectileDX, projectileDY, this->currentWeapon->getRange(), 3, true, this->currentWeapon->getDamage());
					if(!this->currentWeapon->isMelee())
						b->setVisible(true);
					StateManager::getCurrentLevel()->addEntityAfterUpdate(b);

					switch(this->direction)
					{
					case FRONT:
						if(this->currentWeapon->isMelee())
							this->activeAnimation = FRONT_MELEE;
						else if(this->currentWeapon == this->pistol)
							this->activeAnimation = FRONT_PISTOL_SHOOT;
						else if(this->currentWeapon == this->rifle)
							this->activeAnimation = FRONT_RIFLE_SHOOT;
						break;
					case BACK:
						if(this->currentWeapon->isMelee())
							this->activeAnimation = BACK_MELEE;
						else if(this->currentWeapon == this->pistol)
							this->activeAnimation = BACK_PISTOL_SHOOT;
						else if(this->currentWeapon == this->rifle)
							this->activeAnimation = BACK_RIFLE_SHOOT;
						break;
					case SIDE:
						if(this->currentWeapon->isMelee())
							this->activeAnimation = SIDE_MELEE;
						else if(this->currentWeapon == this->pistol)
							this->activeAnimation = SIDE_PISTOL_SHOOT;
						else if(this->currentWeapon == this->rifle)
							this->activeAnimation = SIDE_RIFLE_SHOOT;
						break;
					}

					this->activeAnimationTicks = 4;
					frameTimer = 0;
				}

				if(!this->currentWeapon->isMelee())
				{
					// manage ammo
					this->currentWeapon->fire();

					if(this->currentWeapon == this->grenade)
						SoundManager::getInstance()->playSoundEffect(GRENADE_TOSS);
					else
					{
						SoundManager::getInstance()->playSoundEffect(RIFLE_SHOT);
						StateManager::getCurrentLevel()->addShots(1);
					}
				}
				else
					SoundManager::getInstance()->playSoundEffect(PIPE_SWING);
			}

			weaponTimer = 0;
		}
	}

	float newX, newY;
	bool collidedX = false;
	bool collidedY = false;

	newX = this->worldX;
	newY = this->worldY;

	// update position based on dx, dy, and dt
	newX += this->dx * (dt / 1000.0);
	newY += this->dy * (dt / 1000.0);

	// Check x collision and y collision separately so that player can continue to move
	// in one direction if they are up against a wall.

	// X collision
	// set new origin of bounding box based on position
	this->setCollisionOrigin(newX + COL_MARGIN, worldY + COL_MARGIN_Y);

	// check collision against all collidables in map
	for(std::vector<StaticCollidable*>::iterator it = StateManager::getCurrentLevel()->getStaticCollidables()->begin(); it != StateManager::getCurrentLevel()->getStaticCollidables()->end(); it++)
	{
		if(((*it)->getType() == FULL || ((*it)->getType() == CRAWL_THROUGH && !this->prone)) && this->hasCollided((*it)))
		{
			newX = this->worldX;
			break;
		}
	}

	// Y collision
	// set new origin of bounding box based on position
	this->setCollisionOrigin(worldX + COL_MARGIN, newY + COL_MARGIN_Y);

	// check collision against all collidables in map
	for(std::vector<StaticCollidable*>::iterator it = StateManager::getCurrentLevel()->getStaticCollidables()->begin(); it != StateManager::getCurrentLevel()->getStaticCollidables()->end(); it++)
	{
		if(((*it)->getType() == FULL || ((*it)->getType() == CRAWL_THROUGH && !this->prone)) && this->hasCollided((*it)))
		{
			newY = this->worldY;
			break;
		}
	}

	this->worldX = newX;
	this->worldY = newY;
	this->setCollisionOrigin(this->worldX + COL_MARGIN, this->worldY + COL_MARGIN_Y);

	// check for exits
	for(std::vector<Exit*>::iterator it = StateManager::getCurrentLevel()->getExits()->begin(); it != StateManager::getCurrentLevel()->getExits()->end(); it++)
	{
		if(this->hasCollided((*it)))
		{
			StateManager::getInstance()->getGameplayState()->setLevel((*it)->getDestination());
			break;
		}
	}

	// check for location triggers
	for(std::vector<Trigger*>::iterator it = StateManager::getCurrentLevel()->getLocationTriggers()->begin(); it != StateManager::getCurrentLevel()->getLocationTriggers()->end(); it++)
	{
		if(!(*it)->hasTriggered() && this->hasCollided((*it)))
		{
			StateManager::getInstance()->getGameplayState()->setDialogue((*it)->getFileName());
			(*it)->setTriggered(true);
			break;
		}
	}

	// check for pickups
	for(std::vector<PickUp*>::iterator it = StateManager::getCurrentLevel()->getPickUps()->begin(); it != StateManager::getCurrentLevel()->getPickUps()->end(); it++)
	{
		if(!(*it)->isFinished() && this->hasCollided((*it)))
		{
			(*it)->pickUp();
			break;
		}
	}

	if(this->interactUp && input->isKeyDownForAction(INTERACT))
	{
		this->interactUp = false;

		//check for nearby interactables
		for(std::vector<I_Interactable*>::iterator it = StateManager::getCurrentLevel()->getInteractables()->begin(); it != StateManager::getCurrentLevel()->getInteractables()->end(); it++)
		{
			std::pair<float, float> coords = (*it)->getWorldCoords();

			float distanceX = (coords.first + (*it)->getWidth() / 2) - (this->worldX + this->getWidth() / 2);
			float distanceY = (coords.second + (*it)->getHeight() / 2) - (this->worldY + this->getHeight() / 2);

			float distance = std::sqrt((distanceX * distanceX) + (distanceY * distanceY));

			if(distance < 100)
			{
				(*it)->interact();
				break;
			}
		}
	}

	//////////////////
	// Screen update
	//////////////////
	// convert world coords to screen coords and set sprite position
	std::pair<float, float> screenCoords = StateManager::getCurrentLevel()->convertWorldCoordsToScreenCoords(this->getWorldCoords());

	this->sprite->setPosition(screenCoords.first, screenCoords.second);

	// set appropriate animation
	if(this->activeAnimationTicks > 0)
	{
		assert(this->activeAnimation < NUM_PLAYER_ANIMATIONS);
		this->sprite->setCurrentAnimation(this->activeAnimation);

		// flip frame
		if(frameTimer > 100)
		{
			this->sprite->nextFrame();
			frameTimer = 0;
			this->activeAnimationTicks--;
		}
	}
	else
	{
		this->sprite->setCurrentAnimation(this->passiveAnimation);

		// flip frame
		if(frameTimer > 250)
		{
			if((this->passiveAnimation != FRONT_CRAWL && this->passiveAnimation != BACK_CRAWL && this->passiveAnimation != SIDE_CRAWL) || (this->dx != 0 || this->dy != 0))
				this->sprite->nextFrame();
			frameTimer = 0;
		}
	}
}

void Player::draw()
{
	this->sprite->draw();
}

std::pair<float, float> Player::getWorldCoords()
{
	return std::pair<float, float>(this->worldX, this->worldY);
}

float Player::getWidth()
{
	return this->sprite->getWidth();
}

float Player::getHeight()
{
	return this->sprite->getHeight();
}

void Player::setWorldCoords(float worldX, float worldY)
{
	this->worldX = worldX;
	this->worldY = worldY;
}

int Player::getLayerID()
{
	return this->sprite->layerID;
}

int Player::getScore()
{
	return score;
}

int Player::getHealth()
{
	return health;
}

int Player::getMaxHealth()
{
	return maxHealth;
}

float Player::getSpeed()
{
	return this->speed;
}

float Player::getEnergy()
{
	return this->energy;
}

bool Player::isSprinting()
{
	return this->sprinting;
}

bool Player::hasPistol()
{
	return this->pickedUpPistol;
}

bool Player::hasRifle()
{
	return this->pickedUpRifle;
}

void Player::pickUpPistol(bool b)
{
	this->pickedUpPistol = b;

	if(b && this->currentWeapon == NULL)
		this->currentWeapon = this->pistol;
}

void Player::pickUpRifle(bool b)
{
	this->pickedUpRifle = b;

	if(b && this->currentWeapon == NULL)
		this->currentWeapon = this->rifle;
}

Weapon* Player::getCurrentWeapon()
{
	return this->currentWeapon;
}

Weapon* Player::getPipe()
{
	return this->pipe;
}

Weapon* Player::getPistol()
{
	return this->pistol;
}

Weapon* Player::getRifle()
{
	return this->rifle;
}

Weapon* Player::getGrenade()
{
	return this->grenade;
}

Weapon* Player::getHamburger()
{
	return this->hamburger;
}

void Player::setCurrentWeapon(Weapon* w)
{
	assert(w == NULL || w == this->pipe || w == this->rifle || w == this->pistol || w == this->grenade || w == this->hamburger);

	this->currentWeapon = w;
}

Sprite* Player::getSprite()
{
	return sprite;
}

void Player::setScore(int s)
{
	score = s;
}

void Player::addScore(int s)
{
	score += s;
}

void Player::setHealth(int h)
{
	health = h;
}

void Player::setMaxHealth(int m)
{
	maxHealth = m;
}

void Player::setSprite(Sprite* s)
{
	sprite = s;
}

void Player::setVisible(bool v)
{
	this->visible = v;
}

void Player::resetPlayer()
{
	delete this->sprite;
	this->sprite = NULL;
	
	delete this->pipe;
	this->pipe = NULL;

	delete this->pistol;
	this->pistol = NULL;

	delete this->rifle;
	this->rifle = NULL;

	this->initializePlayer();
}

bool Player::isFinished()
{
	return false;
}

bool Player::isVisible()
{
	return visible;
}