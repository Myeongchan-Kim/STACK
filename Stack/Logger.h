#pragma once
#include <wchar.h>
#include <time.h>
extern "C"
{
#include <unqlite.h>
}

class Logger
{
public:
	Logger();
	~Logger();

	int LoadRecord();
	void SaveRecord(int maxCount);
	void SavePlayLog(int Count, float Height);
private:
	unqlite* m_db;
};

