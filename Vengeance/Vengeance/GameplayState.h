#pragma once

#include "State.h"
#include "Level.h"
#include "GameplayUI.h"

// State in which gameplay takes place.
class GameplayState : public State
{
private:
	Level* currentLevel;
	bool paused;
	GameplayUI* ui;

public:
	GameplayState();
	~GameplayState();

	void draw();
	void update(double dt);

	void setLevel(std::string fileName, bool showtransition = true);
	void setLevel(Level* l);

	void setDialogue(std::string fileName);

	Level* getCurrentLevel()
	{
		return this->currentLevel;
	}

	int getSelectedHotbar();
	void setSelectedHotbar(int n);

	void setPaused(bool p);
};