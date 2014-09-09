#include "KeyBindingsMenuState.h"
#include "StateManager.h"
#include "DrawPrimitives.h"

const std::string KeyBindingsMenuState::keybindLabels[ACTIONS_END] = 
{
	"Move Up", "Move Down", "Move Left", "Move Right", "Attack", "Interact", "Sprint", "Reload", 
	"Advance Dialogue", "Prone", "Hotbar 1", "Hotbar 2", "Hotbar 3", "Hotbar 4", "Hotbar 5", "Map"
};

KeyBindingsMenuState::KeyBindingsMenuState() : currentBindClicked(-1), ignoreBtnClick(false)
{
	this->initialize();

	InputManager::getInstance()->addKeyListener(this);
	InputManager::getInstance()->addMouseListener(this);

	this->active = false;
}

KeyBindingsMenuState::~KeyBindingsMenuState()
{
	delete this->background;
	this->background = NULL;

	for(std::vector<Button*>::iterator it = this->buttons.begin(); it != this->buttons.end(); it++)
	{
		delete (*it);
	}
}

void KeyBindingsMenuState::initialize()
{
	Sprite *backSprite, *resetSprite;
	Button* keybindButtons[ACTIONS_END];
	Button *backBtn, *resetBtn;

	// initialize background
	this->background = new StaticBackground("images/OptionsMenu_ButtonFree.png",  800, 600);

	// initialize sprites
	backSprite = new Sprite("images/BackButton.png");
	resetSprite = new Sprite("images/DefaultsButton.png");

	labelSprite = new Sprite("images/KeybindLabels.png");

	backSprite->initialize(3, 248, 64, 15, 50, 2);
	resetSprite->initialize(3, 248, 64, 273, 50, 2);

	labelSprite->initialize(ACTIONS_END, 194, 32, 0, 0, 2);

	Button::initializeSpriteAnimationsForButtons(backSprite);
	Button::initializeSpriteAnimationsForButtons(resetSprite);

	for(int i = 0; i < labelSprite->numberOfAnimations; i++)
	{
		labelSprite->addSpriteAnimFrame(i, 0, (labelSprite->numberOfAnimations - 1 - i) * labelSprite->getHeight());
		
		Sprite* s = new Sprite("images/KeybindGenericButton.png");
		if(i < 10)
			s->initialize(3, 144, 32, 236, 456 - (i * 35), 2);
		else
			s->initialize(3, 144, 32, 636, 456 - ((i - 10) * 35), 2);
		Button::initializeSpriteAnimationsForButtons(s);

		keybindButtons[i] = new Button(s, "bindBtn" + GenericMethods::intToString(i));
		keybindButtons[i]->addButtonListener(this);
		this->buttons.push_back(keybindButtons[i]);
	}

	// initialize buttons
	backBtn = new Button(backSprite, "backBtn");
	resetBtn = new Button(resetSprite, "resetBtn");

	backBtn->addButtonListener(this);
	resetBtn->addButtonListener(this);

	this->buttons.push_back(backBtn);
	this->buttons.push_back(resetBtn);
}

void KeyBindingsMenuState::draw()
{
	this->background->draw();

	// draw labels
	/*for(int i = 0; i < this->labelSprite->numberOfAnimations; i++)
	{
		this->labelSprite->setCurrentAnimation(i);
		this->labelSprite->setPosition(20, 600 - (i * (labelSprite->getHeight() + 3) + 144));
		this->labelSprite->draw();
	}*/

	// draw buttons
	for(std::vector<Button*>::iterator it = this->buttons.begin(); it != this->buttons.end(); it++)
	{
		(*it)->draw();
	}

	Game::setTexturing(false);
	{
		// draw button text & label text
		for(int i = 0; i < ACTIONS_END; i++)
		{
			char c = InputManager::getInstance()->getBinding((Actions)i);

			std::string bindingText;
		
			if(this->currentBindClicked == i)
				bindingText = "";
			else
				bindingText = InputManager::getBindingText(c);
		
			DrawPrimitives::setColor(1, 1, 1);
			if(i < 10)
			{
				// button text
				if(bindingText.size() >= 11)
					DrawPrimitives::drawText(bindingText, 240, 464 - (i * 35));
				else if(bindingText.size() > 0)
					DrawPrimitives::drawText(bindingText, 240 + ((11 - bindingText.size()) * 6), 464 - (i * 35));

				// label text
				DrawPrimitives::drawText(this->keybindLabels[i], 20, 464 - (i * 35));
			}
			else
			{
				if(bindingText.size() >= 11)
					DrawPrimitives::drawText(bindingText, 640, 464 - ((i - 10) * 35));
				else if(bindingText.size() > 0)
					DrawPrimitives::drawText(bindingText, 640 + ((11 - bindingText.size()) * 6), 464 - ((i - 10) * 35));

				DrawPrimitives::drawText(this->keybindLabels[i], 470, 464 - ((i - 10) * 35));
			}

		}
	}
	Game::setTexturing(true);
}

void KeyBindingsMenuState::update(double dt)
{
	for(std::vector<Button*>::iterator it = this->buttons.begin(); it != this->buttons.end(); it++)
	{
		(*it)->update(dt);
	}

	if(!InputManager::getInstance()->isLeftMouseDown())
		this->ignoreBtnClick = false;
}

void KeyBindingsMenuState::buttonClicked(ButtonEvent* be)
{
	if(!this->ignoreBtnClick)
	{
		if(be->getCommand() == "backBtn")
		{
			InputManager::getInstance()->saveBindings();
			StateManager::getInstance()->setCurrentState(OPTIONS);
			this->active = false;
		}
		else if(be->getCommand() == "saveBtn")
		{
			InputManager::getInstance()->saveBindings();
		}
		else if(be->getCommand() == "resetBtn")
		{
			InputManager::getInstance()->resetBindingsToDefault();
		}
		else if(be->getCommand().substr(0, 7) == "bindBtn")
		{
			std::string bindNo = be->getCommand().substr(7, be->getCommand().size() - 7);

			this->currentBindClicked = GenericMethods::stringToInt(bindNo);
		}
		else
			assert(false);
	}
}

void KeyBindingsMenuState::setActive(bool b)
{
	this->active = b;
}

bool KeyBindingsMenuState::acceptingKeyEvents()
{
	return this->active;
}

void KeyBindingsMenuState::keyEventOccurred(KeyEvent* ke)
{
	if(ke->getState())
	{
		if(ke->getKey() == VK_ESCAPE)
		{
			if(this->currentBindClicked == -1)
			{
				StateManager::getInstance()->setCurrentState(OPTIONS);
				this->active = false;
			}
			else
				this->currentBindClicked = -1;
		}
		else if(this->currentBindClicked >= 0)
		{
			InputManager::getInstance()->setBinding((Actions) this->currentBindClicked, ke->getKey());
			this->currentBindClicked = -1;
		}
	}
}

void KeyBindingsMenuState::mouseClicked(MouseClickEvent* mce)
{
	if(this->currentBindClicked >= 0)
	{
		if(this->buttons[this->currentBindClicked]->containsCoords(mce->getClickX(), mce->getClickY()))
		{
			InputManager::getInstance()->setBinding((Actions) this->currentBindClicked, (unsigned char)(mce->getButton() + 1));

			this->ignoreBtnClick = true;
		}

		this->currentBindClicked = -1;
	}
}