#pragma once
#include <wchar.h>
#include <time.h>
#include <fstream>

extern "C"
{
#include "unqlite.h"
}

class Logger
{
public:
	Logger();
	~Logger();

	void ShutDown();

	int LoadRecord();
	void SaveRecord(int maxCount);
	void SavePlayLog(int Count, float Height);
private:
	unqlite* m_db;
	
};

