#include "StateManager.h"
#include "Game.h"
#include "InputManager.h"
#include "SoundManager.h"

StateManager* StateManager::instance = NULL;

StateManager::StateManager()
{
	this->mainMenuState = NULL;
	this->gameplayState = NULL;
	this->optionsMenuState = NULL;
	this->gameOverState = NULL;
	this->keyBindingsMenuState = NULL;
	this->transitionState = NULL;
	this->saveLoadMenuState = NULL;
	this->saveMenuState = NULL;
	this->loadMenuState = NULL;
	this->creditsState = NULL;
	this->splashScreenState = new SplashScreenState();
	this->endingState = NULL;

	this->prompt = NULL;

	InputManager::getInstance()->addKeyListener(this);

	this->currentStateType = NONE;
	this->setCurrentState(SPLASH_SCREEN);
}

StateManager::~StateManager()
{
	delete this->gameplayState;
	this->gameplayState = NULL;
	delete this->mainMenuState;
	this->mainMenuState = NULL;
	delete this->gameOverState;
	this->gameOverState = NULL;
	delete this->keyBindingsMenuState;
	this->keyBindingsMenuState = NULL;
	delete this->currentState;
	this->currentState = NULL;
	delete this->saveLoadMenuState;
	this->saveLoadMenuState = NULL;
	delete this->saveMenuState;
	this->saveMenuState = NULL;
	delete this->loadMenuState;
	this->loadMenuState = NULL;
	delete this->creditsState;
	this->creditsState = NULL;
}

void StateManager::escKeyPressed()
{
	switch(currentStateType)
	{
	case MAIN_MENU:
		if(this->gameplayState == NULL)
		{
			// TODO show prompt to exit
			exit(1);
		}
		else
		{
			this->setCurrentState(GAMEPLAY);
		}
		break;
	case GAMEPLAY:
	case GAME_OVER:
	case OPTIONS:
	case SAVE_LOAD:
		this->setCurrentState(MAIN_MENU);
		break;
	case SAVE:
		this->setCurrentState(SAVE_LOAD);
		break;
	case LOAD:
		if(this->getGameplayState() == NULL)
			this->setCurrentState(MAIN_MENU);
		else
			this->setCurrentState(SAVE_LOAD);
		break;
	default:
		break;
	}
}

StateManager* StateManager::getInstance()
{
	if(instance == NULL)
	{
		instance = new StateManager();
	}

	return instance;
}

bool StateManager::isInitialized()
{
	return instance != NULL;
}

Level* StateManager::getCurrentLevel()
{
	if(StateManager::getInstance()->getGameplayState() != NULL)
		return StateManager::getInstance()->getGameplayState()->getCurrentLevel();
	else
		return NULL;
}

Responses StateManager::getPromptResponse()
{
	Responses result = this->prompt->getResponse();

	assert(result != NOT_SET);

	return result;
}

void StateManager::draw()
{
	this->currentState->draw();

	if(this->prompt && this->prompt->isVisible())
		this->prompt->draw();
}

void StateManager::update(double dt)
{
	if(this->currentStateType == MAIN_MENU && this->splashScreenState != NULL)
	{
		delete this->splashScreenState;
		this->splashScreenState = NULL;
	}

	if(this->prompt && this->prompt->isVisible())
	{
		this->prompt->update(dt);
		if(this->prompt->getResponse() != NOT_SET)
			this->prompt->setVisible(false);
	}
	else
		this->currentState->update(dt);
}

void StateManager::keyEventOccurred(KeyEvent* ke)
{
	if(ke->getKey() == VK_ESCAPE && ke->getState())
	{
		this->escKeyPressed();
	}
}

void StateManager::newGame()
{
	if(this->gameplayState)
	{
		delete this->gameplayState;
		this->gameplayState = NULL;
	}

	Player::getInstance()->resetPlayer();

	this->gameplayState = new GameplayState();
	this->gameplayState->setLevel(new Level("levels/acrewood.lvl"));

	this->setCurrentState(GAMEPLAY);
}

void StateManager::loadLevel(Level* l)
{
	assert(l != NULL);

	if(this->gameplayState == NULL)
		this->gameplayState = new GameplayState();

	this->gameplayState->setLevel(l);

	this->setCurrentState(GAMEPLAY);
}

void StateManager::setCurrentState(StateTypes type, Level* l)
{
	this->prevStateType = this->currentStateType;
	this->currentStateType = type;

	switch(type)
	{
	case MAIN_MENU:
		if(this->prevStateType == NONE || this->prevStateType == GAMEPLAY || this->prevStateType == CREDITS || this->prevStateType == TRANSITION || this->prevStateType == GAME_OVER)
		{
			if(this->prevStateType == GAME_OVER)
			{
				delete this->gameplayState;
				this->gameplayState = NULL;
			}
			if(this->endingState != NULL)
			{
				delete this->endingState;
				this->endingState = NULL;
			}

			SoundManager::getInstance()->stopMusic();
			SoundManager::getInstance()->playMusic(MAIN_THEME, true);
		}
		else if(this->prevStateType == SPLASH_SCREEN && this->mainMenuState == NULL)
			this->mainMenuState = new MainMenuState();

		this->currentState = this->mainMenuState;
		Game::getInstance()->setCursor(NORMAL_CURSOR);
		break;
	case GAMEPLAY:
		SoundManager::getInstance()->stopMusic();
		if(!this->gameplayState)
			this->gameplayState = new GameplayState();

		this->currentState = this->gameplayState;
		Game::getInstance()->setCursor(CROSSHAIRS_CURSOR);
		SoundManager::getInstance()->playMusic(this->gameplayState->getCurrentLevel()->getMusic(), true);
		break;
	case OPTIONS:
		if(!this->optionsMenuState)
			this->optionsMenuState = new OptionsMenuState();
		this->currentState = this->optionsMenuState;
		Game::getInstance()->setCursor(NORMAL_CURSOR);
		break;
	case CREDITS:
		if(!this->creditsState)
			this->creditsState = new CreditsState();
		SoundManager::getInstance()->stopMusic();
		this->creditsState->reset();
		this->currentState = this->creditsState;
		if(this->prevStateType == ENDING)
		{
			delete this->gameplayState;
			this->gameplayState = NULL;
		}
		Game::getInstance()->setCursor(NORMAL_CURSOR);
		SoundManager::getInstance()->playMusic(CREDITS_THEME, true);
		break;
	case GAME_OVER:
		if(!this->gameOverState)
			this->gameOverState = new GameOverState();
		this->currentState = this->gameOverState;
		Game::getInstance()->setCursor(NORMAL_CURSOR);
		break;
	case KEY_BINDINGS:
		if(!this->keyBindingsMenuState)
			this->keyBindingsMenuState = new KeyBindingsMenuState();
		this->currentState = this->keyBindingsMenuState;
		this->keyBindingsMenuState->setActive(true);
		Game::getInstance()->setCursor(NORMAL_CURSOR);
		break;
	case TRANSITION:
		assert(l != NULL);
		if(this->transitionState != NULL)
			delete this->transitionState;
		this->transitionState = new LevelTransitionState(l);
		this->currentState = this->transitionState;
		Game::getInstance()->setCursor(NORMAL_CURSOR);
		break;
	case SAVE_LOAD:
		if(!this->saveLoadMenuState)
			this->saveLoadMenuState = new SaveLoadMenuState();
		this->currentState = this->saveLoadMenuState;
		Game::getInstance()->setCursor(NORMAL_CURSOR);
		break;
	case SAVE:
		if(!this->saveMenuState)
			this->saveMenuState = new SaveMenuState();
		this->currentState = this->saveMenuState;
		Game::getInstance()->setCursor(NORMAL_CURSOR);
		break;
	case LOAD:
		if(!this->loadMenuState)
			this->loadMenuState = new LoadMenuState();
		this->currentState = this->loadMenuState;
		Game::getInstance()->setCursor(NORMAL_CURSOR);
		break;
	case SPLASH_SCREEN:
		this->currentState = this->splashScreenState;
		Game::getInstance()->setCursor(NORMAL_CURSOR);
		break;
	case ENDING:
		this->endingState = new EndingState();
		this->currentState = this->endingState;
		Game::getInstance()->setCursor(NORMAL_CURSOR);
		break;
	default:
		break;
	}
}

void StateManager::showPrompt(std::string promptText)
{
	if(this->prompt == NULL)
	{
		this->prompt = new Prompt();
		this->prompt->setVisible(false);
	}

	this->prompt->reset();
	this->prompt->setPromptText(promptText);
	this->prompt->setVisible(true);
}