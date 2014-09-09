#include "CreditsState.h"
#include "StateManager.h"
#include "InputManager.h"

CreditsState::CreditsState()
{
	this->creditsSprite = new Sprite("images/Credits.png");
	
	this->creditsSprite->initialize(1, 800, 2200, 0, -1600, 2);
	this->creditsSprite->addSpriteAnimFrame(0, 0, 0);

	InputManager::getInstance()->addKeyListener(this);

	this->active = false;
}

CreditsState::~CreditsState()
{
	delete this->creditsSprite;
	this->creditsSprite = NULL;
}

void CreditsState::draw()
{
	this->creditsSprite->draw();
}

void CreditsState::update(double dt)
{
	double dy = 0;
	const double TIMER_LIMIT = 5000;

	if(this->initialTimer < TIMER_LIMIT)
		this->initialTimer += dt;

	if(this->initialTimer >= TIMER_LIMIT && this->creditsSprite->positionY < 0)
	{
		dy = 50;
		this->creditsSprite->positionY += (dy * (dt / 1000));

		if(this->creditsSprite->positionY > 0)
			this->creditsSprite->positionY = 0;
	}
}

void CreditsState::reset()
{
	this->initialTimer = 0;
	this->creditsSprite->positionY = -1600;
	this->active = true;
}

bool CreditsState::acceptingKeyEvents()
{
	return this->active;
}

void CreditsState::keyEventOccurred(KeyEvent* ke)
{
	if(ke->getState())
	{
		this->active = false;
		StateManager::getInstance()->setCurrentState(MAIN_MENU);
	}
}