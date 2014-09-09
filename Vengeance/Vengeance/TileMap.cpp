#include "TileMap.h"
#include "Game.h"
#include "InputManager.h"
#include <assert.h>

TileMap::TileMap(std::string fileName, int width, int height, std::string tileset) : fileName(fileName), width(width), height(height), cameraX(0), cameraY(0), dx(0), dy(0)
{
	this->tileSetSprite = new Sprite("images/" + tileset);
	this->initializeTileSetSprite();

	// initialize tiles array
	this->tiles = new int* [width];

	for(int i = 0; i < width; i++)
	{
		this->tiles[i] = new int[height];
	}

	this->loadMap();
}

TileMap::~TileMap()
{
	delete this->tileSetSprite;

	delete [] tiles;
}

void TileMap::initializeTileSetSprite()
{
	this->tileSetSprite->setNumberOfAnimations(NUM_TILES_IN_SET);
	this->tileSetSprite->setSpriteFrameSize(TILE_SIZE, TILE_SIZE);
	this->tileSetSprite->setLayerID(0);

	const int NUM_ROWS = NUM_TILES_IN_SET / TILES_PER_ROW;

	for(int i = 0; i < NUM_TILES_IN_SET; i++)
	{
		// column number is i % TILES_PER_ROW
		// row number is NUM_ROWS - (i / TILES_PER_ROW) - 1 ==> subtract from NUM_ROWS due to bottom-right origin in OpenGL
		this->tileSetSprite->addSpriteAnimFrame(i, (i % TILES_PER_ROW) * TILE_SIZE, (NUM_ROWS - (i / TILES_PER_ROW) - 1) * TILE_SIZE);
	}
	
}

void TileMap::loadMap()
{
	std::ifstream file;
	std::string line;

	int count = this->height;

	file.open(fileName);

	while(std::getline(file, line))
	{
		assert(line.size() == this->width);
		assert(count >= 0);

		for(unsigned i = 0; i < line.size(); i++)
		{
 			tiles[i][count] = this->getTileNumberFromCharacter(line[i]);
		}

		count--;
	}

	file.close();
}

void TileMap::update(float dt)
{
	// modify scroll speed according to input
	//this->setScrollSpeed();

	this->drawList.clear();

	//this->cameraX += (dt / 1000.0) * dx;
	//this->cameraY += (dt / 1000.0) * dy;

	//if(cameraX < 0)
		//cameraX = 0;
	//else if(cameraX >= ((this->width * TILE_SIZE) - Game::stateInfo.screenWidth))
		//cameraX = (this->width * TILE_SIZE) - Game::stateInfo.screenWidth - 1;

	//if(cameraY < 0)
		//cameraY = 0;
	//else if(cameraY >= ((this->height * TILE_SIZE) - Game::stateInfo.screenHeight))
		//cameraY = (this->height * TILE_SIZE) - Game::stateInfo.screenHeight - 1;

	this->cameraX = Player::getInstance()->getWorldCoords().first - 400 + 32;
	this->cameraY = Player::getInstance()->getWorldCoords().second - 300 + 32;

	int firstTileX, lastTileX, firstTileY, lastTileY;
	firstTileX = cameraX / TILE_SIZE;
	firstTileY = cameraY / TILE_SIZE;
	lastTileX = ((cameraX + Game::stateInfo.screenWidth) / TILE_SIZE) + 1;
	lastTileY = ((cameraY + Game::stateInfo.screenHeight) / TILE_SIZE) + 1;

	if(firstTileX < 0)
		firstTileX = 0;
	if(firstTileY < 0)
		firstTileY = 0;
	if(lastTileX > this->width)
		lastTileX = this->width;
	if(lastTileY > this->height)
		lastTileY = this->height;

	for(int i = firstTileX; i < lastTileX; i++)
	{
		for(int j = firstTileY; j < lastTileY; j++)
		{
			TileTypeWithCoords t;
			t.type = tiles[i][j];
			t.x = (i * TILE_SIZE) - (int)cameraX;
			t.y = (j * TILE_SIZE) - (int)cameraY;

			this->drawList.push_back(t);
		}
	}
}

void TileMap::draw()
{
	for(std::vector<TileTypeWithCoords>::iterator it = drawList.begin(); it != drawList.end(); it++)
	{
		TileTypeWithCoords t = (*it);
		this->tileSetSprite->setCurrentAnimation(t.type);
		this->tileSetSprite->setPosition(t.x, t.y);
		this->tileSetSprite->draw();
	}
}

void TileMap::setScrollSpeed()
{
	InputManager* input = InputManager::getInstance();

	if(input->isKeyDownForAction(MOVE_RIGHT) && !input->isKeyDownForAction(MOVE_LEFT))
	{
		if(Player::getInstance()->getSprite()->positionX + SPRITE_SIZE > (4 * Game::stateInfo.screenWidth / 5))
		{
			this->setHorizontalScrollSpeed(Player::getInstance()->getSpeed());
		}
		else
		{
			this->setHorizontalScrollSpeed(0);
		}
	}
	else if(input->isKeyDownForAction(MOVE_LEFT) && !input->isKeyDownForAction(MOVE_RIGHT))
	{
		if(Player::getInstance()->getSprite()->positionX < (Game::stateInfo.screenWidth / 5))
		{
			this->setHorizontalScrollSpeed(-1 * Player::getInstance()->getSpeed());
		}
		else
		{
			this->setHorizontalScrollSpeed(0);
		}
	}
	else
	{
		this->setHorizontalScrollSpeed(0);
	}

	if(input->isKeyDownForAction(MOVE_UP) && !input->isKeyDownForAction(MOVE_DOWN))
	{
		if(Player::getInstance()->getSprite()->positionY + SPRITE_SIZE > (4 * Game::stateInfo.screenHeight / 5))
		{
			this->setVerticalScrollSpeed(Player::getInstance()->getSpeed());
		}
		else
		{
			this->setVerticalScrollSpeed(0);
		}
	}
	else if(input->isKeyDownForAction(MOVE_DOWN) && !input->isKeyDownForAction(MOVE_UP))
	{
		if(Player::getInstance()->getSprite()->positionY < (Game::stateInfo.screenHeight / 5))
		{
			this->setVerticalScrollSpeed(-1 * Player::getInstance()->getSpeed());
		}
		else
		{
			this->setVerticalScrollSpeed(0);
		}
	}
	else
	{
			this->setVerticalScrollSpeed(0);
	}
}

int TileMap::getTileNumberFromCharacter(char c)
{
	//assert(((int) c) >= 48);

	return ((int) c) - 48;
}

void TileMap::setHorizontalScrollSpeed(float dx)
{
	this->dx = dx;
}

void TileMap::setVerticalScrollSpeed(float dy)
{
	this->dy = dy;
}

std::pair<float, float> TileMap::getWorldCoordsOfCamera()
{
	return std::pair<float, float>(this->cameraX, this->cameraY);
}

void TileMap::setWorldCoordsOfCamera(std::pair<float, float> coords)
{
	this->cameraX = coords.first;
	this->cameraY = coords.second;
}

std::pair<float, float> TileMap::convertWorldCoordsToScreenCoords(std::pair<float, float> worldCoords)
{
	return std::pair<float, float>(worldCoords.first - this->cameraX, worldCoords.second - this->cameraY);
}

int TileMap::getWidthTiles()
{
	return this->width;
}

int TileMap::getHeightTiles()
{
	return this->height;
}

int TileMap::getWidthPixels()
{
	return this->width * TILE_SIZE;
}

int TileMap::getHeightPixels()
{
	return this->height * TILE_SIZE;
}

bool TileMap::isOnScreen(std::pair<float, float> worldCoords)
{
	return worldCoords.first > this->cameraX - TILE_SIZE &&
		worldCoords.first < this->cameraX + Game::stateInfo.screenWidth + TILE_SIZE &&
		worldCoords.second > this->cameraY - TILE_SIZE &&
		worldCoords.second < this->cameraY + Game::stateInfo.screenHeight + TILE_SIZE;
}