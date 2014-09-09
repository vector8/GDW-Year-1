#include "Level.h"
#include "Game.h"
#include <assert.h>
#include "NeutralMob.h"
#include "StateManager.h"
#include "Doodad.h"
#include "Bruiser.h"
#include "SubBoss.h"
#include "DocMaybe.h"

Level::Level(std::string fileName) : fileName(fileName), time(0), kills(0), shots(0), damageTaken(0), triggerTimer(0)
{
	this->load();
}

Level::~Level()
{
	delete this->tileMap;
	this->tileMap = NULL;
	delete this->mapSprite;
	this->mapSprite = NULL;

	while(!this->enemies.empty())
	{
		delete this->enemies.back();
		this->enemies.pop_back();
	}
	while(!this->entities.empty())
	{
		delete this->entities.back();
		this->entities.pop_back();
	}
	while(!this->tempEntities.empty())
	{
		delete this->tempEntities.back();
		this->tempEntities.pop_back();
	}
	while(!this->staticCollidables.empty())
	{
		delete this->staticCollidables.back();
		this->staticCollidables.pop_back();
	}
	while(!this->exits.empty())
	{
		delete this->exits.back();
		this->exits.pop_back();
	}
	while(!this->timeTriggers.empty())
	{
		delete this->timeTriggers.back();
		this->timeTriggers.pop_back();
	}
	while(!this->locationTriggers.empty())
	{
		delete this->locationTriggers.back();
		this->locationTriggers.pop_back();
	}
	while(!this->pickUps.empty())
	{
		delete this->pickUps.back();
		this->pickUps.pop_back();
	}

	// interactables get deleted in entities delete
	this->interactables.clear();
}

void Level::load()
{
	// parse the level file to extract map, enemy, and doodad info
	std::ifstream file;
	std::string line;

	std::string mapName;

	std::pair<float, float> cameraCoords(0, 0);

	Enemy* e;

	enum ParseMode
	{
		MAP_MODE,
		PLAYER_MODE,
		ENEMIES_MODE,
		DOODADS_MODE,
		NEUTRAL_MODE,
		TRIGGERS_MODE,
		PICKUPS_MODE,
		EXIT_MODE
	} parseMode;

	parseMode = MAP_MODE;

	file.open(this->fileName);

	while(std::getline(file, line))
	{
		line = GenericMethods::toLower(GenericMethods::trim(line));

		if(line.size() == 0 || line[0] == '#')
		{
			// ignore these lines
			continue;
		}
		else
		{
			// change mode
			if(line == "player")
				parseMode = PLAYER_MODE;
			else if(line == "enemies")
				parseMode = ENEMIES_MODE;
			else if(line == "doodads")
				parseMode = DOODADS_MODE;
			else if(line == "neutral")
				parseMode = NEUTRAL_MODE;
			else if(line == "triggers")
				parseMode = TRIGGERS_MODE;
			else if(line == "pickups")
				parseMode = PICKUPS_MODE;
			else if(line == "exit")
				parseMode = EXIT_MODE;
			else
			{
				// parse line
				std::vector<std::string> tokens;

				if(parseMode == MAP_MODE)
				{
					tokens = GenericMethods::split(line, " ");

					assert(tokens.size() == 4);

					this->tileMap = new TileMap("levels/" + tokens[0],
												GenericMethods::stringToInt(tokens[1]),
												GenericMethods::stringToInt(tokens[2]),
												tokens[3]);
					this->parseCollisionMap("levels/" + tokens[0],
											GenericMethods::stringToInt(tokens[1]),
											GenericMethods::stringToInt(tokens[2]));
				}
				else if(parseMode == PLAYER_MODE)
				{
					tokens = GenericMethods::split(line, " ");

					assert(this->tileMap);
					Player::getInstance()->setWorldCoords((GenericMethods::stringToInt(tokens[0]) - 1) * TILE_SIZE, (this->tileMap->getHeightTiles() - (GenericMethods::stringToInt(tokens[1]) - 1)) * TILE_SIZE);

					cameraCoords = Player::getInstance()->getWorldCoords();

					cameraCoords.first -= (float)Game::stateInfo.screenWidth / 2;
					cameraCoords.second -= (float)Game::stateInfo.screenHeight / 2;
				}
				else if(parseMode == ENEMIES_MODE)
				{
					tokens = GenericMethods::split(line, " ");

					if(GenericMethods::toLower(tokens[0]) == "patrol" && this->enemies.size() > 0)
					{
						assert(e);						// last enemy should exist
						assert(tokens.size() > 1);		// more than one token
						assert(tokens.size() % 2 == 0);	// even number of tokens

						e->setPatrolDelay(GenericMethods::stringToInt(tokens[1]));

						for(unsigned i = 2; i < tokens.size(); i += 2)
						{
							int waypointX, waypointY;

							waypointX = (GenericMethods::stringToInt(tokens[i]) - 1) * TILE_SIZE;
							waypointY = (this->tileMap->getHeightTiles() - (GenericMethods::stringToInt(tokens[i + 1]) - 1)) * TILE_SIZE;

							e->addPatrolWaypoint(waypointX,waypointY);
						}
					}
					else
					{
						assert(tokens.size() == 6);

						// custom AI based on the image name
						if(tokens[0] == "bruiser.png")
						{
							e = new Bruiser("images/" + tokens[0], 
												(GenericMethods::stringToInt(tokens[1]) - 1) * TILE_SIZE, 
												(this->tileMap->getHeightTiles() - (GenericMethods::stringToInt(tokens[2]) - 1)) * TILE_SIZE, 
												GenericMethods::stringToInt(tokens[3]), 
												GenericMethods::stringToInt(tokens[4]), 
												GenericMethods::stringToInt(tokens[5]));
						}
						else if(tokens[0] == "subboss.png")
						{
							e = new SubBoss("images/" + tokens[0], 
												(GenericMethods::stringToInt(tokens[1]) - 1) * TILE_SIZE, 
												(this->tileMap->getHeightTiles() - (GenericMethods::stringToInt(tokens[2]) - 1)) * TILE_SIZE, 
												GenericMethods::stringToInt(tokens[3]), 
												GenericMethods::stringToInt(tokens[4]), 
												GenericMethods::stringToInt(tokens[5]));
						}
						else if(tokens[0] == "docmaybe.png")
						{
							e = new DocMaybe("images/" + tokens[0], 
												(GenericMethods::stringToInt(tokens[1]) - 1) * TILE_SIZE, 
												(this->tileMap->getHeightTiles() - (GenericMethods::stringToInt(tokens[2]) - 1)) * TILE_SIZE, 
												GenericMethods::stringToInt(tokens[3]), 
												GenericMethods::stringToInt(tokens[4]), 
												GenericMethods::stringToInt(tokens[5]));
						}
						else
						{
							e = new Enemy("images/" + tokens[0], 
												(GenericMethods::stringToInt(tokens[1]) - 1) * TILE_SIZE, 
												(this->tileMap->getHeightTiles() - (GenericMethods::stringToInt(tokens[2]) - 1)) * TILE_SIZE, 
												GenericMethods::stringToInt(tokens[3]), 
												GenericMethods::stringToInt(tokens[4]), 
												GenericMethods::stringToInt(tokens[5]));
						}
						e->setVisible(true);

						this->enemies.push_back(e);
					}
				}
				else if(parseMode == DOODADS_MODE)
				{
					tokens = GenericMethods::split(line, " ");

					assert(tokens.size() == 6 || tokens.size() == 7);

					Doodad* d;

					if(tokens.size() == 3)
						d = new Doodad("images/" + tokens[0], 
										(GenericMethods::stringToInt(tokens[1]) - 1) * TILE_SIZE,
										(this->tileMap->getHeightTiles() - (GenericMethods::stringToInt(tokens[2]) - 1)) * TILE_SIZE,
										GenericMethods::stringToInt(tokens[3]),
										GenericMethods::stringToInt(tokens[4]),
										tokens[5] != "0");
					else
						d = new Doodad("images/" + tokens[0], 
										(GenericMethods::stringToInt(tokens[1]) - 1) * TILE_SIZE,
										(this->tileMap->getHeightTiles() - (GenericMethods::stringToInt(tokens[2]) - 1)) * TILE_SIZE,
										GenericMethods::stringToInt(tokens[3]),
										GenericMethods::stringToInt(tokens[4]),
										tokens[5] != "0",
										"dialogue/" + tokens[6]);

					this->entities.push_back(d);
					this->interactables.push_back(d);

					// create collidable for doodad if required
					if(d->isCollidable())
						this->staticCollidables.push_back(new StaticCollidable(FULL, d->getWorldCoords().first, d->getWorldCoords().second, d->getWidth(), d->getHeight()));
				}
				else if(parseMode == NEUTRAL_MODE)
				{
					tokens = GenericMethods::split(line, " ");

					assert(tokens.size() == 3 || tokens.size() == 4);

					NeutralMob* n;

					if(tokens.size() == 3)
						n = new NeutralMob(	"images/" + tokens[0], 
											(GenericMethods::stringToInt(tokens[1]) - 1) * TILE_SIZE,
											(this->tileMap->getHeightTiles() - (GenericMethods::stringToInt(tokens[2]) - 1)) * TILE_SIZE);
					else
						n = new NeutralMob(	"images/" + tokens[0], 
											(GenericMethods::stringToInt(tokens[1]) - 1) * TILE_SIZE,
											(this->tileMap->getHeightTiles() - (GenericMethods::stringToInt(tokens[2]) - 1)) * TILE_SIZE,
											"dialogue/" + tokens[3]);

					this->entities.push_back(n);
					this->interactables.push_back(n);
				}
				else if(parseMode == TRIGGERS_MODE)
				{
					tokens = GenericMethods::split(line, " ");

					if(tokens[0] == "time")
					{
						assert(tokens.size() == 3);

						int time = GenericMethods::stringToInt(tokens[1]);

						Trigger* t = new Trigger(TIME, "dialogue/" + tokens[2], time);

						this->timeTriggers.push_back(t);
					}
					else if(tokens[0] == "location")
					{
						assert(tokens.size() == 6);

						int x = (GenericMethods::stringToInt(tokens[1]) - 1) * TILE_SIZE;
						int y = (this->tileMap->getHeightTiles() - (GenericMethods::stringToInt(tokens[2]) - 1)) * TILE_SIZE;
						int width = GenericMethods::stringToInt(tokens[3]);
						int height = GenericMethods::stringToInt(tokens[4]);

						Trigger* t = new Trigger(LOCATION, "dialogue/" + tokens[5], x, y, width, height);
						
						this->locationTriggers.push_back(t);
					}
					else
					{
						assert(false);
					}
				}
				else if(parseMode == PICKUPS_MODE)
				{
					tokens = GenericMethods::split(line, " ");

					PickUpTypes type;

					if(tokens[0] == "pistol")
						type = PISTOL;
					else if(tokens[0] == "rifle")
						type = RIFLE;
					else if(tokens[0] == "pistol_ammo")
						type = PISTOL_AMMO;
					else if(tokens[0] == "rifle_ammo")
						type = RIFLE_AMMO;
					else if(tokens[0] == "grenade")
						type = GRENADE;
					else if(tokens[0] == "hamburger")
						type = HAMBURGER;
					else
						assert(false);

					PickUp* p = new PickUp(type, (GenericMethods::stringToInt(tokens[1]) - 1) * TILE_SIZE + DOODAD_SIZE, (this->tileMap->getHeightTiles() - (GenericMethods::stringToInt(tokens[2]) - 1)) * TILE_SIZE + DOODAD_SIZE);

					this->pickUps.push_back(p);
				}
				else if(parseMode == EXIT_MODE)
				{
					tokens = GenericMethods::split(line, " ");

					assert(tokens.size() == 3);

					this->exits.push_back(new Exit(	"levels/" + tokens[2],
													(GenericMethods::stringToInt(tokens[0]) - 1) * TILE_SIZE, 
													(this->tileMap->getHeightTiles() - (GenericMethods::stringToInt(tokens[1]) - 1)) * TILE_SIZE));
				}
			}
		}
	}

	this->setCameraWorldCoords(cameraCoords);

	file.close();

	this->mapSprite = new Sprite("images/AcrewoodPlayerMap.png");
	this->mapSprite->initialize(1, 700, 500, 50, 50, 5);
	this->mapSprite->addSpriteAnimFrame(0, 0, 0);
	this->mapSprite->setCurrentAnimation(0);
}

void Level::parseCollisionMap(std::string fileName, int width, int height)
{
	std::ifstream file;
	std::string line;

	int lineNumber = height;
	
	file.open(fileName);

	while(std::getline(file, line))
	{
		for(unsigned colNumber = 0; colNumber < line.size(); colNumber++)
		{
			std::vector<StaticCollidable*> collidablesInCurrentTile = StaticCollidable::createCollidables(line[colNumber], colNumber * TILE_SIZE, lineNumber * TILE_SIZE);

			for(std::vector<StaticCollidable*>::iterator it = collidablesInCurrentTile.begin(); it != collidablesInCurrentTile.end(); it++)
			{
				this->staticCollidables.push_back((*it));
			}
		}

		lineNumber--;
	}
}

void Level::update(double dt)
{
	this->triggerTimer += dt;

	this->addTime(dt);

	this->tileMap->update(dt);

	Player::getInstance()->update(dt);

	// update enemies
	for(std::vector<Enemy*>::iterator it = this->enemies.begin(); it != this->enemies.end(); it++)
	{
		// do not erase finished enemies, they are needed for save
		if(!(*it)->isFinished())
		{
			(*it)->update(dt);
		}
	}

	// update misc entities
	std::vector<Entity*>::iterator itEntity = this->entities.begin();
	while(itEntity != this->entities.end())
	{
		Entity* e = (*itEntity);

		e->update(dt);

		if(e->isFinished())
		{
			itEntity = this->entities.erase(itEntity);

			delete e;
		}
		else 
			itEntity++;
	}

	// add all temp entities to regular entity list since update is complete
	for(std::vector<Entity*>::iterator it=this->tempEntities.begin(); it != this->tempEntities.end();it++)
	{
		entities.push_back((*it));
	}

	tempEntities.clear();

	// update time-based triggers
	for(std::vector<Trigger*>::iterator it = this->timeTriggers.begin(); it != this->timeTriggers.end(); it++)
	{
		if(!(*it)->hasTriggered() && (*it)->getTime() <= this->triggerTimer)
		{
			StateManager::getInstance()->getGameplayState()->setDialogue((*it)->getFileName());

			(*it)->setTriggered(true);
		}
	}

	// update pickups
	for(std::vector<PickUp*>::iterator it = this->pickUps.begin(); it != this->pickUps.end(); it++)
	{
		// do not erase finished pickups, they are needed for save
		if(!(*it)->isFinished())
		{
			(*it)->update(dt);
		}
	}

}

void Level::draw()
{
	this->tileMap->draw();

	Player::getInstance()->draw();

 	for(std::vector<Enemy*>::iterator it = this->enemies.begin(); it != this->enemies.end(); it++)
	{
		if((*it)->isVisible() && !(*it)->isFinished() && this->tileMap->isOnScreen((*it)->getWorldCoords()))
			(*it)->draw();
	}

	for(std::vector<Entity*>::iterator it = this->entities.begin(); it != this->entities.end(); it++)
	{
		if((*it)->isVisible() && this->tileMap->isOnScreen((*it)->getWorldCoords()))
			(*it)->draw();
	}

	for(std::vector<PickUp*>::iterator it = this->pickUps.begin(); it != this->pickUps.end(); it++)
	{
		if((*it)->isVisible() && !(*it)->isFinished() && this->tileMap->isOnScreen((*it)->getWorldCoords()))
			(*it)->draw();
	}
}

// called from GameplayUI
void Level::drawMap()
{
	//this->mapSprite->draw();
	Game::setTexturing(false);
	{
		// draw map
		DrawPrimitives::setColor(1,1,1);
		DrawPrimitives::drawRectangle(true, 50, 50, 700, 500, 0);
		DrawPrimitives::setColor(0,0,0);
		for(std::vector<StaticCollidable*>::iterator it = this->staticCollidables.begin(); it != this->staticCollidables.end(); it++)
		{
			std::pair<float, float> coords = convertWorldCoordsToPlayerMapCoords((*it)->getX(), (*it)->getY());
			std::pair<float, float> widthHeight = convertWorldCoordsToPlayerMapCoords((*it)->getWidth(), (*it)->getHeight());

			if((*it)->getType() == FULL)
				DrawPrimitives::setColor(0, 0, 0);
			else if((*it)->getType() == CRAWL_THROUGH)
				DrawPrimitives::setColor(0.5, 0.5, 0.5);

			DrawPrimitives::drawRectangle(true, coords.first + 50, coords.second + 26, widthHeight.first + 1, widthHeight.second + 1, 0);
		}

		// draw player blip on map
		std::pair<float, float> playerCoords = Player::getInstance()->getWorldCoords();
		std::pair<float, float> mapCoords = convertWorldCoordsToPlayerMapCoords(playerCoords.first, playerCoords.second);

		DrawPrimitives::setColor(0,0,1);
		DrawPrimitives::drawFilledCircle(5, 5, mapCoords.first + 60, mapCoords.second + 36);
	}
	Game::setTexturing(true);
}

std::pair<float, float> Level::convertWorldCoordsToPlayerMapCoords(float x, float y)
{
	return std::pair<float, float>((x / (float)this->tileMap->getWidthPixels()) * 700.f, (y / (float)this->tileMap->getHeightPixels()) * 500.f);
}

void Level::addEntity(Entity* e)
{
	if(e)
		this->entities.push_back(e);
}

void Level::addEntityAfterUpdate(Entity* e)
{
	if(e)
		this->tempEntities.push_back(e);
}

std::vector<Enemy*>* Level::getEnemies()
{
	return &this->enemies;
}

std::vector<Entity*>* Level::getEntities()
{
	return &this->entities;
}

std::vector<Exit*>* Level::getExits()
{
	return &this->exits;
}

std::vector<I_Interactable*>* Level::getInteractables()
{
	return &this->interactables;
}

std::vector<Trigger*>* Level::getTimeTriggers()
{
	return &this->timeTriggers;
}

std::vector<Trigger*>* Level::getLocationTriggers()
{
	return &this->locationTriggers;
}

std::vector<PickUp*>* Level::getPickUps()
{
	return &this->pickUps;
}

void Level::addTime(double t)
{
	this->time += t;
}

void Level::addKills(int k)
{
	this->kills += k;
}

void Level::addShots(int s)
{
	this->shots += s;
}

void Level::addDamageTaken(int d)
{
	this->damageTaken += d;
}

void Level::setTime(double t)
{
	this->time = t;
}

void Level::setKills(int k)
{
	this->kills = k;
}

void Level::setShots(int s)
{
	this->shots = s;
}

void Level::setDamageTaken(int d)
{
	this->damageTaken = d;
}

void Level::setCameraWorldCoords(std::pair<float, float> cameraCoords)
{
	assert(this->tileMap);

	this->tileMap->setWorldCoordsOfCamera(cameraCoords);
}

int Level::getTime()
{
	return this->time;
}

int Level::getKills()
{
	return this->kills;
}

int Level::getShots()
{
	return this->shots;
}

int Level::getDamageTaken()
{
	return this->damageTaken;
}

std::string Level::getFileName()
{
	return this->fileName;
}

MusicTracks Level::getMusic()
{
	if(this->fileName == "levels/acrewood.lvl")
	{
		return ACREWOOD_THEME;
	}
	else if(this->fileName == "levels/downtown.lvl")
	{
		return DOWNTOWN_THEME;
	}
	else if(this->fileName == "levels/industrial.lvl")
	{
		return INDUSTRIAL_THEME;
	}
	else if(this->fileName == "levels/undercitym1.lvl" || this->fileName == "levels/undercitym2.lvl")
	{
		return UNDERCITY_THEME;
	}
	else if(this->fileName == "levels/warehouse.lvl")
	{
		return WAREHOUSE_THEME;
	}
	else
	{
		assert(false);
		return MUSIC_TRACKS_END;
	}
}

std::pair<float, float> Level::convertWorldCoordsToScreenCoords(std::pair<float, float> worldCoords)
{
	return this->tileMap->convertWorldCoordsToScreenCoords(worldCoords);
}

std::vector<StaticCollidable*>* Level::getStaticCollidables()
{
	return &this->staticCollidables;
}