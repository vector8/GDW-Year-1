#include "OptionsMenuState.h"
#include "StateManager.h"

OptionsMenuState::OptionsMenuState()
{
	this->initialize();
}

OptionsMenuState::~OptionsMenuState()
{
	delete this->background;
	this->background = NULL;

	for(std::vector<Button*>::iterator it = this->buttons.begin(); it != this->buttons.end(); it++)
	{
		delete (*it);
	}
}

void OptionsMenuState::initialize()
{
	Sprite *keyBindsSprite, *backSprite;
	Button *keyBindsBtn, *backBtn;

	// initialize background
	this->background = new StaticBackground("images/OptionsMenu_ButtonFree.png",  800, 600);

	// initialize sprites
	keyBindsSprite = new Sprite("images/KeyBindButton.png");
	backSprite = new Sprite("images/BackButton.png");

	keyBindsSprite->initialize(3, 248, 64, 15, 420, 2);
	backSprite->initialize(3, 248, 64, 15, 50, 2);

	Button::initializeSpriteAnimationsForButtons(keyBindsSprite);
	Button::initializeSpriteAnimationsForButtons(backSprite);

	// initialize buttons
	keyBindsBtn = new Button(keyBindsSprite, "keyBindsBtn");
	backBtn = new Button(backSprite, "backBtn");

	keyBindsBtn->addButtonListener(this);
	backBtn->addButtonListener(this);

	this->buttons.push_back(keyBindsBtn);
	this->buttons.push_back(backBtn);
}

void OptionsMenuState::draw()
{
	this->background->draw();

	for(std::vector<Button*>::iterator it = this->buttons.begin(); it != this->buttons.end(); it++)
	{
		(*it)->draw();
	}
}

void OptionsMenuState::update(double dt)
{
	for(std::vector<Button*>::iterator it = this->buttons.begin(); it != this->buttons.end(); it++)
	{
		(*it)->update(dt);
	}
}

void OptionsMenuState::buttonClicked(ButtonEvent* be)
{
	if(be->getCommand() == "backBtn")
	{
		StateManager::getInstance()->setCurrentState(MAIN_MENU);
	}
	else if(be->getCommand() == "keyBindsBtn")
	{
		StateManager::getInstance()->setCurrentState(KEY_BINDINGS);
	}
}