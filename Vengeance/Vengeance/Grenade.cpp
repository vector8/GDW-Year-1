#include "Grenade.h"
#include "StateManager.h"
#include "DrawPrimitives.h"
#include "MiscGameResources.h"
#include "SoundManager.h"

Grenade::Grenade(float posX, float posY, double dx, double dy, int range, int layerID, bool fromPlayer, int damage)
	: worldX(posX), worldY(posY), dx(dx), dy(dy), range(range), layerID(layerID), fromPlayer(fromPlayer), damage(damage)
{
	this->visible = false;
	this->finished = false;
	this->distanceTravelled = 0;
	this->fuse = 0;
	this->currentRadius = 0;

	this->setCollisionBounds(worldX, worldY, 1, 1);
}

Grenade::~Grenade()
{

}

void Grenade::update(double dt)
{
	float tempX, tempY;

	if(this->fuse < 3000)
	{
		this->fuse += dt;

		if(this->fuse >= 3000)
			SoundManager::getInstance()->playSoundEffect(GRENADE_EXPLODE);

		tempX = (dt / 1000.0) * this->dx;
		tempY = (dt / 1000.0) * this->dy;

		distanceTravelled += std::sqrt(tempX * tempX + tempY * tempY);

		this->worldX += tempX;
		this->worldY += tempY;

		this->setCollisionOrigin(worldX, worldY);

		if(distanceTravelled > range)
		{
			this->dx = 0;
			this->dy = 0;
		}

		for(std::vector<StaticCollidable*>::iterator it = StateManager::getCurrentLevel()->getStaticCollidables()->begin(); it != StateManager::getCurrentLevel()->getStaticCollidables()->end(); it++)
		{
			if(this->hasCollided((*it)))
			{
				this->dx = 0;
				this->dy = 0;
				break;
			}
		}
	}
	else if(this->currentRadius < GRENADE_EXPLOSION_RADIUS)
		this->currentRadius += 5;
	else
	{
		if(this->isFromPlayer())
		{
			std::vector<Enemy*>* enemies = StateManager::getCurrentLevel()->getEnemies();
			for(std::vector<Enemy*>::iterator it = enemies->begin(); it != enemies->end(); it++)
			{
				if(!(*it)->isFinished())
				{
					float x = ((*it)->getWorldCoords().first + (*it)->getWidth() / 2) - this->worldX;
					float y = ((*it)->getWorldCoords().second + (*it)->getHeight() / 2) - this->worldY;

					float distance = std::sqrt(x * x + y * y);

					if(distance <= GRENADE_EXPLOSION_RADIUS)
					{

						if((*it)->getHealth() > this->damage)
						{
							(*it)->setHealth((*it)->getHealth() - this->damage);
							(*it)->alertToPlayer();
						}
						else
							(*it)->setHealth(0);
					}
				}
			}
		}
		else
		{
			Player* p = Player::getInstance();

			float x = (p->getWorldCoords().first + p->getWidth() / 2) - this->worldX;
			float y = (p->getWorldCoords().second + p->getHeight() / 2) - this->worldY;

			float distance = std::sqrt(x * x + y * y);

			if(distance <= GRENADE_EXPLOSION_RADIUS)
			{
				StateManager::getCurrentLevel()->addDamageTaken(this->damage);

				if(p->getHealth() > this->damage)
					p->setHealth(p->getHealth() - this->damage);
				else
					p->setHealth(0);
			}
		}

		this->finished = true;
	}
}

void Grenade::draw()
{
	if(visible)
	{
		std::pair<float, float> screenCoords = StateManager::getCurrentLevel()->convertWorldCoordsToScreenCoords(this->getWorldCoords());

		if(this->fuse < 3000)
		{
			DrawPrimitives::setColor(0,.5,0);

			DrawPrimitives::drawFilledCircle(6, 5, screenCoords.first, screenCoords.second);
		}
		else
		{
			DrawPrimitives::setColor(1, 0, 0);
			DrawPrimitives::drawCircle(20, this->currentRadius, screenCoords.first, screenCoords.second);
		}
	}
}

std::pair<float, float> Grenade::getWorldCoords()
{
	return std::pair<float, float>(this->worldX, this->worldY);
}

float Grenade::getWidth()
{
	return 10;
}

float Grenade::getHeight()
{
	return 10;
}

int Grenade::getLayerID()
{
	return layerID;
}

void Grenade::setVisible(bool v)
{
	visible = v;
}

bool Grenade::isVisible()
{
	return visible;
}

bool Grenade::isFinished()
{
	return finished;
}

bool Grenade::isFromPlayer()
{
	return fromPlayer;
}