#pragma once
#include "Enemy.h"

class Bruiser : public Enemy
{

public:
	Bruiser(std::string spriteSheetName, float posX, float posY, int health, int maxHealth, int damage);
	~Bruiser();
	
	void update(double dt);
};