#pragma once
#include "Enemy.h"

class DocMaybe : public Enemy
{
private:
	double grenadeTimer;
	int grenadeCount;

	bool dead;

public:
	DocMaybe(std::string spriteSheetName, float posX, float posY, int health, int maxHealth, int damage);
	~DocMaybe();
	
	void update(double dt);

	void alertToPlayer();
};