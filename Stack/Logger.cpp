#include "Logger.h"
#include "ConstVars.h"
#include <string>
#include <ctime>
#include <fstream>

Logger::Logger()
{
	//int rc = unqlite_open(&m_db, "data.db", UNQLITE_OPEN_CREATE);
	
}


Logger::~Logger()
{
}

void Logger::ShutDown()
{
	//unqlite_close(m_db);
}



int Logger::LoadRecord()
{


	//int rc;
	//unqlite_int64 nBytes;
	////Extract data size first
	// 	rc = unqlite_kv_fetch(m_db, "max", -1, NULL, &nBytes);
	//if (rc != UNQLITE_OK) { return 0; }

	////Allocate a buffer big enough to hold the record content
	//char* zBuf = (char *)malloc(nBytes);
	//if (zBuf == NULL) { return 0; }
	//
	//
	//rc = unqlite_kv_fetch(m_db, "max", -1, zBuf, &nBytes);
	//while (rc == UNQLITE_BUSY)
	//{
	//	Sleep(20);
	//	rc = unqlite_kv_fetch(m_db, "max", -1, zBuf, &nBytes);
	//}

	//int result = std::stoi(std::string(zBuf));
	//
	//delete zBuf;

	int result;
	std::ifstream ifs("highScore.bin", std::ios::in);

	if (ifs.good()) 
	{
		ifs >> result;
	}
	else
	{
		result = 0;
	}

	return result;
}

void Logger::SaveRecord(int maxCount)
{
	/*
	int rc;
	std::string score = std::to_string(maxCount);
	
	int old_record = LoadRecord();
   	if (old_record >= maxCount)
		return;

	int len = score.size();
	
	rc = unqlite_kv_store(m_db, "max", -1, score.c_str(), len);

	while (rc == UNQLITE_BUSY)
	{
		Sleep(20);
		rc = unqlite_kv_store(m_db, "max", -1, score.c_str(), len);
	}
	*/
	int oldRecord = LoadRecord();

	if (maxCount > oldRecord)
	{
		std::ofstream ofs("highScore.bin", std::ios::trunc);
		ofs << maxCount;
	}
	return;
}

void Logger::SavePlayLog(int Count, float Height)
{
	int rc;
	std::string score = std::to_string(Count);
	std::string height = std::to_string(Height);

	auto curTime = std::time(NULL);
	struct tm * timeinfo = new tm();
	localtime_s(timeinfo, &curTime);
	char timeStr[20] = { 0, };
	strftime(timeStr, 20, "%Y%m%d-%X", timeinfo); //20160617-14:55:02
	std::string result = std::string(timeStr) + " " + score + " " + height; //20160617-14:55:02 34 34.0\n
	int len = result.size();
	
	//rc = unqlite_kv_append(m_db, "PLAYLOG", -1, result.c_str(), len);;
	//while (rc == UNQLITE_BUSY)
	//{
	//	Sleep(20);
	//	rc = unqlite_kv_append(m_db, "PLAYLOG", -1, result.c_str(), len);;
	//}

	//unqlite_commit(m_db);
	//delete timeinfo;

	
	std::ofstream ofs("playLog.log", std::ios::app);
	ofs << result << std::endl;

	return;
}


