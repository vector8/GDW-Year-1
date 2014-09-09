#include "KeyEvent.h"

KeyEvent::KeyEvent(unsigned char key, bool state) : key(key), state(state)
{}

KeyEvent::~KeyEvent()
{}

unsigned char KeyEvent::getKey()
{
	return this->key;
}

bool KeyEvent::getState()
{
	return this->state;
}