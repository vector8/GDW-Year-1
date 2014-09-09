#pragma once

#include "MiscGameResources.h"
#include "Sprite.h"
#include "Button.h"
#include "IButtonListener.h"

enum Responses
{
	NOT_SET,
	YES,
	NO
};

class Prompt : public IButtonListener
{
private:
	Sprite* bg;
	std::vector<std::string> promptText;
	Responses response;

	Button *yesBtn, *noBtn;

	bool visible;

public:
	Prompt();
	~Prompt();

	void initialize();

	void draw();
	void update(double dt);

	Responses getResponse();
	bool isVisible();

	void reset();
	void setPromptText(std::string promptText);
	//void setButtonText(std::string btn1Text, std::string btn2Text);
	void setVisible(bool v);

	void Prompt::buttonClicked(ButtonEvent* be);
};