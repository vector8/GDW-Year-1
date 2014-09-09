#include "StaticCollidable.h"
#include <string>
#include <fstream>
#include "GenericMethods.h"
#include <assert.h>

std::vector<std::vector<StaticCollidable*>> StaticCollidable::templateCollisionGroups;

StaticCollidable::StaticCollidable(CollisionType type, float worldX, float worldY, int width, int height) : type(type)
{
	this->setCollisionBounds(worldX, worldY, width, height);
}

void StaticCollidable::parseCollisionGroups()
{
	std::ifstream file;
	std::string line;
	
	std::vector<StaticCollidable*>* currentGroup = NULL;

	CollisionType type;

	file.open("levels/CollisionGroups.txt");

	while(std::getline(file, line))
	{
		line = GenericMethods::trim(line);

		if(line.size() == 0 || line[0] == '#')
			continue;
		else
		{
			if(line == "1:")
			{
				if(currentGroup)
					StaticCollidable::templateCollisionGroups.push_back(*currentGroup);

				type = FULL;
				currentGroup = new std::vector<StaticCollidable*>();
			}
			else if(line == "2:")
				type = CRAWL_THROUGH;
			else
			{
				// line structure: "x,y width,height"
				std::vector<std::string> tokens = GenericMethods::split(line, ", ");
				assert(tokens.size() == 4);

				int x, y, width, height;

				x = GenericMethods::stringToInt(tokens[0]);
				y = GenericMethods::stringToInt(tokens[1]);
				width = GenericMethods::stringToInt(tokens[2]);
				height = GenericMethods::stringToInt(tokens[3]);

				// correct for coordinate systems..
				y = 96 - y - height;

				currentGroup->push_back(new StaticCollidable(type, (float) x, (float) y, width, height));
			}
		}
	}

	if(currentGroup)
		StaticCollidable::templateCollisionGroups.push_back(*currentGroup);
}

int StaticCollidable::getCollisionGroupFromTileTypeCharacter(char tileType)
{
	switch(tileType)
	{
	case '0':
	case '7':
	case '8':
	case '9':
	case ':':
	case ';':
	case '<':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
		return 0;
		break;
	case '6':
	case 'C':
		return 1;
		break;
	case '?':
	case '@':
		return 2;
		break;
	case 'A':
	case 'B':
		return 3;
		break;
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
		return 14;
		break;
	/*case 'J':
		return 4;
		break;
	case 'K':
		return 5;
		break;
	case 'L':
		return 6;
		break;
	case 'M':
		return 7;
		break;
	case 'N':
		return 8;
		break;
	case 'O':
		return 9;
		break;
	case 'P':
		return 10;
		break;
	case 'Q':
		return 11;
		break;
	case 'R':
		return 12;
		break;*/
	case 'S':
	case 'T':
		return 13;
		break;
	default:
		return -1;
	}
}

std::vector<StaticCollidable*> StaticCollidable::createCollidables(char type, float worldX, float worldY)
{
	std::vector<StaticCollidable*> result;

	int groupIndex = StaticCollidable::getCollisionGroupFromTileTypeCharacter(type);

	if(StaticCollidable::templateCollisionGroups.size() == 0)
		StaticCollidable::parseCollisionGroups();

	assert(groupIndex < (int)StaticCollidable::templateCollisionGroups.size());

	if(groupIndex >= 0)
	{
		std::vector<StaticCollidable*>* group = &(StaticCollidable::templateCollisionGroups[groupIndex]);

		for(std::vector<StaticCollidable*>::iterator it = group->begin(); it != group->end(); it++)
		{
			result.push_back(new StaticCollidable((*it)->getType(), (*it)->getX() + worldX, (*it)->getY() + worldY, (*it)->getWidth(), (*it)->getHeight()));
		}
	}

	return result;
}

float StaticCollidable::getX()
{
	return this->getCollisionBounds().originX;
}

float StaticCollidable::getY()
{
	return this->getCollisionBounds().originY;

}
	
int StaticCollidable::getWidth()
{
	return this->getCollisionBounds().width;

}

int StaticCollidable::getHeight()
{
	return this->getCollisionBounds().height;

}

CollisionType StaticCollidable::getType()
{
	return this->type;
}