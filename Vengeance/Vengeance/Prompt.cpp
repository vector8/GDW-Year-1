#include "Prompt.h"
#include "Game.h"

Prompt::Prompt() : response(NOT_SET), visible(false), promptText(NULL)
{
	this->initialize();
}

Prompt::~Prompt()
{
	delete this->bg;
	this->bg = NULL;

	delete this->yesBtn;
	this->yesBtn = NULL;

	delete this->noBtn;
	this->noBtn = NULL;
}

void Prompt::initialize()
{
	Sprite *yesBtnSprite, *noBtnSprite;

	this->bg = new Sprite("images/prompt_box.png");
	this->bg->initialize(1, 400, 160, 200, 220, 2);
	this->bg->addSpriteAnimFrame(0, 0, 0);

	yesBtnSprite = new Sprite("images/YesNoButton.png");
	noBtnSprite = new Sprite("images/YesNoButton.png");

	yesBtnSprite->initialize(3, 64, 32, 300, 235, 3);
	noBtnSprite->initialize(3, 64, 32, 436, 235, 3);

	Button::initializeSpriteAnimationsForButtons(yesBtnSprite);
	Button::initializeSpriteAnimationsForButtons(noBtnSprite);

	this->yesBtn = new Button("Yes", yesBtnSprite, "yesBtn");
	this->noBtn = new Button("No", noBtnSprite, "noBtn");

	this->yesBtn->addButtonListener(this);
	this->noBtn->addButtonListener(this);
}

void Prompt::draw()
{
	this->bg->draw();
	this->yesBtn->draw();
	this->noBtn->draw();

	Game::setTexturing(false);
	{
		DrawPrimitives::setColor(1, 1, 1);
		for(unsigned i = 0; i < this->promptText.size(); i++)
		{
			DrawPrimitives::drawText(this->promptText[i], 215, 350 - i*35);
		}
	}
	Game::setTexturing(true);
}

void Prompt::update(double dt)
{
	this->yesBtn->update(dt);
	this->noBtn->update(dt);
}

Responses Prompt::getResponse()
{
	return this->response;
}

bool Prompt::isVisible()
{
	return this->visible;
}

void Prompt::reset()
{
	this->response = NOT_SET;
}

void Prompt::setPromptText(std::string promptText)
{
	this->promptText.clear();
	this->promptText = GenericMethods::split(promptText, "\n");
}

void Prompt::setVisible(bool v)
{
	this->visible = v;
}

void Prompt::buttonClicked(ButtonEvent* be)
{
	if(be->getCommand() == "yesBtn")
	{
		this->response = YES;
	}
	else if(be->getCommand() == "noBtn")
	{
		this->response = NO;
	}
}