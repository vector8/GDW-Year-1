#pragma once
#include <string>
#include "IKeyListener.h"
#include "IMouseListener.h"
#include <list>

#define NUM_CHARS 256
#define LEFT_MOUSE_BUTTON 1
#define MIDDLE_MOUSE_BUTTON 2
#define RIGHT_MOUSE_BUTTON 3

enum Actions
{
	MOVE_UP,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT,
	ATTACK,
	INTERACT,
	SPRINT,
	RELOAD,
	SKIP,
	PRONE,
	HOTBAR_1,
	HOTBAR_2,
	HOTBAR_3,
	HOTBAR_4,
	HOTBAR_5,
	MAP,
	ACTIONS_END
};

class InputManager
{
private:
	char actionBindings[ACTIONS_END];
	bool keyStates[NUM_CHARS];

	bool leftMouseDown, rightMouseDown, middleMouseDown;

	int mouseX, mouseY, clickX, clickY;

	std::list<IKeyListener*> keyListeners;
	std::list<IMouseListener*> mouseListeners;

	InputManager();
	~InputManager();

	void loadBindings(std::string fileName);

	void raiseKeyEvent(KeyEvent* e);
	void raiseMouseClickEvent(MouseClickEvent* e);

public:
	static const std::string actionTags[ACTIONS_END];

	static InputManager* getInstance();

	bool isKeyDownForAction(Actions a);

	void resetBindingsToDefault();

	char getBinding(Actions a);

	void setBinding(Actions a, unsigned char c);

	void saveBindings();

	static std::string getBindingText(unsigned char c);

	void setModifierKeyState(unsigned char c, bool state);

	void setKeyState(unsigned char c, bool state);

	void keyboardDown(unsigned char c, int mouseX, int mouseY);

	void keyboardUp(unsigned char c, int mouseX, int mouseY);

	void mouseClicked(int button, int state, int x, int y);

	void mouseMoved(int x, int y);

	bool isLeftMouseDown();

	bool isMiddleMouseDown();

	bool isRightMouseDown();

	int getMouseX();

	int getMouseY();

	int getClickX();

	int getClickY();

	void addKeyListener(IKeyListener* l);
	void removeKeyListener(IKeyListener* l);

	void addMouseListener(IMouseListener* l);
};