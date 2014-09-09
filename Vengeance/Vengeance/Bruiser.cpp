#include "Bruiser.h"
#include "Player.h"
#include "StateManager.h"
#include "Bullet.h"
#include <assert.h>

Bruiser::Bruiser(std::string spriteSheetName, float posX, float posY, int health, int maxHealth, int damage) : Enemy(spriteSheetName, posX, posY, health, maxHealth, damage)
{
}

Bruiser::~Bruiser()
{

}
	
void Bruiser::update(double dt)
{
	bool shotFired = false;

	this->frameTimer += dt;

	if(this->weaponTimer < 500)
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

			this->dx = 150 * normX;
			this->dy = 150 * normY;

			if(weaponTimer >= 500 && distanceToPlayer < 50)
			{
				// swing at the player
				// use an invisible bullet
				double bulletDX, bulletDY;

				const double BULLET_SPEED = 500.0;

				bulletDX = BULLET_SPEED * normX;
				bulletDY = BULLET_SPEED * normY;

				Bullet* b;
				b = new Bullet(this->worldX + sprite->centerX, this->worldY + sprite->centerY, bulletDX, bulletDY, 50, 3, false, this->damage);
				b->setVisible(false);

				StateManager::getCurrentLevel()->addEntityAfterUpdate(b);

				weaponTimer = 0;

				SoundManager::getInstance()->playSoundEffect(PIPE_SWING);

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