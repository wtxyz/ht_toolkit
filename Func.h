#pragma once
#include "Config.h"
class Func
{
public:
	//int GenerateNFCStreamForHuawei(const char* streamCode, const char* mac, const char* sn, const char* paramEx1, char* result);
	int GenerateNFC(const char* streamCode, const char* mac, const char* sn, const char* paramEx1, char* result);
	//int ReadConfig(const char* cfgPath,Config& cfg);
	int ParseConfig(std::string src, Config& fg);
};

