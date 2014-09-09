#pragma once
#include "Sprite.h"
#include <vector>
#include "MiscGameResources.h"
#include <fstream>
#include <sstream>
#include "Player.h"
#include "GenericMethods.h"

struct TileTypeWithCoords
{
	// i.e. the animation number on the tileset sprite
	int type;

	// screen coordinates
	float x;
	float y;
};

class TileMap
{
private:
	int** tiles;

	// world coordinates (in pixels) of the camera
	float cameraX, cameraY;

	// scroll speed of the camera, in pixels per second
	float dx, dy;

	// width and height of the map, in tiles
	int width, height;

	std::string fileName;

	Sprite* tileSetSprite;

	std::vector<TileTypeWithCoords> drawList;

	void initializeTileSetSprite();

	void loadMap();

	void setScrollSpeed();

	int getTileNumberFromCharacter(char c);

public:
	TileMap(std::string fileName, int width, int height, std::string tileset);
	~TileMap();

	void draw();
	void update(float dt);

	void setHorizontalScrollSpeed(float dx);
	void setVerticalScrollSpeed(float dy);

	std::pair<float, float> getWorldCoordsOfCamera();

	void setWorldCoordsOfCamera(std::pair<float, float> coords);

	std::pair<float, float> convertWorldCoordsToScreenCoords(std::pair<float, float> worldCoords);

	int getWidthTiles();
	int getHeightTiles();

	int getWidthPixels();
	int getHeightPixels();

	bool isOnScreen(std::pair<float, float> worldCoords);
};