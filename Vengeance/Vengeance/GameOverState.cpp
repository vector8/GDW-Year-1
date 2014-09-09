#include "GameOverState.h"
#include "StateManager.h"

GameOverState::GameOverState()
{
	this->initialize();
}

GameOverState::~GameOverState()
{
	delete this->background;
	delete this->mainMenuBtn;
}

void GameOverState::initialize()
{
	Sprite *mainMenuSprite;

	// initialize background
	this->background = new StaticBackground("images/GameOverBackground.png",  800, 600);

	// initialize sprite
	mainMenuSprite = new Sprite("images/MainMenuButton.png");
	mainMenuSprite->initialize(3, 248, 64, 15, 50, 2);
	Button::initializeSpriteAnimationsForButtons(mainMenuSprite);

	// initialize button
	this->mainMenuBtn = new Button(mainMenuSprite, "mainMenuBtn");
	this->mainMenuBtn->addButtonListener(this);
} 

void GameOverState::draw()
{
	this->background->draw();
	this->mainMenuBtn->draw();
}

void GameOverState::update(double dt)
{
	this->mainMenuBtn->update(dt);
}

void GameOverState::buttonClicked(ButtonEvent* be)
{
	if(be->getCommand() == "mainMenuBtn")
	{
		StateManager::getInstance()->setCurrentState(MAIN_MENU);
	}
}