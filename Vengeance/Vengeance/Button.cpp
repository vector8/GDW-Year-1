#include "Button.h"
#include "InputManager.h"
#include "Game.h"

Button::Button(Sprite* sprite, std::string name) : name(name), text(""), visible(true), enabled(true), currentSprite(0)
{
	this->sprites.push_back(sprite);
	this->clicked = false;
}

Button::Button(std::string text, Sprite* sprite, std::string name) : text(text), name(name), visible(true), enabled(true), currentSprite(0)
{
	this->sprites.push_back(sprite);
	this->clicked = false;
}

Button::~Button()
{
	this->sprites.clear();
}

void Button::update(double dt)
{
	if(this->enabled && this->visible)
	{
		InputManager* input = InputManager::getInstance();

		if(input->isLeftMouseDown() && this->containsCoords(input->getClickX(), input->getClickY()))
		{
			this->sprites[this->currentSprite]->setCurrentAnimation(2);
			this->clicked = true;
		}
		else if(this->containsCoords(input->getMouseX(), input->getMouseY()))
		{
			if(this->clicked && !input->isLeftMouseDown())
			{
				this->clicked = false;

				// click
				ButtonEvent* e = new ButtonEvent(this);
				for(std::vector<IButtonListener*>::iterator it = this->listeners.begin(); it != this->listeners.end(); it++)
				{
					(*it)->buttonClicked(e);
				}
			}

			this->sprites[this->currentSprite]->setCurrentAnimation(1);
		}
		else
		{
			this->sprites[this->currentSprite]->setCurrentAnimation(0);
		}
	}
}

void Button::draw()
{
	if(this->visible)
	{
		this->sprites[this->currentSprite]->draw();

		if(this->text.size() > 0)
		{
			Game::setTexturing(false);

			DrawPrimitives::setColor(1, 1, 1);
			//if(this->text.size() >= (this->sprites[this->currentSprite]->getWidth() / 2) / 6)
			//	DrawPrimitives::drawText(this->text, this->sprites[this->currentSprite]->positionX + 5, this->sprites[this->currentSprite]->positionY + this->sprites[this->currentSprite]->getHeight() * 0.25);
			//else
			DrawPrimitives::drawText(this->text, this->sprites[this->currentSprite]->positionX + (this->sprites[this->currentSprite]->getWidth() / 2) - (this->text.size() * 6), this->sprites[this->currentSprite]->positionY + this->sprites[this->currentSprite]->getHeight() * 0.30);

			Game::setTexturing(true);
		}
	}
}

void Button::addButtonListener(IButtonListener* l)
{
	this->listeners.push_back(l);
}

bool Button::containsCoords(float x, float y)
{
	return x >= this->sprites[this->currentSprite]->positionX && x <= (this->sprites[this->currentSprite]->positionX + this->sprites[this->currentSprite]->getWidth()) && 
		y >= this->sprites[this->currentSprite]->positionY && y <= (this->sprites[this->currentSprite]->positionY + this->sprites[this->currentSprite]->getHeight());
}

std::string Button::getName()
{
	return this->name;
}

std::string Button::getText()
{
	return this->text;
}

bool Button::isEnabled()
{
	return this->enabled;
}

bool Button::isVisible()
{
	return this->visible;
}

void Button::setText(std::string text)
{
	this->text = text;
}

void Button::setEnabled(bool e)
{
	this->enabled = e;
}

void Button::setVisible(bool v)
{
	this->visible = v;
}

void Button::setPosition(float x, float y)
{
	this->sprites[this->currentSprite]->setPosition(x, y);
}

void Button::addSprite(Sprite* s)
{
	this->sprites.push_back(s);
}

void Button::setCurrentSprite(int index)
{
	assert((unsigned) index < this->sprites.size());

	this->currentSprite = index;
}

int Button::getCurrentSpriteIndex()
{
	return this->currentSprite;
}

void Button::initializeSpriteAnimationsForButtons(Sprite* s)
{
	for(int i = 0; i < s->numberOfAnimations; i++)
	{
		s->addSpriteAnimFrame(i, 0, (s->numberOfAnimations - i - 1) * s->getHeight());
	}
}