#include "MainMenuState.h"
#include "StateManager.h"
#include "SoundManager.h"
#include <assert.h>

MainMenuState::MainMenuState() : quitPressed(false)
{
	this->initialize();
}

MainMenuState::~MainMenuState()
{
	delete this->background;
	this->background = NULL;

	for(std::vector<Button*>::iterator it = this->buttons.begin(); it != this->buttons.end(); it++)
	{
		delete (*it);
	}
}

void MainMenuState::initialize()
{
	Sprite *continueBtnSprite, *newBtnSprite, *loadSaveBtnSprite, *loadBtnSprite, *optionsBtnSprite, *creditsBtnSprite, *quitBtnSprite;

	// initialize background
	this->background = new StaticBackground("images/MainMenu_ButtonFree.png",  800, 600);

	// initialize sprites
	continueBtnSprite = new Sprite("images/ContinueButton.png");
	newBtnSprite = new Sprite("images/NewButton.png");
	loadSaveBtnSprite = new Sprite("images/LoadSaveButton.png");
	loadBtnSprite = new Sprite("images/LoadButton.png");
	optionsBtnSprite = new Sprite("images/OptionsButton.png");
	creditsBtnSprite = new Sprite("images/CreditsButton.png");
	quitBtnSprite = new Sprite("images/QuitButton.png");

	Sprite* buttonSprites[] = {continueBtnSprite, newBtnSprite, loadSaveBtnSprite, optionsBtnSprite, creditsBtnSprite, quitBtnSprite};

	for(int i = 0; i < 6; i++)
	{
		buttonSprites[i]->setNumberOfAnimations(3);
		buttonSprites[i]->setSpriteFrameSize(248, 64);
		buttonSprites[i]->setPosition(15, 600 - (i * 74) - 180);
		buttonSprites[i]->setLayerID(2);

		Button::initializeSpriteAnimationsForButtons(buttonSprites[i]);
	}

	loadBtnSprite->setNumberOfAnimations(3);
	loadBtnSprite->setSpriteFrameSize(248, 64);
	loadBtnSprite->setPosition(loadSaveBtnSprite->positionX, loadSaveBtnSprite->positionY);
	loadBtnSprite->setLayerID(2);
	Button::initializeSpriteAnimationsForButtons(loadBtnSprite);

	// initialize buttons
	continueBtn = new Button(continueBtnSprite, "continueBtn");
	newBtn = new Button(newBtnSprite, "newBtn");
	loadSaveBtn = new Button(loadSaveBtnSprite, "loadSaveBtn");
	loadBtn = new Button(loadBtnSprite, "loadBtn");
	optionsBtn = new Button(optionsBtnSprite, "optionsBtn");
	creditsBtn = new Button(creditsBtnSprite, "creditsBtn");
	quitBtn = new Button(quitBtnSprite, "quitBtn");

	continueBtn->addButtonListener(this);
	newBtn->addButtonListener(this);
	loadSaveBtn->addButtonListener(this);
	loadBtn->addButtonListener(this);
	optionsBtn->addButtonListener(this);
	creditsBtn->addButtonListener(this);
	quitBtn->addButtonListener(this);

	this->buttons.push_back(continueBtn);
	this->buttons.push_back(newBtn);
	this->buttons.push_back(loadSaveBtn);
	this->buttons.push_back(loadBtn);
	this->buttons.push_back(optionsBtn);
	this->buttons.push_back(creditsBtn);
	this->buttons.push_back(quitBtn);
}

void MainMenuState::draw()
{
	this->background->draw();

	for(std::vector<Button*>::iterator it = this->buttons.begin(); it != this->buttons.end(); it++)
	{
		(*it)->draw();
	}
}

void MainMenuState::update(double dt)
{
	if(this->quitPressed)
	{
		switch(StateManager::getInstance()->getPromptResponse())
		{
		case YES:
			exit(1);
			break;
		case NO:
			this->quitPressed = false;
			break;
		default:
			assert(false);
		}
	}
	else
	{
		this->loadBtn->setVisible(!StateManager::isInitialized() || StateManager::getInstance()->getGameplayState() == NULL);
		this->continueBtn->setVisible(StateManager::isInitialized() && StateManager::getInstance()->getGameplayState() != NULL);
		this->loadSaveBtn->setVisible(StateManager::isInitialized() && StateManager::getInstance()->getGameplayState() != NULL);

		int i = 0;

		for(std::vector<Button*>::iterator it = this->buttons.begin(); it != this->buttons.end(); it++)
		{
			if((*it)->isVisible() && (*it)->getName() != "quitBtn")
			{
				(*it)->setPosition(15, 600 - (i * 74) - 180);
				
				i++;
			}
			else if((*it)->getName() == "quitBtn")
				(*it)->setPosition(15, 50);

			(*it)->update(dt);
		}
	}
}

void MainMenuState::buttonClicked(ButtonEvent* be)
{
	if(be->getCommand() == "quitBtn")
	{
		if(StateManager::getCurrentLevel() != NULL)
		{
			StateManager::getInstance()->showPrompt("Any unsaved progress will be lost.\nWould you like to continue?");
			this->quitPressed = true;
		}
		else
		{
			exit(1);
		}
	}
	else if(be->getCommand() == "newBtn")
	{
		StateManager::getInstance()->newGame();
	}
	else if(be->getCommand() == "optionsBtn")
	{
		StateManager::getInstance()->setCurrentState(OPTIONS);
	}
	else if(be->getCommand() == "loadSaveBtn")
	{
		StateManager::getInstance()->setCurrentState(SAVE_LOAD);
	}
	else if(be->getCommand() == "loadBtn")
	{
		StateManager::getInstance()->setCurrentState(LOAD);
	}
	else if(be->getCommand() == "continueBtn")
	{
		StateManager::getInstance()->setCurrentState(GAMEPLAY);
	}
	else if(be->getCommand() == "creditsBtn")
	{
		StateManager::getInstance()->setCurrentState(CREDITS);
	}
}