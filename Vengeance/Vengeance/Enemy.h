#pragma once
#include "Entity.h"
#include "ICollidable.h"

enum EnemyAnimations
{
	ENEMY_FRONT_IDLE,
	ENEMY_BACK_IDLE,
	ENEMY_SIDE_IDLE,
	ENEMY_FRONT_WALK,
	ENEMY_BACK_WALK,
	ENEMY_SIDE_WALK,
	ENEMY_FRONT_SHOOT,
	ENEMY_BACK_SHOOT,
	ENEMY_SIDE_SHOOT,
	NUM_ENEMY_ANIMATIONS
};

class Enemy : public Entity, public ICollidable
{
protected:
	int health, maxHealth, damage, patrolDelay, currentPatrolWaypointIndex;
	double patrolDelayTimer, alertTimer;
	int detectionRange;
	float worldX, worldY, originalX, originalY;
	double weaponTimer, frameTimer;
	double dx, dy;
	Sprite* sprite;
	bool visible, finished;
	std::vector<std::pair<int, int>> patrolWaypoints;
	
	SpriteDirections direction;
	EnemyAnimations activeAnimation, passiveAnimation;
	int activeAnimationTicks;

	bool isAlertToPlayer, wasAlertToPlayer;

public:
	Enemy(std::string spriteSheetName, float posX, float posY, int health, int maxHealth, int damage);
	~Enemy();

	virtual void update(double dt);
	void draw();

	std::pair<float, float> getWorldCoords();
	void setWorldCoords(float x, float y);

	float getWidth();
	float getHeight();

	int getLayerID();

	int getHealth();
	int getMaxHealth();
	int getDamage();
	Sprite* getSprite();

	void setHealth(int h);
	void setSprite(Sprite* s);

	void setVisible(bool v);

	int getPatrolDelay();
	void setPatrolDelay(int delay);

	void addPatrolWaypoint(int x, int y);

	virtual void alertToPlayer();

	bool isFinished();
	bool isVisible();
};