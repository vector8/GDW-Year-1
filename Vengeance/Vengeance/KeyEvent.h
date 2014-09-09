#pragma once

#include <string>

class KeyEvent
{
private:
	char key;
	bool state;

public:
	KeyEvent(unsigned char key, bool state);
	~KeyEvent();

	unsigned char getKey();
	bool getState();
};