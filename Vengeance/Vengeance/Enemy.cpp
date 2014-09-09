#include "Enemy.h"
#include "StateManager.h"
#include "DrawPrimitives.h"
#include "Bullet.h"

Enemy::Enemy(std::string spriteSheetName, float posX, float posY, int health, int maxHealth, int damage) : worldX(posX), worldY(posY), originalX(posX), originalY(posY),
	 health(health), maxHealth(maxHealth), damage(damage), detectionRange(225), patrolDelay(0), currentPatrolWaypointIndex(0), patrolDelayTimer(0), alertTimer(0), 
	 isAlertToPlayer(false), wasAlertToPlayer(false), passiveAnimation(ENEMY_FRONT_IDLE), activeAnimation(NUM_ENEMY_ANIMATIONS), activeAnimationTicks(0), frameTimer(0),
	 direction(FRONT)
{
	this->sprite = new Sprite(spriteSheetName);
	this->sprite->initialize(NUM_ENEMY_ANIMATIONS, SPRITE_SIZE, SPRITE_SIZE, posX, posY, 2);

	for(int i = 0; i < NUM_ENEMY_ANIMATIONS; i++)
	{
		this->sprite->addSpriteAnimRow(i, 0, 1024 - ((i + 1) * SPRITE_SIZE), SPRITE_SIZE, 0, 4);
	}

	dx = 0; 
	dy = 0;

	weaponTimer = 0;

	visible = false;
	finished = false;

	this->setCollisionBounds(this->worldX + 10, this->worldY + COL_MARGIN_Y, SPRITE_SIZE - 20, SPRITE_SIZE - 10);
}

Enemy::~Enemy()
{
	delete this->sprite;
	this->sprite = NULL;
}

void Enemy::update(double dt)
{
	bool shotFired = false;

	this->frameTimer += dt;

	if(this->weaponTimer < 2000)
		this->weaponTimer += dt;

	if(this->patrolDelayTimer > 0)
		this->patrolDelayTimer -= dt;

	if(this->alertTimer > 0)
		this->alertTimer -= dt;

	if(this->health == 0 && !this->finished)
	{
		this->finished = true;
		Player::getInstance()->setScore(Player::getInstance()->getScore() + 50);

		StateManager::getCurrentLevel()->addKills(1);
	}
	else
	{
		double deltaX, deltaY, distanceToPlayer, distanceToWaypoint, normX, normY, theta;

		std::pair<float, float> targetCoords = Player::getInstance()->getWorldCoords();

		// calculate distance to player
		deltaX = targetCoords.first - this->worldX;
		deltaY = targetCoords.second - this->worldY;

		distanceToPlayer = std::sqrt((deltaX * deltaX) + (deltaY * deltaY));

		if(distanceToPlayer <= this->detectionRange)
			this->alertToPlayer();

		// find movement target
		if(this->alertTimer > 0)
		{
			// move towards the player
			normX = deltaX / distanceToPlayer;
			normY = deltaY / distanceToPlayer;

			this->dx = 50 * normX;
			this->dy = 50 * normY;

			// fire at the player
			if(weaponTimer >= 2000)
			{
				// fire weapon at player's current location
				// to find the velocity of the bullet, multiply the norm of the delta coordinates by a constant
				double bulletDX, bulletDY;

				const double BULLET_SPEED = 500.0;

				bulletDX = BULLET_SPEED * normX;
				bulletDY = BULLET_SPEED * normY;

				Bullet* b;
				b = new Bullet(this->worldX + sprite->centerX, this->worldY + sprite->centerY, bulletDX, bulletDY, 1000, 3, false, this->damage);
				b->setVisible(true);

				StateManager::getCurrentLevel()->addEntityAfterUpdate(b);

				weaponTimer = 0;

				SoundManager::getInstance()->playSoundEffect(PISTOL_SHOT);

				shotFired = true;
			}
		}
		else
		{
			this->isAlertToPlayer = false;

			// move towards next waypoint if it exists
			if(this->patrolWaypoints.size() > 0 && this->patrolDelayTimer <= 0)
			{
				targetCoords = this->patrolWaypoints[this->currentPatrolWaypointIndex];

				if(std::abs(this->worldX - targetCoords.first) < 1 && std::abs(this->worldY - targetCoords.second) < 1)
				{
					// set next waypoint as target
					this->currentPatrolWaypointIndex = (this->currentPatrolWaypointIndex + 1) % this->patrolWaypoints.size();
					this->patrolDelayTimer = this->patrolDelay * 1000;
					this->dx = 0;
					this->dy = 0;
					this->wasAlertToPlayer = false;
				}
				else
				{
					deltaX = targetCoords.first - this->worldX;
					deltaY = targetCoords.second - this->worldY;

					distanceToWaypoint = std::sqrt((deltaX * deltaX) + (deltaY * deltaY));

					normX = deltaX / distanceToWaypoint;
					normY = deltaY / distanceToWaypoint;

					this->dx = 50 * normX;
					this->dy = 50 * normY;
				}
			}
			else if(this->patrolWaypoints.size() == 0)
			{
				if(std::abs(this->worldX - this->originalX) < 1 && std::abs(this->worldY - originalY) < 1)
				{
					this->dx = 0;
					this->dy = 0;
					this->wasAlertToPlayer = false;
					this->direction = FRONT;
				}
				else
				{
					deltaX = this->originalX - this->worldX;
					deltaY = this->originalY - this->worldY;

					distanceToWaypoint = std::sqrt((deltaX * deltaX) + (deltaY * deltaY));

					normX = deltaX / distanceToWaypoint;
					normY = deltaY / distanceToWaypoint;

					this->dx = 50 * normX;
					this->dy = 50 * normY;
				}
			}
			else
			{
				this->dx = 0;
				this->dy = 0;
			}
		}

		if(this->dx != 0 || this->dy != 0)
		{
			// calculate the direction the enemy is facing and set proper animation
			// theta in radians
			theta = std::acos(normX);

			//convert to degrees
			theta = theta * 180.0 / M_PI;

			if(deltaY < 0)
				theta = -1 * theta;

			// set animation based on direction character is facing and whether they are moving.
			if(abs(theta) < 45 || abs(theta) > 135) 
			{
				// sideways facing
				this->direction = SIDE;
				if(this->dx != 0 || this->dy != 0)
					this->passiveAnimation = ENEMY_SIDE_WALK;
				else
					this->passiveAnimation = ENEMY_SIDE_IDLE;
			}
			else if(theta >= 45 && theta <= 135)
			{
				// back facing
				this->direction = BACK;
				if(this->dx != 0 || this->dy != 0)
					this->passiveAnimation = ENEMY_BACK_WALK;
				else
					this->passiveAnimation = ENEMY_BACK_IDLE;
			}
			else if(theta <= -45 && theta >= -135)
			{
				// front facing
				this->direction = FRONT;
				if(this->dx != 0 || this->dy != 0)
					this->passiveAnimation = ENEMY_FRONT_WALK;
				else
					this->passiveAnimation = ENEMY_FRONT_IDLE;
			}

			// flip sprite depending on x-coord of cursor relative to center of sprite
			if(std::abs(theta) > 90)
				this->sprite->flipHoriz = true;
			else
				this->sprite->flipHoriz = false;
		}
		else // this->dx == 0 && this->dy == 0
		{
			switch(this->direction)
			{
			case FRONT:
				this->passiveAnimation = ENEMY_FRONT_IDLE;
				break;
			case BACK:
				this->passiveAnimation = ENEMY_BACK_IDLE;
				break;
			case SIDE:
				this->passiveAnimation = ENEMY_SIDE_IDLE;
				break;
			default:
				break;
			}
		}

		if(shotFired)
		{
			this->activeAnimationTicks = 4;

			switch(this->direction)
			{
			case FRONT:
				this->activeAnimation = ENEMY_FRONT_SHOOT;
				break;
			case BACK:
				this->activeAnimation = ENEMY_BACK_SHOOT;
				break;
			case SIDE:
				this->activeAnimation = ENEMY_SIDE_SHOOT;
				break;
			}
		}

		// COLLISION CALCULATIONS

		float newX, newY;

		newX = this->worldX;
		newY = this->worldY;

		newX += this->dx * (dt / 1000.0);
		newY += this->dy * (dt / 1000.0);

		// To optimize, only check for collision if enemy is alerted to the player.
		// Otherwise, the enemy should not be moving into collidables.
		if(this->isAlertToPlayer || this->wasAlertToPlayer)
		{
			// X collision
			// set new origin of bounding box based on position
			this->setCollisionOrigin(newX + 10, worldY + COL_MARGIN_Y);

			// check collision against all collidables in map
			for(std::vector<StaticCollidable*>::iterator it = StateManager::getCurrentLevel()->getStaticCollidables()->begin(); it != StateManager::getCurrentLevel()->getStaticCollidables()->end(); it++)
			{
				if(this->hasCollided((*it)))
				{
					newX = this->worldX;
					break;
				} 
			}

			// Y collision
			// set new origin of bounding box based on position
			this->setCollisionOrigin(worldX + 10, newY + COL_MARGIN_Y);

			// check collision against all collidables in map
			for(std::vector<StaticCollidable*>::iterator it = StateManager::getCurrentLevel()->getStaticCollidables()->begin(); it != StateManager::getCurrentLevel()->getStaticCollidables()->end(); it++)
			{
				if(this->hasCollided((*it)))
				{
					newY = this->worldY;
					break;
				}
			}
		}

		this->worldX = newX;
		this->worldY = newY;
		this->setCollisionOrigin(this->worldX + 10, this->worldY + COL_MARGIN_Y);

		// update sprite coords based on world coords
		std::pair<float, float> screenCoords = StateManager::getCurrentLevel()->convertWorldCoordsToScreenCoords(this->getWorldCoords());
		this->sprite->setPosition(screenCoords.first, screenCoords.second);

		// set appropriate animation
		if(this->activeAnimationTicks > 0)
		{
			assert(this->activeAnimation < NUM_ENEMY_ANIMATIONS);
			this->sprite->setCurrentAnimation(this->activeAnimation);

			// flip frame
			if(this->frameTimer > 100)
			{
				this->sprite->nextFrame();
				this->frameTimer = 0;
				this->activeAnimationTicks--;
			}
		}
		else
		{
			this->sprite->setCurrentAnimation(this->passiveAnimation);

			// flip frame
			if(this->frameTimer > 250)
			{
				this->sprite->nextFrame();
				this->frameTimer = 0;
			}
		}
	}
}

void Enemy::draw()
{
	this->sprite->draw();

	// draw a healthbar above the sprite if injured
	if(this->health < this->maxHealth)
	{
		float greenX, redX, y, greenLength, redLength, healthPercent;
		const float HEALTH_BAR_HEIGHT = 10;

		healthPercent = (float)this->health / (float)this->maxHealth;

		y = this->sprite->positionY + this->getHeight();
		greenX = this->sprite->positionX;
		greenLength = this->getWidth() * healthPercent;
		redX = greenX + greenLength;
		redLength = this->getWidth() - greenLength;

		DrawPrimitives::setColor(0, 1, 0);
		DrawPrimitives::drawRectangle(true, greenX, y, greenLength, HEALTH_BAR_HEIGHT, 0);

		DrawPrimitives::setColor(1, 0, 0);
		DrawPrimitives::drawRectangle(true, redX, y, redLength, HEALTH_BAR_HEIGHT, 0);
	}
}

std::pair<float, float> Enemy::getWorldCoords()
{
	return std::pair<float, float>(this->worldX, this->worldY);
}

void Enemy::setWorldCoords(float x, float y)
{
	this->worldX = x;
	this->worldY = y;
}

float Enemy::getWidth()
{
	return this->sprite->getWidth();
}

float Enemy::getHeight()
{
	return this->sprite->getHeight();
}

int Enemy::getLayerID()
{
	return this->sprite->layerID;
}

int Enemy::getHealth()
{
	return this->health;
}

int Enemy::getMaxHealth()
{
	return this->maxHealth;
}

int Enemy::getDamage()
{
	return this->damage;
}

Sprite* Enemy::getSprite()
{
	return this->sprite;
}

void Enemy::setHealth(int h)
{
	this->health = h;
}

void Enemy::setSprite(Sprite* s)
{
	this->sprite = s;
}

void Enemy::setVisible(bool v)
{
	this->visible = v;
}

int Enemy::getPatrolDelay()
{
	return this->patrolDelay;
}

void Enemy::setPatrolDelay(int delay)
{
	this->patrolDelay = delay;
}

void Enemy::addPatrolWaypoint(int x, int y)
{
	this->patrolWaypoints.push_back(std::pair<int, int>(x, y));
}

void Enemy::alertToPlayer()
{
	this->alertTimer = 5000;
	this->isAlertToPlayer = true;
	this->wasAlertToPlayer = true;
}

bool Enemy::isFinished()
{
	return this->finished;
}

bool Enemy::isVisible()
{
	return this->visible;
}
