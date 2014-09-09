#include "LoadMenuState.h"
#include "StateManager.h"
#include "SaveFileManager.h"
#include <assert.h>

LoadMenuState::LoadMenuState() : saveButtonPressed(0), needPrompt(false)
{
	this->initialize();
}

LoadMenuState::~LoadMenuState()
{
	delete this->background;
	this->background = NULL;

	for(std::vector<Button*>::iterator it = this->buttons.begin(); it != this->buttons.end(); it++)
	{
		delete (*it);
	}
}

void LoadMenuState::initialize()
{
	Sprite *save1Sprite, *save2Sprite, *save3Sprite, *noData1Sprite, *noData2Sprite, *noData3Sprite, *backSprite;
	Button *save1Btn, *save2Btn, *save3Btn, *backBtn;
	bool save1Enabled = true, save2Enabled = true, save3Enabled = true;

	// initialize background
	this->background = new StaticBackground("images/OptionsMenu_ButtonFree.png",  800, 600);

	// initialize sprites
	save1Sprite = new Sprite("images/Save01Button.png");
	save2Sprite = new Sprite("images/Save02Button.png");
	save3Sprite = new Sprite("images/Save03Button.png");
	noData1Sprite = new Sprite("images/NoDataButton.png");
	noData2Sprite = new Sprite("images/NoDataButton.png");
	noData3Sprite = new Sprite("images/NoDataButton.png");
	backSprite = new Sprite("images/BackButton.png");

	save1Sprite->initialize(3, 248, 64, 15, 420, 2);
	noData1Sprite->initialize(3, 248, 64, 15, 420, 2);

	save2Sprite->initialize(3, 248, 64, 15, 346, 2);
	noData2Sprite->initialize(3, 248, 64, 15, 346, 2);

	save3Sprite->initialize(3, 248, 64, 15, 272, 2);
	noData3Sprite->initialize(3, 248, 64, 15, 272, 2);

	backSprite->initialize(3, 248, 64, 15, 50, 2);

	Button::initializeSpriteAnimationsForButtons(save1Sprite);
	Button::initializeSpriteAnimationsForButtons(save2Sprite);
	Button::initializeSpriteAnimationsForButtons(save3Sprite);
	Button::initializeSpriteAnimationsForButtons(noData1Sprite);
	Button::initializeSpriteAnimationsForButtons(noData2Sprite);
	Button::initializeSpriteAnimationsForButtons(noData3Sprite);
	Button::initializeSpriteAnimationsForButtons(backSprite);

	// initialize buttons
	save1Btn = new Button(save1Sprite, "save1Btn");
	save1Btn->addSprite(noData1Sprite);

	save2Btn = new Button(save2Sprite, "save2Btn");
	save2Btn->addSprite(noData2Sprite);

	save3Btn = new Button(save3Sprite, "save3Btn");
	save3Btn->addSprite(noData3Sprite);

	if(SaveFileManager::getInstance()->saveExists(1))
		save1Btn->setCurrentSprite(0);
	else
	{
		save1Btn->setCurrentSprite(1);
		save1Enabled = false;
	}

	if(SaveFileManager::getInstance()->saveExists(2))
		save2Btn->setCurrentSprite(0);
	else
	{
		save2Btn->setCurrentSprite(1);
		save2Enabled = false;
	}

	if(SaveFileManager::getInstance()->saveExists(3))
		save3Btn->setCurrentSprite(0);
	else
	{
		save3Btn->setCurrentSprite(1);
		save3Enabled = false;
	}
	
	backBtn = new Button(backSprite, "backBtn");

	save1Btn->setEnabled(save1Enabled);
	save2Btn->setEnabled(save2Enabled);
	save3Btn->setEnabled(save3Enabled);

	save1Btn->addButtonListener(this);
	save2Btn->addButtonListener(this);
	save3Btn->addButtonListener(this);
	backBtn->addButtonListener(this);

	this->buttons.push_back(backBtn);
	this->buttons.push_back(save1Btn);
	this->buttons.push_back(save2Btn);
	this->buttons.push_back(save3Btn);
}

void LoadMenuState::draw()
{
	this->background->draw();

	for(std::vector<Button*>::iterator it = this->buttons.begin(); it != this->buttons.end(); it++)
	{
		(*it)->draw();
	}
}

void LoadMenuState::update(double dt)
{
	if(this->needPrompt)
	{
		switch(StateManager::getInstance()->getPromptResponse())
		{
		case NO:
			this->saveButtonPressed = 0;
			break;
		case YES:
			break;
		default:
			assert(false);
		}

		this->needPrompt = false;
	}

	if(this->saveButtonPressed > 0)
	{
		assert(SaveFileManager::getInstance()->saveExists(this->saveButtonPressed));

		// load
		SaveFileManager::getInstance()->load(this->saveButtonPressed);

		this->saveButtonPressed = 0;
	}
	else
	{
		int i = 0;

		for(std::vector<Button*>::iterator it = this->buttons.begin(); it != this->buttons.end(); it++)
		{
			if(i > 0)
			{
				if(SaveFileManager::getInstance()->saveExists(i))
				{
					(*it)->setEnabled(true);
					(*it)->setCurrentSprite(0);
				}
				else
				{
					(*it)->setEnabled(false);
					(*it)->setCurrentSprite(1);
				}
			}

			(*it)->update(dt);

			i++;
		}
	}
}

void LoadMenuState::buttonClicked(ButtonEvent* be)
{
	static const std::string PROMPT = "Any unsaved progress will be lost.\nWould you like to continue?";

	if(be->getCommand() == "backBtn")
	{
		if(StateManager::getInstance()->getGameplayState() == NULL)
			StateManager::getInstance()->setCurrentState(MAIN_MENU);
		else
			StateManager::getInstance()->setCurrentState(SAVE_LOAD);
	}
	else 
	{
		if(StateManager::getInstance()->getGameplayState() != NULL)
		{
			StateManager::getInstance()->showPrompt(PROMPT);
			this->needPrompt = true;
		}

		if(be->getCommand() == "save1Btn")
			this->saveButtonPressed = 1;
		else if(be->getCommand() == "save2Btn")
			this->saveButtonPressed = 2;
		else if(be->getCommand() == "save3Btn")
			this->saveButtonPressed = 3;
		else
			assert(false);
	}
}