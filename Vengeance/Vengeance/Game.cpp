#include "Game.h"
#include "StateManager.h"
#include "InputManager.h"

GameStateInfo Game::stateInfo;

/* this is called by std::sort to sort the list based on layerID 
 *  for drawing in the proper order 
 */
bool spriteSortingFunction(Sprite *s1, Sprite *s2)
{
	// return true if s1's layerID is less than s2's layerID
	return (s1->layerID < s2->layerID);
}

bool entitySortingFunction(Entity *e1, Entity *e2)
{
	// return true if s1's layerID is less than s2's layerID
	return (e1->getLayerID() < e2->getLayerID());
}

/* constructor */
Game::Game(void)
{
	/* green background */
	stateInfo.bgClearColor.red = 0;
	stateInfo.bgClearColor.green = 0;
	stateInfo.bgClearColor.blue = 0;

	/* init state */
	stateInfo.gameState = STATE_GAMEPLAY;
	renderingTimer = new Timer("RENDER");
	updateTimer = new Timer("UPDATE");

	// init input
	//initializeInput();
}

/* destructor */
Game::~Game(void)
{
	/* deallocate memory and clean up here. if needed */
}

Game* Game::getInstance()
{
	static Game g;

	return &g;
}

void Game::setTexturing(bool on)
{
	if(on)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);
}

//void Game::initializeInput()
//{
//	input.a = false;
//	input.d = false;
//	input.w = false;
//	input.s = false;
//	input.e = false;
//	input.r = false;
//	input.space = false;
//	input.shift = false;
//	input.ctrl = false;
//}

/* 
 * initializeGame()
 * - this function is called in the constructor to initialize everything related
 *   to the game, i..e loading sprites etc.
 * - MUST be called prior to any drawing/updating (you should add in checks to ensure this occurs in the right order)
 */
void Game::initializeGame()
{	
}

/* draw()
 * - this gets called automatically about 30 times per second
 * - this function just draws the sprites 
 */
void Game::draw()
{
	/* pre-draw - setup the rendering */
	PreDraw();

	/* draw - actually render to the screen */
	DrawGame();
	
	/* post-draw - after rendering, setup the next frame */
	PostDraw();
}

/*
 * Pre-Draw() is for setting up things that need to happen in order to draw
 *    the game, i.e. sorting, splitting things into appropriate lists etc.
 */
void Game::PreDraw()
{
	/* clear the screen */
	glViewport(0,0,stateInfo.windowWidth,stateInfo.windowHeight);
	glClearColor(stateInfo.bgClearColor.red, 
				 stateInfo.bgClearColor.green, 
				 stateInfo.bgClearColor.blue, 0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity(); // clear out the transformation matrix
	glEnable(GL_TEXTURE_2D); // turn on texturing

	// if we update our timer it will tell us the elapsed time since the previous 
	// frame that we rendered
	renderingTimer->tick();
}

/* 
 * DrawGame()
 *  - this is the actual drawing of the current frame of the game.
 */
void Game::DrawGame()
{
	/* here is where your drawing stuff goes */
	StateManager::getInstance()->draw();

	glDisable(GL_TEXTURE_2D);

	/* this makes it actually show up on the screen */
	glutSwapBuffers();
}

/*
 * PostDraw()
 *  - in here you should clean up and set up things for the next frame
 *  - i.e. once I've used my assets, I can change them to set up things for
 *    the next frame, usually just memory management or setting up game state 
 *    boolean values etc.  
 */
void Game::PostDraw()
{
	// nothing here at the moment
}

/* update()
  - this function is essentially the game loop
    it gets called often and as such you
	don't actually need a "loop" to define the game
	it happens behind the scenes
  - think of this function as one iteration of the game loop
  - if you need to update physics calculations, sprite animation info,
    or sound etc, it goes in here
*/
void Game::update()
{
	// update our clock so we have the delta time since the last update
	updateTimer->tick();

	// check modifier keys and update input, since they dont trigger keyboard events
	SHORT keyState;
	bool state;
	
	keyState = GetKeyState(VK_SHIFT);
	if(keyState & SHIFTED)
		state = true;
	else
		state = false;
	InputManager::getInstance()->setModifierKeyState(VK_SHIFT, state);

	keyState = GetKeyState(VK_CONTROL);
	if(keyState & SHIFTED)
		state = true;
	else
		state = false;
	InputManager::getInstance()->setModifierKeyState(VK_CONTROL, state);

	/* you should probably update all of the sprites in a list just like the drawing */
	/* maybe two lists, one for physics updates and another for sprite animation frame update */

	StateManager::getInstance()->update(updateTimer->getElapsedTimeMS());
}

void Game::setCursor(CursorType type)
{
	switch(type)
	{
	case CROSSHAIRS_CURSOR:
		glutSetCursor(GLUT_CURSOR_CROSSHAIR);
		break;
	case NORMAL_CURSOR:
	default:
		glutSetCursor(GLUT_CURSOR_INHERIT);
		break;
	}
}