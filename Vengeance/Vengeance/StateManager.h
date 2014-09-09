#pragma once

#include "GameplayState.h"
#include "MainMenuState.h"
#include "GameOverState.h"
#include "OptionsMenuState.h"
#include "KeyBindingsMenuState.h"
#include "IKeyListener.h"
#include "LevelTransitionState.h"
#include "Prompt.h"
#include "SaveLoadMenuState.h"
#include "SaveMenuState.h"
#include "LoadMenuState.h"
#include "CreditsState.h"
#include "SplashScreenState.h"
#include "EndingState.h"

enum StateTypes
{
	NONE,
	MAIN_MENU,
	GAMEPLAY,
	OPTIONS,
	CREDITS,
	GAME_OVER,
	KEY_BINDINGS,
	TRANSITION,
	SAVE_LOAD,
	SAVE,
	LOAD,
	SPLASH_SCREEN,
	ENDING
};

class StateManager : public IKeyListener
{
private:
	static StateManager* instance;
	State* currentState;
	StateTypes currentStateType, prevStateType;
	GameplayState* gameplayState;
	OptionsMenuState* optionsMenuState;
	MainMenuState* mainMenuState;
	GameOverState* gameOverState;
	KeyBindingsMenuState* keyBindingsMenuState;
	LevelTransitionState* transitionState;
	SaveLoadMenuState* saveLoadMenuState;
	SaveMenuState* saveMenuState;
	LoadMenuState* loadMenuState;
	CreditsState* creditsState;
	SplashScreenState* splashScreenState;
	EndingState* endingState;

	Prompt* prompt;

	// private to allow for singleton
	StateManager();

	void escKeyPressed();

public:
	~StateManager();

	static StateManager* getInstance();

	static bool isInitialized();

	static Level* getCurrentLevel();

	Responses getPromptResponse();

	void draw();
	void update(double dt);

	bool acceptingKeyEvents(){return true;}

	void keyEventOccurred(KeyEvent* ke);

	void newGame();

	void loadLevel(Level* l);

	GameplayState* getGameplayState()
	{
		return this->gameplayState;
	}

	void setCurrentState(StateTypes type, Level* l = NULL);

	void showPrompt(std::string promptText);
};