#include "LevelTransitionState.h"
#include "StateManager.h"
#include "DrawPrimitives.h"

LevelTransitionState::LevelTransitionState(Level* l) : time(l->getTime()), kills(l->getKills()), shots(l->getShots()), damage(l->getDamageTaken()), levelFileName(l->getFileName())
{
	this->initialize();
}

LevelTransitionState::~LevelTransitionState()
{
	delete this->background;
	this->background = NULL;

	delete this->continueBtn;
	this->continueBtn = NULL;

	delete this->levelNameSprite;
	this->levelNameSprite = NULL;
	delete this->completeSprite;
	this->completeSprite = NULL;
	delete this->timeSprite;
	this->timeSprite = NULL;
	delete this->killsSprite;
	this->killsSprite = NULL;
	delete this->shotsSprite;
	this->shotsSprite = NULL;
	delete this->damageTakenSprite;
	this->damageTakenSprite = NULL;
}

void LevelTransitionState::initialize()
{
	Sprite* contBtnSprite = new Sprite("images/ContinueButton.png");
	// initialize background
	this->background = new StaticBackground("images/StatscreenBackground.png",  800, 600);

	// initialize sprites
	// TODO add levelname field to Level class
	if(this->levelFileName == "levels/acrewood.lvl")
	{
		this->levelNameSprite = new Sprite("images/Label_Map_Acrewood.png");
	}
	else if(this->levelFileName == "levels/downtown.lvl")
	{
		this->levelNameSprite = new Sprite("images/Label_Map_Downtown.png");
	}
	else if(this->levelFileName == "levels/industrial.lvl")
	{
		this->levelNameSprite = new Sprite("images/Label_Map_Industrial.png");
	}
	else if(this->levelFileName == "levels/undercitym1.lvl" || this->levelFileName == "levels/undercitym2.lvl")
	{
		this->levelNameSprite = new Sprite("images/Label_Map_Undercity.png");
	}
	else if(this->levelFileName == "levels/warehouse.lvl")
	{
		this->levelNameSprite = new Sprite("images/Label_Map_Warehouse.png");
	}

	this->completeSprite = new Sprite("images/Label_Complete.png");
	this->timeSprite = new Sprite("images/Label_Time.png");
	this->killsSprite = new Sprite("images/Label_Kills.png");
	this->shotsSprite = new Sprite("images/Label_Shots.png");
	this->damageTakenSprite = new Sprite("images/Label_Damage.png");
	
	this->levelNameSprite->initialize(1, 600, 64, 100, 511, 2);
	this->completeSprite->initialize(1, 256, 112, 415, 456, 2);
	this->timeSprite->initialize(1, 300, 64, 100, 366, 2);
	this->killsSprite->initialize(1, 300, 64, 100, 284, 2);
	this->shotsSprite->initialize(1, 300, 64, 100, 202, 2);
	this->damageTakenSprite->initialize(1, 300, 64, 100, 120, 2);

	contBtnSprite->initialize(3, 248, 64, 502, 50, 2);

	this->levelNameSprite->addSpriteAnimFrame(0, 0, 0);
	this->completeSprite->addSpriteAnimFrame(0, 0, 0);
	this->timeSprite->addSpriteAnimFrame(0, 0, 0);
	this->killsSprite->addSpriteAnimFrame(0, 0, 0);
	this->shotsSprite->addSpriteAnimFrame(0, 0, 0);
	this->damageTakenSprite->addSpriteAnimFrame(0, 0, 0);

	Button::initializeSpriteAnimationsForButtons(contBtnSprite);

	// initialize buttons
	this->continueBtn = new Button(contBtnSprite, "continueBtn");

	this->continueBtn->addButtonListener(this);
}

void LevelTransitionState::draw()
{
	this->background->draw();

	this->levelNameSprite->draw();
	this->completeSprite->draw();
	this->timeSprite->draw();
	this->killsSprite->draw();
	this->shotsSprite->draw();
	this->damageTakenSprite->draw();

	this->continueBtn->draw();

	Game::setTexturing(false);
	{
		DrawPrimitives::setColor(1, 1, 1);

		int minutes = (this->time/1000)/60, seconds = (this->time/1000)%60;
		std::string secString = GenericMethods::intToString(seconds);

		if(seconds < 10)
			secString = "0" + secString;

		std::string timeFull = GenericMethods::intToString(minutes) + ":" + secString;

		DrawPrimitives::drawText(timeFull, 600, this->timeSprite->positionY + 32);
		DrawPrimitives::drawText(GenericMethods::intToString(this->kills), 600, this->killsSprite->positionY + 32);
		DrawPrimitives::drawText(GenericMethods::intToString(this->shots), 600, this->shotsSprite->positionY + 32);
		DrawPrimitives::drawText(GenericMethods::intToString(this->damage), 600, this->damageTakenSprite->positionY + 32);
	}
	Game::setTexturing(true);
}

void LevelTransitionState::update(double dt)
{
	this->continueBtn->update(dt);
}

void LevelTransitionState::buttonClicked(ButtonEvent* be)
{
	if(be->getCommand() == "continueBtn")
	{
		StateManager::getInstance()->setCurrentState(GAMEPLAY);
	}
}