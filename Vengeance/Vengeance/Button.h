#pragma once

#include "MiscGameResources.h"
#include "Sprite.h"
#include "IButtonListener.h"
#include <vector>

class Button
{
private:
	std::vector<Sprite*> sprites;
	int currentSprite;
	std::string text;
	std::string name;
	std::vector<IButtonListener*> listeners;
	bool clicked;
	bool visible;
	bool enabled;

public:
	Button(Sprite* sprite, std::string name);
	Button(std::string text, Sprite* sprite, std::string name);
	~Button();

	void draw();
	void update(double dt);

	void addButtonListener(IButtonListener* l);

	bool containsCoords(float x, float y);

	std::string getName();
	std::string getText();
	bool isEnabled();
	bool isVisible();

	void setText(std::string text);
	void setEnabled(bool e);
	void setVisible(bool v);
	void setPosition(float x, float y);

	void addSprite(Sprite* s);
	void setCurrentSprite(int index);

	int getCurrentSpriteIndex();

	static void initializeSpriteAnimationsForButtons(Sprite* s);
};