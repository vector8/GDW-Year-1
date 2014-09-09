#pragma once

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define NUM_TILES_IN_SET 39
#define TILE_SIZE 96
#define SPRITE_SIZE 64
#define DOODAD_SIZE 32
#define TILES_PER_ROW 13

#ifndef SHIFTED
#define SHIFTED 0x8000
#endif

/* these are enumerated types (just names) of game states
   that the game can be in at any moment.  These help you
   manage the flow of the game.  Especially if you want 
   to do different updates (call different update functions)
   or have different behaviour for input (i.e. mouse and keyboard
   functions might have different behaviour in-game (STATE_GAMEPLAY)
   versus when in the Menu (STATE_MAINMENU)
*/
enum {
	STATE_GAMEPLAY=0, 
	STATE_MAINMENU, 
	STATE_CREDITS
};

enum CursorType
{
	NORMAL_CURSOR,
	CROSSHAIRS_CURSOR
};

//// input information related to mouse
//// you could add keyboard info in here too if you need to know if a key is currently pressed down
//typedef struct _InputInfo
//{
//	bool leftMouseDown, middleMouseDown, rightMouseDown; // is the mouse down?
//	float clickX,clickY; // where the mouse was clicked
//	float currentX,currentY;
//
//	bool w, a, s, d, e, r, space, shift, ctrl;
//}InputInfo;

// convenience structure to hold rgb values
typedef struct _RGBColor
{
	float red,green,blue;
}RGBColor;

// GameStateInfo
// this stores anything related to the state of the game
// i.e. what level are we on?, what is the score? etc.
typedef struct _GameStateInfo
{
	int gameState; // the game state, see enum's above
	RGBColor bgClearColor; // the default clear color of the background

	// window width/height are the actual size of the window
	int windowWidth, windowHeight;

	// screen width/height are the apparent size of the window
	// i.e. you can have a 640x480 game screen stretched to 1280x1024
	// your calculations for actual pixel locations will depend on the ratio
	//   between screen and window width/height
	int screenWidth, screenHeight;

	float ratioWidth,ratioHeight;
}GameStateInfo;