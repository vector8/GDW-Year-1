#include "SaveLoadMenuState.h"
#include "StateManager.h"

SaveLoadMenuState::SaveLoadMenuState()
{
	this->initialize();
}

SaveLoadMenuState::~SaveLoadMenuState()
{
	delete this->background;
	this->background = NULL;

	for(std::vector<Button*>::iterator it = this->buttons.begin(); it != this->buttons.end(); it++)
	{
		delete (*it);
	}
}

void SaveLoadMenuState::initialize()
{
	Sprite *saveSprite, *loadSprite, *backSprite;
	Button *saveBtn, *loadBtn, *backBtn;

	// initialize background
	this->background = new StaticBackground("images/OptionsMenu_ButtonFree.png",  800, 600);

	// initialize sprites
	saveSprite = new Sprite("images/SaveButton.png");
	loadSprite = new Sprite("images/LoadButton.png");
	backSprite = new Sprite("images/BackButton.png");

	saveSprite->initialize(3, 248, 64, 15, 420, 2);
	loadSprite->initialize(3, 248, 64, 15, 346, 2);
	backSprite->initialize(3, 248, 64, 15, 50, 2);

	Button::initializeSpriteAnimationsForButtons(saveSprite);
	Button::initializeSpriteAnimationsForButtons(loadSprite);
	Button::initializeSpriteAnimationsForButtons(backSprite);

	// initialize buttons
	saveBtn = new Button(saveSprite, "saveBtn");
	loadBtn = new Button(loadSprite, "loadBtn");
	backBtn = new Button(backSprite, "backBtn");

	saveBtn->addButtonListener(this);
	loadBtn->addButtonListener(this);
	backBtn->addButtonListener(this);

	this->buttons.push_back(saveBtn);
	this->buttons.push_back(loadBtn);
	this->buttons.push_back(backBtn);
}

void SaveLoadMenuState::draw()
{
	this->background->draw();

	for(std::vector<Button*>::iterator it = this->buttons.begin(); it != this->buttons.end(); it++)
	{
		(*it)->draw();
	}
}

void SaveLoadMenuState::update(double dt)
{
	for(std::vector<Button*>::iterator it = this->buttons.begin(); it != this->buttons.end(); it++)
	{
		(*it)->update(dt);
	}
}

void SaveLoadMenuState::buttonClicked(ButtonEvent* be)
{
	if(be->getCommand() == "backBtn")
	{
		StateManager::getInstance()->setCurrentState(MAIN_MENU);
	}
	else if(be->getCommand() == "saveBtn")
	{
		StateManager::getInstance()->setCurrentState(SAVE);
	}
	else if(be->getCommand() == "loadBtn")
	{
		StateManager::getInstance()->setCurrentState(LOAD);
	}
}