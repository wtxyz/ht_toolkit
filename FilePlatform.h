#pragma once

/*
* 处理不同操作平台下的文件操作，当前默认Windows
*/
#include <string>

class FilePlatform
{
public:
	static void ReadFromFile(
		const std::wstring& szFileName,
		unsigned char* readBuffer,
		unsigned long* byteReadNums);

	static void WriteToFile(
		const std::wstring path,
		const unsigned char* writeBuffer,
		size_t byteNums);

	static void GetAFileSize(
		const std::wstring filePath,
		unsigned long* fileSize);

};

