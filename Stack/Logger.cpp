#include "Logger.h"
#include "ConstVars.h"
#include <string>
#include <ctime>

Logger::Logger()
{
	//char* pStr;
	//int strSize = WideCharToMultiByte(CP_ACP, 0, "data.db", -1, NULL, 0, NULL, NULL);
	////char* 메모리 할당
	//pStr = new char[strSize];

	////형 변환 
	//WideCharToMultiByte(CP_ACP, 0, fileName, -1, pStr, strSize, 0, 0);
	//
	//delete pStr;
	int rc = unqlite_open(&m_db, "data.db", UNQLITE_OPEN_CREATE);
}


Logger::~Logger()
{
}

void Logger::ShutDown()
{
	unqlite_close(m_db);
}



int Logger::LoadRecord()
{

	int rc;
	unqlite_int64 nBytes;
	//Extract data size first
   	rc = unqlite_kv_fetch(m_db, "max", -1, NULL, &nBytes);
	if (rc != UNQLITE_OK) { return 0; }

	//Allocate a buffer big enough to hold the record content
	char* zBuf = (char *)malloc(nBytes);
	if (zBuf == NULL) { return 0; }
	rc = unqlite_kv_fetch(m_db, "max", -1, zBuf, &nBytes);
	int result = std::stoi(std::string(zBuf));
	
	delete zBuf;

	return result;
}

void Logger::SaveRecord(int maxCount)
{
	int rc;
	std::string score = std::to_string(maxCount);
	
	int old_record = LoadRecord();
	if (old_record > maxCount)
		return;

	int len = score.size();
	rc = unqlite_kv_store(m_db, "max", -1, score.c_str(), len);
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
	std::string result = std::string(timeStr) + " " + score + " " + height +"\n"; //20160617-14:55:02 34 34.0\n
	int len = result.size();
	
	rc = unqlite_kv_append(m_db, "PLAYLOG", -1, result.c_str(), len);;
	
	unqlite_commit(m_db);
	delete timeinfo;

	return;
}


