#include "InputManager.h"
#include <cstdlib>
#include <fstream>
#include <vector>
#include "GenericMethods.h"
#include <assert.h>
#include <windows.h>
#include "Game.h"

const std::string InputManager::actionTags[ACTIONS_END] = 
{
	"#MOVE_UP#", "#MOVE_DOWN#", "#MOVE_LEFT#", "#MOVE_RIGHT#", "#ATTACK#", "#INTERACT#", "#SPRINT#", "#RELOAD#", 
	"#SKIP#", "#PRONE#", "#HOTBAR_1#", "#HOTBAR_2#", "#HOTBAR_3#", "#HOTBAR_4#", "#HOTBAR_5#", "#MAP#"
};

InputManager::InputManager() : leftMouseDown(false), rightMouseDown(false), middleMouseDown(false),	mouseX(0), mouseY(0), clickX(0), clickY(0)
{
	for(unsigned i = 0; i < NUM_CHARS; i++)
	{
		this->keyStates[i] = false;
	}

	for(unsigned i = 0; i < ACTIONS_END; i++)
	{
		this->actionBindings[i] = NULL;
	}

	this->loadBindings("settings/controls.cfg");
}

InputManager::~InputManager(){}

void InputManager::loadBindings(std::string fileName)
{
	std::ifstream file;
	std::string line;

	file.open(fileName);

	while(std::getline(file, line))
	{
		if(line.size() > 0)
		{
			std::vector<std::string> tokens;

			tokens = GenericMethods::split(line, " ");

			assert(tokens.size() == 2);

			int action, bind;

			action = GenericMethods::stringToInt(tokens[0]);
			bind = GenericMethods::stringToInt(tokens[1]);

			this->actionBindings[action] = (unsigned char)bind;
		}
	}
}

void InputManager::raiseKeyEvent(KeyEvent* e)
{
	for(std::list<IKeyListener*>::iterator it = this->keyListeners.begin(); it != this->keyListeners.end(); it++)
	{
		if((*it) != NULL && (*it)->acceptingKeyEvents())
			(*it)->keyEventOccurred(e);
	}
}

void InputManager::raiseMouseClickEvent(MouseClickEvent* e)
{
	for(std::list<IMouseListener*>::iterator it = this->mouseListeners.begin(); it != this->mouseListeners.end(); it++)
	{
		(*it)->mouseClicked(e);
	}
}

InputManager* InputManager::getInstance()
{
	static InputManager* instance = new InputManager();

	return instance;
}

bool InputManager::isKeyDownForAction(Actions a)
{
	char key = actionBindings[a];

	switch(key)
	{
	case NULL:
		return false;
	case LEFT_MOUSE_BUTTON:
		return this->leftMouseDown;
	case MIDDLE_MOUSE_BUTTON:
		return this->middleMouseDown;
	case RIGHT_MOUSE_BUTTON:
		return this->rightMouseDown;
	default:
		return this->keyStates[key];
	}
}

void InputManager::resetBindingsToDefault()
{
	this->loadBindings("settings/default_controls.cfg");
}

char InputManager::getBinding(Actions a)
{
	return this->actionBindings[a];
}

void InputManager::setBinding(Actions a, unsigned char c)
{
	for(unsigned i = 0; i < ACTIONS_END; i++)
	{
		if(i != a && this->actionBindings[i] == c)
		{
			// TODO display prompt?
			this->actionBindings[i] = NULL;
		}
	}

	this->actionBindings[a] = c;
}

void InputManager::saveBindings()
{
	std::ofstream file;

	file.open("settings/controls.cfg");

	for(unsigned i = 0; i < ACTIONS_END; i++)
	{
		file << i << " " << (int)this->actionBindings[i] << std::endl;
	}
}

std::string InputManager::getBindingText(unsigned char c)
{
	c = toupper(c);

	switch(c)
	{
	case NULL:
		return "<Not Bound>";
	case LEFT_MOUSE_BUTTON:
		return "Left Click";
	case MIDDLE_MOUSE_BUTTON:
		return "Middle Click";
	case RIGHT_MOUSE_BUTTON:
		return "Right Click";
	case VK_SHIFT:
		return "Shift";
	case VK_CONTROL:
		return "Ctrl";
	case VK_RETURN:
		return "Enter";
	case VK_SPACE:
		return "Space";
	case VK_BACK:
		return "Backspace";
	case VK_TAB:
		return "Tab";
	case VK_UP:
		return "Up";
	case VK_DOWN:
		return "Down";
	case VK_LEFT:
		return "Left";
	case VK_RIGHT:
		return "Right";
	case VK_DELETE:
		return "Delete";
	default:
		std::stringstream ss;
		ss << c;
		return ss.str();
	}
}

void InputManager::setModifierKeyState(unsigned char c, bool state)
{
	if(c == VK_CONTROL || c == VK_SHIFT)
	{
		if(this->keyStates[c] != state)
		{
			this->keyStates[c] = state;

			KeyEvent* e = new KeyEvent(c, state);

			this->raiseKeyEvent(e);
		}
	}
	else
		assert(false);
}

void InputManager::setKeyState(unsigned char c, bool state)
{
	if(this->keyStates[VK_CONTROL] && c >= 1 && c <= 26)
	{
		c += 64;
	}

	if(this->keyStates[c] != state)
	{
		this->keyStates[c] = state;

		KeyEvent* e = new KeyEvent(c, state);

		this->raiseKeyEvent(e);
	}
}

/* keyboardDown()
   - this gets called when you press a key down
   - you are given the key that was pressed
     and where the (x,y) location of the mouse is when pressed
*/
void InputManager::keyboardDown(unsigned char c, int mouseX, int mouseY)
{
	c = toupper(c);

	this->setKeyState(c, true);
}

/* keyboardUp()
   - this gets called when you lift a key up
   - you are given the key that was pressed
     and where the (x,y) location of the mouse is when pressed
*/
void InputManager::keyboardUp(unsigned char c, int mouseX, int mouseY)
{
	c = toupper(c);

	this->setKeyState(c, false);
}

/*
 * mouseClicked
 * - this function is called when the mouse is clicked and it handles the 
 *   input state managment
 */
void InputManager::mouseClicked(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN) 
	{
		switch(button)
		{
		case GLUT_LEFT_BUTTON:
			this->leftMouseDown = true;
			break;
		case GLUT_MIDDLE_BUTTON:
			this->middleMouseDown = true;
			break;
		case GLUT_RIGHT_BUTTON:
			this->rightMouseDown = true;
			break;
		}
		this->clickX = x * Game::stateInfo.ratioWidth;
		this->clickY = (Game::stateInfo.windowHeight - y) * Game::stateInfo.ratioHeight;

		MouseClickEvent* e = new MouseClickEvent(button, this->clickX, this->clickY);
		this->raiseMouseClickEvent(e);
	}
	else
	{
		switch(button)
		{
		case GLUT_LEFT_BUTTON:
			this->leftMouseDown = false;
			break;
		case GLUT_MIDDLE_BUTTON:
			this->middleMouseDown = false;
			break;
		case GLUT_RIGHT_BUTTON:
			this->rightMouseDown = false;
			break;
		}
	}
}

/*
 * mouseMoved(x,y)
 * - this occurs when the mouse has moved, regardless if the button is clicked.  
 *   you are given the x,y locations in window coordinates (from the top left corner) and thus 
 *   must be converted to screen coordinates using the screen to window pixels ratio
 *   and the y must be flipped to make the bottom left corner the origin.
 */
void InputManager::mouseMoved(int x, int y)
{
	/* convert from window to screen pixel coordinates */
	this->mouseX = x * Game::stateInfo.ratioWidth;
	this->mouseY = (Game::stateInfo.windowHeight - y) * Game::stateInfo.ratioHeight;
}

bool InputManager::isLeftMouseDown()
{
	return this->leftMouseDown;
}

bool InputManager::isMiddleMouseDown()
{
	return this->leftMouseDown;
}

bool InputManager::isRightMouseDown()
{
	return this->leftMouseDown;
}

int InputManager::getMouseX()
{
	return this->mouseX;
}

int InputManager::getMouseY()
{
	return this->mouseY;
}

int InputManager::getClickX()
{
	return this->clickX;
}

int InputManager::getClickY()
{
	return this->clickY;
}

void InputManager::addKeyListener(IKeyListener* l)
{
	this->keyListeners.push_back(l);
}

void InputManager::removeKeyListener(IKeyListener* l)
{
	this->keyListeners.remove(l);
}

void InputManager::addMouseListener(IMouseListener* l)
{
	this->mouseListeners.push_back(l);
}