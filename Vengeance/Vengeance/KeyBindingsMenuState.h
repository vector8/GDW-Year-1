#pragma once

#include "State.h"
#include "Button.h"
#include "StaticBackground.h"
#include <vector>
#include "IButtonListener.h"
#include "IKeyListener.h"
#include "IMouseListener.h"
#include "InputManager.h"

class KeyBindingsMenuState : public State, public IButtonListener, public IKeyListener, public IMouseListener
{
private:
	static const std::string keybindLabels[ACTIONS_END];

	StaticBackground* background;
	std::vector<Button*> buttons;
	Sprite* labelSprite;
	int currentBindClicked;

	bool ignoreBtnClick;
	bool active;

	void initialize();

	void volumeDown();
	void volumeUp();
	void keyBinds();
	void back();

public:
	KeyBindingsMenuState();
	~KeyBindingsMenuState();

	void draw();
	void update(double dt);

	void buttonClicked(ButtonEvent* be);

	void setActive(bool b);

	bool acceptingKeyEvents();
	void keyEventOccurred(KeyEvent* ke);

	void mouseClicked(MouseClickEvent* mce);
};