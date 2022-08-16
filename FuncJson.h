#pragma once
/*
* JSON字符串的一些处理函数
* 2022-06-29
*/

#include <stdint.h>
#include <string>

class FuncJson
{
public:
	void Run();
	//写入字节大小头
	static void WriteHeader(
		const std::string& jsonText,
		unsigned char* bytes);

	//压缩JSON
	static int CompressJSON(
		const unsigned char* jsonText,
		uint32_t lens,
		std::string& jsonCompText);
};

