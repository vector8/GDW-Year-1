#pragma once
#include "MiscGameResources.h"
#include "Entity.h"
#include "Sprite.h"
#include "ICollidable.h"
#include "Weapon.h"

#define COL_MARGIN 20
#define COL_MARGIN_Y 5

#define PIPE_DAMAGE 10
#define MELEE_RANGE 50

#define PISTOL_MAG_CAP 7
#define PISTOL_MAX_AMMO 115
#define PISTOL_DAMAGE 25

#define RIFLE_MAG_CAP 30
#define RIFLE_MAX_AMMO 500
#define RIFLE_DAMAGE 35

#define GRENADE_CAP 3
#define GRENADE_DAMAGE 75
#define GRENADE_RANGE 400

#define HAMBURGER_CAP 5
#define HAMBURGER_HEALING 25

#define GUN_RANGE 1000

enum PlayerAnimations
{
	FRONT_IDLE,
	BACK_IDLE,
	SIDE_IDLE,
	FRONT_WALK,
	BACK_WALK,
	SIDE_WALK,
	FRONT_CRAWL,
	BACK_CRAWL,
	SIDE_CRAWL,
	FRONT_DEATH,
	BACK_DEATH,
	SIDE_DEATH,
	FRONT_MELEE,
	BACK_MELEE,
	SIDE_MELEE,
	FRONT_PISTOL_SHOOT,
	BACK_PISTOL_SHOOT,
	SIDE_PISTOL_SHOOT,
	FRONT_RIFLE_SHOOT,
	BACK_RIFLE_SHOOT,
	SIDE_RIFLE_SHOOT,
	NUM_PLAYER_ANIMATIONS
};

class Player : public Entity, public ICollidable
{
private:
	Player();
	~Player();

	int score, health, maxHealth;
	float dx, dy;
	float worldX, worldY;
	float speed, energy;
	Sprite* sprite;
	bool sprinting, prone;
	bool visible;
	SpriteDirections direction;
	PlayerAnimations activeAnimation, passiveAnimation;
	int activeAnimationTicks;

	bool interactUp, reloadUp;

	bool pickedUpPistol, pickedUpRifle;

	Weapon* currentWeapon;
	Weapon* rifle;
	Weapon* pistol;
	Weapon* pipe;
	Weapon* grenade;
	Weapon* hamburger;

	void initializePlayer();
	void initializeSprite();

public:
	static Player* getInstance();

	void update(double dt);
	void draw();

	std::pair<float, float> getWorldCoords();

	float getWidth();
	float getHeight();

	void setWorldCoords(float worldX, float worldY);

	int getLayerID();

	int getScore();

	int getHealth();
	int getMaxHealth();

	float getEnergy();

	float getSpeed();

	bool isSprinting();

	bool hasPistol();
	bool hasRifle();

	void pickUpPistol(bool b);
	void pickUpRifle(bool b);

	Weapon* getCurrentWeapon();
	Weapon* getPipe();
	Weapon* getPistol();
	Weapon* getRifle();
	Weapon* getGrenade();
	Weapon* getHamburger();

	void setCurrentWeapon(Weapon* w);

	Sprite* getSprite();

	void setScore(int s);
	void addScore(int s);

	void setHealth(int h);
	void setMaxHealth(int m);
	
	void setSprite(Sprite* s);

	void setVisible(bool v);

	void resetPlayer();

	bool isFinished();
	bool isVisible();
};