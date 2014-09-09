#include "Bullet.h"
#include "StateManager.h"
#include "DrawPrimitives.h"
#include "MiscGameResources.h"

Bullet::Bullet(float posX, float posY, double dx, double dy, int range, int layerID, bool fromPlayer, int damage)
	: worldX(posX), worldY(posY), dx(dx), dy(dy), range(range), layerID(layerID), fromPlayer(fromPlayer), damage(damage)
{
	this->visible = false;
	this->finished = false;
	this->distanceTravelled = 0;

	this->setCollisionBounds(worldX, worldY, 1, 1);
}

Bullet::~Bullet()
{

}

void Bullet::update(double dt)
{
	float tempX, tempY;

	tempX = (dt / 1000.0) * this->dx;
	tempY = (dt / 1000.0) * this->dy;

	distanceTravelled += std::sqrt(tempX * tempX + tempY * tempY);

	worldX += tempX;
	worldY += tempY;

	this->setCollisionOrigin(worldX, worldY);

	std::pair<float, float> screenCoords = StateManager::getCurrentLevel()->convertWorldCoordsToScreenCoords(this->getWorldCoords());

	if((screenCoords.first < 0 || screenCoords.first > Game::stateInfo.screenWidth || screenCoords.second < 0 || screenCoords.second > Game::stateInfo.screenHeight) || distanceTravelled > range)
	{
		finished = true;
		return;
	}

	if(this->isFromPlayer())
	{
		std::vector<Enemy*>* enemies = StateManager::getCurrentLevel()->getEnemies();
		for(std::vector<Enemy*>::iterator it = enemies->begin(); it != enemies->end(); it++)
		{
			if(!(*it)->isFinished() && this->hasCollided((*it)))
			{
				if((*it)->getHealth() > this->damage)
				{
					(*it)->setHealth((*it)->getHealth() - this->damage);
					(*it)->alertToPlayer();
				}
				else
					(*it)->setHealth(0);

				finished = true;
				break;
			}
		}
	}
	else
	{
		Player* p = Player::getInstance();

		if(this->hasCollided(p))
		{
			StateManager::getCurrentLevel()->addDamageTaken(this->damage);

			if(p->getHealth() > this->damage)
				p->setHealth(p->getHealth() - this->damage);
			else
				p->setHealth(0);
			
			finished = true;
		}
	}

	if(!finished)
	{
		for(std::vector<StaticCollidable*>::iterator it = StateManager::getCurrentLevel()->getStaticCollidables()->begin(); it != StateManager::getCurrentLevel()->getStaticCollidables()->end(); it++)
		{
			if((*it)->getType() == FULL && this->hasCollided((*it)))
			{
				finished = true;
				break;
			}
		}
	}
}

void Bullet::draw()
{
	if(visible)
	{
		std::pair<float, float> screenCoords = StateManager::getCurrentLevel()->convertWorldCoordsToScreenCoords(this->getWorldCoords());

		DrawPrimitives::setColor(0,0,1);

		DrawPrimitives::drawFilledCircle(6, 5, screenCoords.first, screenCoords.second);
	}
}

std::pair<float, float> Bullet::getWorldCoords()
{
	return std::pair<float, float>(this->worldX, this->worldY);
}

float Bullet::getWidth()
{
	return 10;
}

float Bullet::getHeight()
{
	return 10;
}

int Bullet::getLayerID()
{
	return layerID;
}

void Bullet::setVisible(bool v)
{
	visible = v;
}

bool Bullet::isVisible()
{
	return visible;
}

bool Bullet::isFinished()
{
	return finished;
}

bool Bullet::isFromPlayer()
{
	return fromPlayer;
}