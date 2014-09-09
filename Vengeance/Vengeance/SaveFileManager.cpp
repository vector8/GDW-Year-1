#include "SaveFileManager.h"
#include <assert.h>
#include "StateManager.h"
#include "Game.h"

SaveFileManager::SaveFileManager()
{
}

SaveFileManager::~SaveFileManager()
{
}

SaveFileManager* SaveFileManager::getInstance()
{
	static SaveFileManager* instance = new SaveFileManager();

	return instance;
}

bool SaveFileManager::saveExists(int saveFileNumber)
{
	assert(saveFileNumber >= 1 && saveFileNumber <= 3);

	std::ifstream file;

	file.open("saves/save" + GenericMethods::intToString(saveFileNumber) + ".save");

	bool result = file.is_open();

	file.close();

	return result;
}

void SaveFileManager::save(int saveFileNumber)
{
	assert(saveFileNumber >= 1 && saveFileNumber <= 3);
	assert(StateManager::getInstance() != NULL);
	assert(StateManager::getInstance()->getGameplayState() != NULL);
	assert(StateManager::getInstance()->getGameplayState()->getCurrentLevel() != NULL);

	std::ofstream file;
	std::string fileName;
	Level* l = StateManager::getCurrentLevel();
	Player* p = Player::getInstance();

	fileName = "saves/save" + GenericMethods::intToString(saveFileNumber) + ".save";

	file.open(fileName);

	assert(file.is_open());

	//save stuff
	// level file
	file << l->getFileName() << std::endl;

	// enemies
	for(std::vector<Enemy*>::iterator it = l->getEnemies()->begin(); it != l->getEnemies()->end(); it++)
	{
		if(it != l->getEnemies()->begin())
			file << SEPARATOR;

		file << (*it)->getHealth() << SEPARATOR << (*it)->getWorldCoords().first << SEPARATOR << (*it)->getWorldCoords().second;
	}

	file << std::endl;

	// time triggers
	for(std::vector<Trigger*>::iterator it = l->getTimeTriggers()->begin(); it != l->getTimeTriggers()->end(); it++)
	{
		if(it != l->getTimeTriggers()->begin())
			file << SEPARATOR;

		if((*it)->hasTriggered())
			file << 1;
		else
			file << 0;
	}

	file << std::endl;

	// location triggers
	for(std::vector<Trigger*>::iterator it = l->getLocationTriggers()->begin(); it != l->getLocationTriggers()->end(); it++)
	{
		if(it != l->getLocationTriggers()->begin())
			file << SEPARATOR;

		if((*it)->hasTriggered())
			file << 1;
		else
			file << 0;
	}

	file << std::endl;

	// pickups
	for(std::vector<PickUp*>::iterator it = l->getPickUps()->begin(); it != l->getPickUps()->end(); it++)
	{
		if(it != l->getPickUps()->begin())
			file << SEPARATOR;

		if((*it)->isFinished())
			file << 1;
		else
			file << 0;
	}

	file << std::endl;

	// stats
	file << (int)(l->getTime()) << SEPARATOR << l->getKills() << SEPARATOR << l->getShots() << SEPARATOR << l->getDamageTaken() << std::endl;

	// PLAYER
	// health, maxhealth, score, worldX, worldY, hasPistol, hasRifle, selectedHotbar, pistolmag, pistolreserve, riflemag, riflereserve, grenades, hamburgers
	file << p->getHealth() << SEPARATOR << p->getMaxHealth() << SEPARATOR << p->getScore() << SEPARATOR;
	file << (int)(p->getWorldCoords().first) << SEPARATOR << (int)(p->getWorldCoords().second) << SEPARATOR;
	file << (int)(p->hasPistol()) << SEPARATOR << (int)(p->hasRifle()) << SEPARATOR;

	file << StateManager::getInstance()->getGameplayState()->getSelectedHotbar() << SEPARATOR;

	file << p->getPistol()->getMagazineAmmo() << SEPARATOR << p->getPistol()->getReserveAmmo() << SEPARATOR;
	file << p->getRifle()->getMagazineAmmo() << SEPARATOR << p->getRifle()->getReserveAmmo() << SEPARATOR;
	file << p->getGrenade()->getMagazineAmmo() << SEPARATOR << p->getHamburger()->getMagazineAmmo();
}

void SaveFileManager::load(int saveFileNumber)
{
	assert(saveFileNumber >= 1 && saveFileNumber <= 3);

	std::ifstream file;
	std::string line;
	std::vector<std::string> tokens;

	Level* l;
	Player* p = Player::getInstance();

	p->resetPlayer();

	file.open("saves/save" + GenericMethods::intToString(saveFileNumber) + ".save");

	assert(file.is_open());

	// level file
	std::getline(file, line);
	line = GenericMethods::trim(line);
	l = new Level(line);

	// enemies
	std::getline(file, line);
	line = GenericMethods::trim(line);
	tokens = GenericMethods::split(line, SEPARATOR);
	assert((unsigned)(tokens.size()) % 3 == 0);
	assert(tokens.size() == l->getEnemies()->size() * 3);

	for(unsigned i = 0; i < l->getEnemies()->size(); i++)
	{
		int health  = GenericMethods::stringToInt(tokens[3*i]);
		int worldX  = GenericMethods::stringToInt(tokens[(3*i) + 1]);
		int worldY  = GenericMethods::stringToInt(tokens[(3*i) + 2]);

		l->getEnemies()->at(i)->setHealth(health);
		l->getEnemies()->at(i)->setWorldCoords(worldX, worldY);
	}

	tokens.clear();

	// time triggers
	std::getline(file, line);
	line = GenericMethods::trim(line);
	tokens = GenericMethods::split(line, SEPARATOR);
	assert(tokens.size() == l->getTimeTriggers()->size());

	for(unsigned i = 0; i < l->getTimeTriggers()->size(); i++)
	{
		if(tokens[i] == "1")
			l->getTimeTriggers()->at(i)->setTriggered(true);
		else
			l->getTimeTriggers()->at(i)->setTriggered(false);
	}

	tokens.clear();
	
	// location triggers
	std::getline(file, line);
	line = GenericMethods::trim(line);
	tokens = GenericMethods::split(line, SEPARATOR);
	assert(tokens.size() == l->getLocationTriggers()->size());

	for(unsigned i = 0; i < l->getLocationTriggers()->size(); i++)
	{
		if(tokens[i] == "1")
			l->getLocationTriggers()->at(i)->setTriggered(true);
		else
			l->getLocationTriggers()->at(i)->setTriggered(false);
	}

	tokens.clear();

	// pickups
	std::getline(file, line);
	line = GenericMethods::trim(line);
	tokens = GenericMethods::split(line, SEPARATOR);
	assert(tokens.size() == l->getPickUps()->size());

	for(unsigned i = 0; i < l->getPickUps()->size(); i++)
	{
		if(tokens[i] == "1")
			l->getPickUps()->at(i)->setFinished(true);
		else
			l->getPickUps()->at(i)->setFinished(false);
	}

	tokens.clear();

	// stats
	std::getline(file, line);
	line = GenericMethods::trim(line);
	tokens = GenericMethods::split(line, SEPARATOR);

	assert(tokens.size() == 4);

	l->setTime(GenericMethods::stringToInt(tokens[0]));
	l->setKills(GenericMethods::stringToInt(tokens[1]));
	l->setShots(GenericMethods::stringToInt(tokens[2]));
	l->setDamageTaken(GenericMethods::stringToInt(tokens[3]));

	tokens.clear();

	// PLAYER
	std::getline(file, line);
	line = GenericMethods::trim(line);
	tokens = GenericMethods::split(line, SEPARATOR);
	int* tokenInts = new int[tokens.size()];

	assert(tokens.size() == 14);
	
	for(unsigned i = 0; i < tokens.size(); i++)
	{
		tokenInts[i] = GenericMethods::stringToInt(tokens[i]);
	}

	p->setHealth(tokenInts[0]);
	p->setMaxHealth(tokenInts[1]);
	p->setScore(tokenInts[2]);
	p->setWorldCoords(tokenInts[3], tokenInts[4]);
	if(tokenInts[5] == 0)
		p->pickUpPistol(false);
	else
		p->pickUpPistol(true);
	if(tokenInts[6] == 0)
		p->pickUpRifle(false);
	else
		p->pickUpRifle(true);
	
	int selectedHotbar = tokenInts[7];

	p->getPistol()->setMagazineAmmo(tokenInts[8]);
	p->getPistol()->setReserveAmmo(tokenInts[9]);
	p->getRifle()->setMagazineAmmo(tokenInts[10]);
	p->getRifle()->setReserveAmmo(tokenInts[11]);
	p->getGrenade()->setMagazineAmmo(tokenInts[12]);
	p->getHamburger()->setMagazineAmmo(tokenInts[13]);

	std::pair<float, float> cameraCoords = p->getWorldCoords();

	cameraCoords.first -= (float)Game::stateInfo.screenWidth / 2;
	cameraCoords.second -= (float)Game::stateInfo.screenHeight / 2;

	l->setCameraWorldCoords(cameraCoords);

	tokens.clear();
	delete tokenInts;

	StateManager::getInstance()->loadLevel(l);
	StateManager::getInstance()->getGameplayState()->setSelectedHotbar(selectedHotbar);
}