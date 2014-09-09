#pragma once

#include <fstream>

#define SEPARATOR ","

class SaveFileManager
{
private:
	SaveFileManager();
	~SaveFileManager();
public:
	static SaveFileManager* getInstance();

	bool saveExists(int saveFileNumber);

	void save(int saveFileNumber);

	void load(int saveFileNumber);
};