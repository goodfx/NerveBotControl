#include "stdafx.h"
#include <io.h>
#include <direct.h>
#include <string>
#include "Logs.h"
#include "StringConvert.h"
void LogsOut(std::string LogString) {
	std::string LogFloder = "log/";
	if (_access(LogFloder.c_str(), 0) == -1) {
		_mkdir(LogFloder.c_str());
	}
	
	std::string TimeString = TimeToString(time(NULL), "%d-%02d-%02d");
	std::string CurrentLogFloder = LogFloder + TimeString;
	if (_access(CurrentLogFloder.c_str(), 0) == -1) {
		_mkdir(CurrentLogFloder.c_str());
	}
	std::string LogFile = CurrentLogFloder + "/log.txt";
	FILE* fp = NULL;
	fopen_s(&fp, LogFile.c_str(), "a");
	if (fp != NULL) {
		std::string LogContent = format("%s\n", LogString.c_str());
		fwrite(LogContent.c_str(), 1, LogContent.size(), fp);
		fclose(fp);
	}

}