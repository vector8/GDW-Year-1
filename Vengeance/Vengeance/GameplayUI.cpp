#include "GameplayUI.h"
#include "DrawPrimitives.h"
#include "Player.h"
#include "InputManager.h"
#include "StateManager.h"

GameplayUI::GameplayUI() : spaceUp(true), dialogueIndex(0), promptAnimTimer(0), selectedHotbar(1)
{
	this->dialogueBoxSprite = new Sprite("images/dialogue_box.png");
	this->dialogueBoxSprite->initialize(1, 720, 160, 40, 40, 3);
	this->dialogueBoxSprite->addSpriteAnimFrame(0, 0, 0);
	this->dialogueBoxSprite->setCurrentAnimation(0);

	this->dialoguePromptSprite = new Sprite("images/dialogue_prompt.png");
	this->dialoguePromptSprite->initialize(1, 16, 16, 734, 50, 3);
	this->dialoguePromptSprite->addSpriteAnimRow(0, 0, 0, 16, 0, 2);
	this->dialoguePromptSprite->setCurrentAnimation(0);

	this->ammoBox = new Sprite("images/ammo_display.png");
	this->ammoBox->initialize(3, 160, 50, 623, 18, 3);
	this->ammoBox->addSpriteAnimFrame(0, 0, 100);
	this->ammoBox->addSpriteAnimFrame(1, 0, 50);
	this->ammoBox->addSpriteAnimFrame(2, 0, 0);
	this->ammoBox->setCurrentAnimation(0);

	this->initializeHotbarSprite(this->hotbar1, 1);
	this->initializeHotbarSprite(this->hotbar2, 2);
	this->initializeHotbarSprite(this->hotbar3, 3);
	this->initializeHotbarSprite(this->hotbar4, 4);
	this->initializeHotbarSprite(this->hotbar5, 5);
}

GameplayUI::~GameplayUI()
{
	delete this->dialogueBoxSprite;
	delete this->dialoguePromptSprite;
	delete this->ammoBox;
	delete this->hotbar1;
	delete this->hotbar2;
	delete this->hotbar3;
	delete this->hotbar4;
	delete this->hotbar5;
}

void GameplayUI::initializeHotbarSprite(Sprite *(&s), int n)
{
	s = NULL;

	switch(n)
	{
	case 1:
		s = new Sprite("images/HotbarPipe.png");
		break;
	case 2:
		s = new Sprite("images/HotbarPistol.png");
		break;
	case 3:
		s = new Sprite("images/HotbarRifle.png");
		break;
	case 4:
		s = new Sprite("images/HotbarGrenade.png");
		break;
	case 5:
		s = new Sprite("images/HotbarHamburger.png");
		break;
	}

	if(s == NULL)
		assert(false);

	s->initialize(4, 100, 80, 18 + ((n-1)*100), 18, 3);
	s->addSpriteAnimFrame(0, 0, 240);
	s->addSpriteAnimFrame(1, 0, 160);
	s->addSpriteAnimFrame(2, 0, 80);
	s->addSpriteAnimFrame(3, 0, 0);
	s->setCurrentAnimation(0);
}

void GameplayUI::draw()
{
	// draw ammo box
	if(Player::getInstance()->getCurrentWeapon() != NULL && !Player::getInstance()->getCurrentWeapon()->isMelee())
		this->ammoBox->draw();

	// draw hotbar icons
	this->hotbar1->draw();
	this->hotbar2->draw();
	this->hotbar3->draw();
	this->hotbar4->draw();
	this->hotbar5->draw();

	Game::setTexturing(false);
	{
		// draw hotbar text
		DrawPrimitives::setColor(0, 0, 0);
		DrawPrimitives::drawText(InputManager::getBindingText(InputManager::getInstance()->getBinding(HOTBAR_1)), this->hotbar1->positionX + 5, this->hotbar1->positionY + 58);
		DrawPrimitives::drawText(InputManager::getBindingText(InputManager::getInstance()->getBinding(HOTBAR_2)), this->hotbar2->positionX + 5, this->hotbar2->positionY + 58);
		DrawPrimitives::drawText(InputManager::getBindingText(InputManager::getInstance()->getBinding(HOTBAR_3)), this->hotbar3->positionX + 5, this->hotbar3->positionY + 58);
		DrawPrimitives::drawText(InputManager::getBindingText(InputManager::getInstance()->getBinding(HOTBAR_4)), this->hotbar4->positionX + 5, this->hotbar4->positionY + 58);
		DrawPrimitives::drawText(InputManager::getBindingText(InputManager::getInstance()->getBinding(HOTBAR_5)), this->hotbar5->positionX + 5, this->hotbar5->positionY + 58);
	}
	Game::setTexturing(true);

	// draw dialogue if it has been set
	if(this->dialogueIndex < this->currentDialogue.size())
	{
		this->dialogueBoxSprite->draw();

		this->dialoguePromptSprite->draw();
	}

	Game::setTexturing(false);
	{
		/* draw health bar */
		int rectWidth;

		float healthPercent = Player::getInstance()->getHealth() / 100.f;

		rectWidth = 200 * (healthPercent);

		if(rectWidth < 0)
			rectWidth = 0;

		DrawPrimitives::setColor(1.0 * (1.f - healthPercent), 1.0 * (healthPercent), 0);

		DrawPrimitives::drawRectangle(true, 18, Game::stateInfo.screenHeight - 38, rectWidth, 20, 0);

		DrawPrimitives::setColor(0, 0, 0);
		DrawPrimitives::drawRectangle(false, 18, Game::stateInfo.screenHeight - 38, 200, 20, 0);	

		// draw energy bar
		float energy = Player::getInstance()->getEnergy() / 100.f;
		rectWidth = 200 * energy;

		if(rectWidth < 0)
			rectWidth = 0;

		DrawPrimitives::setColor(1, 1, 0);

		DrawPrimitives::drawRectangle(true, 18, Game::stateInfo.screenHeight - 53, rectWidth, 10, 0);

		DrawPrimitives::setColor(0, 0, 0);
		DrawPrimitives::drawRectangle(false, 18, Game::stateInfo.screenHeight - 53, 200, 10, 0);


		/* draw score text */
		if(Player::getInstance()->getHealth() == 0)
			DrawPrimitives::setColor(1,0,0);
		else
			DrawPrimitives::setColor(1,1,1);

		std::string score = "Score: ";
		std::stringstream ss;

		ss << score << Player::getInstance()->getScore();

		DrawPrimitives::drawText(ss.str(), Game::stateInfo.screenWidth - 150, Game::stateInfo.screenHeight - 38);

		// draw ammo text
		if(Player::getInstance()->getCurrentWeapon() != NULL && !Player::getInstance()->getCurrentWeapon()->isMelee())
		{
			std::string magAmmo = GenericMethods::intToString(Player::getInstance()->getCurrentWeapon()->getMagazineAmmo());
			std::string reserveAmmo = GenericMethods::intToString(Player::getInstance()->getCurrentWeapon()->getReserveAmmo());
			std::string maxAmmo = GenericMethods::intToString(Player::getInstance()->getCurrentWeapon()->getMaxAmmo());
		
			DrawPrimitives::setColor(0.27, 0.27, 0.27);

			if(Player::getInstance()->getCurrentWeapon() == Player::getInstance()->getPistol() || Player::getInstance()->getCurrentWeapon() == Player::getInstance()->getRifle())
				DrawPrimitives::drawText(magAmmo + " / " + reserveAmmo, this->ammoBox->positionX + 35, this->ammoBox->positionY + 15);
			else
				DrawPrimitives::drawText(magAmmo + " / " + maxAmmo, this->ammoBox->positionX + 35, this->ammoBox->positionY + 15);
		}

		// draw dialogue if it has been set
		if(this->dialogueIndex < this->currentDialogue.size())
		{
			DrawPrimitives::setColor(0,0,0);

			int cursorX = this->dialogueBoxSprite->positionX + 10;
			int cursorY = this->dialogueBoxSprite->positionY + this->dialogueBoxSprite->getHeight() - (TEXT_SPACING + 6);

			for(std::vector<std::string>::iterator it = this->currentDialogue[this->dialogueIndex].begin(); it != this->currentDialogue[this->dialogueIndex].end(); it++)
			{
				DrawPrimitives::drawText((*it), cursorX, cursorY);
				cursorY -= TEXT_SPACING;
			}
		}
	}
	Game::setTexturing(true);

	// draw map if map key is down
	if(InputManager::getInstance()->isKeyDownForAction(MAP))
	{
		StateManager::getCurrentLevel()->drawMap();
	}
}

void GameplayUI::update(double dt)
{
	InputManager* input = InputManager::getInstance();

	// update ammo display animation
	if(Player::getInstance()->getCurrentWeapon() == Player::getInstance()->getHamburger())
		this->ammoBox->setCurrentAnimation(0);
	else if(Player::getInstance()->getCurrentWeapon() == Player::getInstance()->getPistol() || Player::getInstance()->getCurrentWeapon() == Player::getInstance()->getRifle())
		this->ammoBox->setCurrentAnimation(1);
	else if(Player::getInstance()->getCurrentWeapon() == Player::getInstance()->getGrenade())
		this->ammoBox->setCurrentAnimation(2);

	// advance dialogue if skip key is down
	if(this->spaceUp && this->dialogueIndex < this->currentDialogue.size() && input->isKeyDownForAction(SKIP))
	{
		this->spaceUp = false;
		this->advanceDialogue();
	}
	else if(!input->isKeyDownForAction(SKIP))
	{
		this->spaceUp = true;
	}

	// dialogue prompt animation
	if(this->dialogueIndex < this->currentDialogue.size() && this->promptAnimTimer < 1000)
		promptAnimTimer += dt;
	else if(this->promptAnimTimer >= 1000)
	{
		this->promptAnimTimer = 0;
		this->dialoguePromptSprite->nextFrame();
	}

	// set hotbar animation
	this->hotbar1->setCurrentAnimation(2);

	if(Player::getInstance()->hasPistol())
		this->hotbar2->setCurrentAnimation(2);
	else
		this->hotbar2->setCurrentAnimation(0);

	if(Player::getInstance()->hasRifle())
		this->hotbar3->setCurrentAnimation(2);
	else
		this->hotbar3->setCurrentAnimation(0);

	if(Player::getInstance()->getGrenade()->getMagazineAmmo() > 0)
		this->hotbar4->setCurrentAnimation(2);
	else
		this->hotbar4->setCurrentAnimation(0);

	if(Player::getInstance()->getHamburger()->getMagazineAmmo() > 0)
		this->hotbar5->setCurrentAnimation(2);
	else
		this->hotbar5->setCurrentAnimation(0);

	if(input->isKeyDownForAction(HOTBAR_3))
		this->selectedHotbar = 3;
	else if(input->isKeyDownForAction(HOTBAR_2))
		this->selectedHotbar = 2;
	else if(input->isKeyDownForAction(HOTBAR_1))
		this->selectedHotbar = 1;
	else if(input->isKeyDownForAction(HOTBAR_4))
		this->selectedHotbar = 4;
	else if(input->isKeyDownForAction(HOTBAR_5))
		this->selectedHotbar = 5;

	switch(this->selectedHotbar)
	{
	case 1:
		this->hotbar1->currentAnimation++;
		break;
	case 2:
		this->hotbar2->currentAnimation++;
		break;
	case 3:
		this->hotbar3->currentAnimation++;
		break;
	case 4:
		this->hotbar4->currentAnimation++;
		break;
	case 5:
		this->hotbar5->currentAnimation++;
		break;
	default:
		assert(false);
	}
}

void GameplayUI::setDialogue(std::string fileName)
{
	std::ifstream file;
	std::string line;
	int index = 0;

	this->currentDialogue.clear();
	this->dialogueIndex = 0;

	file.open(fileName);

	while(std::getline(file, line))
	{
		if(this->currentDialogue.size() == 0)
			this->currentDialogue.push_back(std::vector<std::string>());

		if(line != "#BREAK#")
		{
			// replace action tags with corresponding keybinds
			for(unsigned i = 0; i < ACTIONS_END; i++)
			{
				std::string tag = InputManager::actionTags[i];
				char c = InputManager::getInstance()->getBinding((Actions)i);

				line = GenericMethods::replace(line, tag, InputManager::getInstance()->getBindingText(c), true);
			}

			this->currentDialogue[index].push_back(line);
		}
		else
		{
			this->currentDialogue.push_back(std::vector<std::string>());
			index++;
		}
	}
}

void GameplayUI::advanceDialogue()
{
	this->dialogueIndex++;

	if(this->dialogueIndex >= this->currentDialogue.size())
		StateManager::getInstance()->getGameplayState()->setPaused(false);
}

int GameplayUI::getSelectedHotbar()
{
	return this->selectedHotbar;
}

void GameplayUI::setSelectedHotbar(int n)
{
	assert(n > 0 && n < 6);

	this->selectedHotbar = n;
}