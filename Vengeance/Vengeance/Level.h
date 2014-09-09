#pragma once
#include <vector>
#include "Entity.h"
#include "TileMap.h"
#include "Enemy.h"
#include "StaticCollidable.h"
#include "Exit.h"
#include "I_Interactable.h"
#include "Trigger.h"
#include "PickUp.h"
#include "SoundManager.h"

class Level
{
private:
	std::string fileName;

	TileMap* tileMap;

	Sprite* mapSprite;

	std::vector<Enemy*> enemies;
	std::vector<StaticCollidable*> staticCollidables;
	std::vector<Exit*> exits;
	std::vector<I_Interactable*> interactables;
	std::vector<Trigger*> timeTriggers;
	std::vector<Trigger*> locationTriggers;
	std::vector<PickUp*> pickUps;

	// stats
	int time, kills, shots, damageTaken;
	double triggerTimer;

	// these are added after load, such as bullets when fired
	std::vector<Entity*> entities, tempEntities;

	void load();

	void parseCollisionMap(std::string fileName, int width, int height);

	std::pair<float, float> convertWorldCoordsToPlayerMapCoords(float x, float y);

public:
	Level(std::string fileName);
	~Level();

	void update(double dt);
	void draw();
	void drawMap();

	void addEntity(Entity* e);
	void addEntityAfterUpdate(Entity* e);

	std::vector<Enemy*>* getEnemies();
	std::vector<Entity*>* getEntities();
	std::vector<Exit*>* getExits();
	std::vector<I_Interactable*>* getInteractables();
	std::vector<Trigger*>* getTimeTriggers();
	std::vector<Trigger*>* getLocationTriggers();
	std::vector<PickUp*>* getPickUps();

	void addTime(double t);
	void addKills(int k);
	void addShots(int s);
	void addDamageTaken(int d);

	void setTime(double t);
	void setKills(int k);
	void setShots(int s);
	void setDamageTaken(int d);

	void setCameraWorldCoords(std::pair<float, float> cameraCoords);

	int getTime();
	int getKills();
	int getShots();
	int getDamageTaken();

	std::string getFileName();

	MusicTracks getMusic();

	std::pair<float, float> convertWorldCoordsToScreenCoords(std::pair<float, float> worldCoords);

	std::vector<StaticCollidable*>* getStaticCollidables();
};