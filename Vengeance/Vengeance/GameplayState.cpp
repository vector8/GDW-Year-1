#include "GameplayState.h"
#include "StateManager.h"
#include <assert.h>

GameplayState::GameplayState() : paused(false)
{
	this->currentLevel = NULL;
	this->ui = new GameplayUI();
}

GameplayState::~GameplayState()
{
	delete this->currentLevel;
	this->currentLevel = NULL;

	delete this->ui;
	this->ui = NULL;
}

void GameplayState::draw()
{
	this->currentLevel->draw();
	this->ui->draw();
}

void GameplayState::update(double dt)
{
	assert(this->currentLevel != NULL);

	if(Player::getInstance()->getHealth() > 0)
	{
		if(!this->paused)
			this->currentLevel->update(dt);

		this->ui->update(dt);
	}
	else
		StateManager::getInstance()->setCurrentState(GAME_OVER);
}

void GameplayState::setLevel(std::string fileName, bool showTransition)
{
	this->setDialogue("");

	if(this->currentLevel != NULL && showTransition)
		StateManager::getInstance()->setCurrentState(TRANSITION, this->currentLevel);
	this->currentLevel = new Level(fileName);
}

void GameplayState::setLevel(Level* l)
{
	assert(l != NULL);

	this->setDialogue("");

	this->currentLevel = l;
}

void GameplayState::setDialogue(std::string fileName)
{
	this->ui->setDialogue(fileName);
}

int GameplayState::getSelectedHotbar()
{
	return this->ui->getSelectedHotbar();
}

void GameplayState::setSelectedHotbar(int n)
{
	assert(n > 0 && n < 6);

	this->ui->setSelectedHotbar(n);
}

void GameplayState::setPaused(bool p)
{
	this->paused = p;
}