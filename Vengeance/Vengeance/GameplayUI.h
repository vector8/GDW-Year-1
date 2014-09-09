#pragma once

#include "MiscGameResources.h"
#include <vector>
#include "Sprite.h"

#define TEXT_SPACING 24

class GameplayUI
{
private:
	std::vector<std::vector<std::string>> currentDialogue;
	unsigned dialogueIndex;

	Sprite* dialogueBoxSprite;
	Sprite* dialoguePromptSprite;
	Sprite* ammoBox;
	Sprite *hotbar1, *hotbar2, *hotbar3, *hotbar4, *hotbar5;
	Sprite* map;

	int selectedHotbar;

	float promptAnimTimer;

	bool spaceUp;

	void initializeHotbarSprite(Sprite *(&s), int n);

public:
	GameplayUI();
	~GameplayUI();

	void draw();
	void update(double dt);

	void setDialogue(std::string fileName);
	void advanceDialogue();

	int getSelectedHotbar();
	void setSelectedHotbar(int n);
};