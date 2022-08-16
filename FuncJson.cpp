#include "pch.h"
#include "FuncJson.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <sstream> // std::stringstream

#include "FilePlatform.h"



// 替换字符串的某一字符
std::wstring& replace_all(
	std::wstring& str,
	const std::wstring& old_value,
	const std::wstring& new_value)
{
	while (true) {
		std::wstring::size_type pos(0);
		if ((pos = str.find(old_value)) != std::wstring::npos)
			str.replace(pos, old_value.length(), new_value);
		else break;
	}
	return str;
}//replace_all


void FuncJson::WriteHeader(
	const std::string& jsonText,
	unsigned char* bytes)
{
	//文件头扩展4字节 EF 0B FF FF
	size_t strLen = jsonText.length() + 4;

	//std::cout << strLen << std::endl;
	// 字符串大小转换成十六进制
	std::stringstream stream;
	stream << std::hex << strLen;

	//Hex值，没有0x开头
	std::string result(stream.str());

	//小于八个字符，此处代表4个字节，EF 0B FF FF
	if (result.length() <= 8) {
		int _len = 8 - result.length();

		for (int i = 0; i < _len; i++)
			result = "0" + result;//padding to this format : 00 00 00 3C

		std::vector<unsigned long> nums{};

		for (int i = 0; i < 8; i += 2) {
			std::string temp = "0x" + result.substr(i, 2);
			nums.emplace_back(std::stoul(temp, nullptr, 16));
		}

		//反转原先顺序，用于小端字节输出
		std::reverse(nums.begin(), nums.end());

		for (int i = 0; i < nums.size(); i++) {
			bytes[i] = nums[i];
		}

		for (int i = 0; i < jsonText.length(); i++) {
			bytes[i + 4] = jsonText[i];
		}

		std::cout << "Done!" << std::endl;
	}
	else {
		std::cout << "This file is too large." << std::endl;
		std::cout << "Running out in exception!" << std::endl;
	}

}//write_file_header_with_size_in_little_endian


int FuncJson::CompressJSON(
	const unsigned char* jsonText,
	uint32_t lens,
	std::string& jsonCompText)
{
	bool hasPassed = false;

	size_t spaceNums = 0;

	std::vector<char> filterContents;

	for (int i = 0; i < lens; i++) {
		//压缩JSON文件
				// Dec: 34 " Hex: 0x22
		if (int(jsonText[i]) == 0x22) {
			spaceNums++;
		}

		//先执行过滤
		if (!hasPassed) {
			//第一个字符不等于 { ，表示该json文件存在问题，前面的应该去掉
			if (int(jsonText[i]) != 0x7B) {
				//std::cout << "Smile!" << std::endl;
				continue;
			}
			else {
				hasPassed = true;
			}
		}//if ==> hasPassed

		//单独过滤每个字符，控制字符，不可见字符等全部过滤
		//0x20 为空格，如果闭包值中含有空格不应该去掉
		if ((int(jsonText[i]) >= 0x00 && int(jsonText[i]) <= 0x20) || (int(jsonText[i]) >= 0x7F && int(jsonText[i]) <= 0xA0)) {

			if (int(jsonText[i] == 0x20)) {
				//判断是否之前有 " 完成闭合
				if (spaceNums % 2 == 0) {
					//如果是偶数的，说明闭包完成，可以忽略该空格
				}
				else {
					//说明在中间，不能忽略
					filterContents.push_back(jsonText[i]);
				}
			}//if 0x20 " "
		}
		else {
			filterContents.push_back(jsonText[i]);
		}
	}//for

	for (auto& token : filterContents) {
		jsonCompText.push_back(token);
	}

	return jsonCompText.size();
}

#define BUFFERSIZE 1024000

void FuncJson::Run() {

	unsigned long fileSize = 0;

	unsigned char* readBuffer = (unsigned char*)malloc(BUFFERSIZE);

	memset(readBuffer, 0, sizeof(readBuffer));

	FilePlatform::ReadFromFile(L"name", readBuffer, &fileSize);

	std::string jsonCompedText;

	int nums = FuncJson::CompressJSON(readBuffer, fileSize, jsonCompedText);

	unsigned char* writeBuffer = (unsigned char*)malloc(nums + 4);//添加4字节

	memset(readBuffer, 0, sizeof(readBuffer));

	//得到压缩后的文档，进行文件头增加
	FuncJson::WriteHeader(jsonCompedText, writeBuffer);

}
