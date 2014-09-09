#pragma once
#include "Enemy.h"

class SubBoss : public Enemy
{
private:
	int burstShots;
	double burstTimer;

public:
	SubBoss(std::string spriteSheetName, float posX, float posY, int health, int maxHealth, int damage);
	~SubBoss();
	
	void update(double dt);

	void alertToPlayer();
};