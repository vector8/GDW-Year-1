#include "EndingState.h"
#include "StateManager.h"

EndingState::EndingState()
{
	this->initialize();
}

EndingState::~EndingState()
{
	delete this->background;
	delete this->burnItBtn;
	delete this->takeItBtn;
	delete this->continueBtn;
}

void EndingState::initialize()
{
	Sprite *burnItSprite;
	Sprite *takeItSprite;
	Sprite *continueSprite;

	// initialize background
	this->background = new StaticBackground("images/EndingPrechoice.png",  800, 600);

	// initialize sprites
	burnItSprite = new Sprite("images/EndgameBurnButton.png");
	takeItSprite = new Sprite("images/EndgameTakeButton.png");
	continueSprite = new Sprite("images/ContinueButton.png");
	burnItSprite->initialize(3, 248, 64, 30, 50, 2);
	takeItSprite->initialize(3, 248, 64, 522, 50, 2);
	continueSprite->initialize(3, 248, 64, 522, 50, 2);
	Button::initializeSpriteAnimationsForButtons(burnItSprite);
	Button::initializeSpriteAnimationsForButtons(takeItSprite);
	Button::initializeSpriteAnimationsForButtons(continueSprite);

	// initialize buttons
	this->burnItBtn = new Button(burnItSprite, "burnItBtn");
	this->takeItBtn = new Button(takeItSprite, "takeItBtn");
	this->continueBtn = new Button(continueSprite, "continueBtn");
	this->burnItBtn->addButtonListener(this);
	this->takeItBtn->addButtonListener(this);
	this->continueBtn->addButtonListener(this);

	this->continueBtn->setVisible(false);
} 

void EndingState::draw()
{
	this->background->draw();
	this->burnItBtn->draw();
	this->takeItBtn->draw();
	this->continueBtn->draw();
}

void EndingState::update(double dt)
{
	this->burnItBtn->update(dt);
	this->takeItBtn->update(dt);
	this->continueBtn->update(dt);
}

void EndingState::buttonClicked(ButtonEvent* be)
{
	if(be->getCommand() == "burnItBtn")
	{
		delete this->background;
		this->background = new StaticBackground("images/EndingBurn.png",  800, 600);
		this->continueBtn->setVisible(true);
		this->burnItBtn->setVisible(false);
		this->takeItBtn->setVisible(false);
	}
	else if(be->getCommand() == "takeItBtn")
	{
		delete this->background;
		this->background = new StaticBackground("images/EndingTake.png",  800, 600);
		this->continueBtn->setVisible(true);
		this->burnItBtn->setVisible(false);
		this->takeItBtn->setVisible(false);
	}
	else if(be->getCommand() == "continueBtn")
	{
		StateManager::getInstance()->setCurrentState(CREDITS);
	}
}