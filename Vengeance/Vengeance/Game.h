#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include "GL/glut.h"
#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"
#include "MiscGameResources.h"
#include "Sprite.h"
#include "HorizontalScrollingBackground.h"
#include "Timer.h"
#include <algorithm>
#include <cmath>
#include <string>
#include <sstream>
#include <assert.h>
#include "Player.h"
#include "DrawPrimitives.h"
#include "Level.h"

/* 
 * Game.h
 * This is the main Game class of the base sprite lib starter code
 * it handles input, game loop update, display
 */
class Game
{
private:
	Game(void);
	~Game(void);

public:
	/* initialization */

	static Game* getInstance();

	static void setTexturing(bool on);

	//void initializeInput();
	void initializeGame();

	/* convenience functions */
	// screen size is the apparent size in pixels
	// i.e. this is what your game calculations should use
	void setScreenSize(int w, int h)
	{
		stateInfo.screenWidth = w;
		stateInfo.screenHeight = h;
	}
	// sets the screen-to-window pixel coordinates ratio
	void updateScreenRatio()
	{
		stateInfo.ratioWidth = stateInfo.screenWidth/(float)stateInfo.windowWidth;
		stateInfo.ratioHeight = stateInfo.screenHeight/(float)stateInfo.windowHeight;
		/* these are only really used for the mouse function
		   to get actual screen pixel coordinates from the window coordinates */
	}

	// sets the window size in pixels for the window itself
	void setNewWindowSize(int width,int height)
	{
		stateInfo.windowWidth = width;
		stateInfo.windowHeight = height;
	}

	/* draw/rendering routines */
	void draw(); // called from the main

	void PreDraw(); // prior to drawing
	void DrawGame(); // actual drawing the frame
	void PostDraw(); // cleanup and prepare for next frame
	
	void drawSprites(); // draw the sprite list
	void drawEntities(); // draw the entity list
	void drawTestPrimitives(); // test function

	/* update routines, the game loop */
	void update(); // called from main frequently

	/*********************************/
	/* DATA */
	/*********************************/
	
	/* game state info */
	static GameStateInfo stateInfo;

	/* timer's for rendering and animation/physics update */
	Timer *renderingTimer;
	Timer *updateTimer; // for physics/collisions etc.

	void setCursor(CursorType type);
};
