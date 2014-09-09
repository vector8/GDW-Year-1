#include "SplashScreenState.h"
#include "StateManager.h"
#include "SoundManager.h"
#include "InputManager.h"

SplashScreenState::SplashScreenState() : timer(0)
{
	this->splashSprite = new Sprite("images/SplashScreen.png");
	this->splashSprite->initialize(2, 800, 600, 0, 0, 0);
	this->splashSprite->addSpriteAnimFrame(0, 0, 0);
	this->splashSprite->addSpriteAnimFrame(1, 0, 600);

	this->splashSprite->setCurrentAnimation(0);

	InputManager::getInstance()->addKeyListener(this);
}

SplashScreenState::~SplashScreenState()
{
	delete this->splashSprite;
	this->splashSprite = NULL;

	InputManager::getInstance()->removeKeyListener(this);
}

void SplashScreenState::draw()
{
	this->splashSprite->draw();
}

void SplashScreenState::update(double dt)
{
	if(this->timer < SPLASH_FLIP_TIME)
		this->timer += dt;

	if(this->splashSprite->currentAnimation == 0 && this->timer >= SPLASH_FLIP_TIME)
	{
		this->splashSprite->setCurrentAnimation(1);

		SoundManager::getInstance()->playMusic(MAIN_THEME, true);
	}
}

bool SplashScreenState::acceptingKeyEvents()
{
	return true;
}

void SplashScreenState::keyEventOccurred(KeyEvent* ke)
{
	if(this->timer >= SPLASH_FLIP_TIME && ke->getState())
	{
		StateManager::getInstance()->setCurrentState(MAIN_MENU);
	}
}