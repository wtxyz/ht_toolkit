#include "pch.h"
#include "FuncNFC.h"
#include <string.h>
#include <stdio.h>
#include <string>
//#include <rapidjson/document.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <conio.h>
#include "Util.h"
#include "strings.h"

//using namespace rapidjson;

#define MAC_LENGTH 12

struct Config {
public:
	std::string stream;
	std::string paramex1;
	std::string subStream;
	std::string target;
	std::string vendor;
};

struct HisMac {
	size_t idx;
	std::string mac;
	std::string stream;
	std::string subStream;
};

std::string stCodeTemp = "";
Config tmpCfg;
Util ut;
std::vector<const HisMac*> historyMac;

int GenerateNFC(const char* streamCode,
	const char* mac,
	const char* sn,
	const char* paramEx1,
	char* result);

int ParseConfig(std::string src, Config& fg);

void convert_hexa(const char* input, char* output) {
	int loop = 0;
	int i = 0;
	while (input[loop] != '\0') {
		sprintf((char*)(output + i), "%02X", input[loop]);
		loop += 1;
		i += 2;
	}
	//marking the end of the string
	output[i++] = '\0';
}

std::string hexToASCII(std::string hex)
{
	// initialize the ASCII code string as empty.
	std::string ascii = "";
	for (size_t i = 0; i < hex.length(); i += 2)
	{
		// extract two characters from hex string
		std::string part = hex.substr(i, 2);

		// change it into base 16 and
		// typecast as the character
		char ch = stoul(part, nullptr, 16);

		// add this char to final ASCII string
		ascii += ch;
	}
	return ascii;
}



bool canRun = false;

void RefreshScreen() {

	ut.ClearScreen();
	Util::SetConsoleColor(COLOR_VALUE::LimeFore);
	std::cout << "****** Welcome to Harmony Toolkit for NFC!" << std::endl;
	std::cout << "****** @Winter 2022-06-28 14:28 BUILD." << std::endl;
	std::cout << "****** Current Parameters:" << std::endl;
	std::cout << "****** Stream: ";
	Util::SetConsoleColor(COLOR_VALUE::TealFore);
	std::cout << tmpCfg.subStream;
	Util::SetConsoleColor(COLOR_VALUE::RedFore);
	std::cout << "0X06";
	Util::SetConsoleColor(COLOR_VALUE::TealFore);
	std::cout << " {MAC + 1}";
	Util::SetConsoleColor(COLOR_VALUE::RedFore);
	std::cout << " 140C ";
	Util::SetConsoleColor(COLOR_VALUE::TealFore);
	std::cout << "{MAC ASCII} ";
	Util::SetConsoleColor(COLOR_VALUE::YellowFore);
	std::cout << tmpCfg.paramex1 << std::endl;
	Util::SetConsoleColor(COLOR_VALUE::LimeFore);
	std::cout << "****** ParamEX: ";
	Util::SetConsoleColor(COLOR_VALUE::YellowFore);
	std::cout << tmpCfg.paramex1 << std::endl;
	Util::SetConsoleColor(COLOR_VALUE::LimeFore);
	std::cout << "****** HW AirLink invite code: b4zd8bz3" << std::endl;
	//std::cout << "****** Type Reset word to reset NFC template." << std::endl;
	std::cout << "****** Type [i] to show history." << std::endl;

	Util::SetConsoleColor(COLOR_VALUE::GrayFore);

	for (size_t i = 0; i < historyMac.size(); i++) {
		std::cout << "[";
		Util::SetConsoleColor(COLOR_VALUE::RedFore);
		std::cout << i;
		Util::SetConsoleColor(COLOR_VALUE::GrayFore);
		std::cout << "]" << historyMac[i]->mac << " ";
	}

	Util::SetConsoleColor(COLOR_VALUE::LimeFore);

}

void ShowHistory(int i) {
	if (i > historyMac.size()) {
		Util::SetConsoleColor(COLOR_VALUE::MaroonFore);
		std::cout << "Exceed the limitation of history." << std::endl;
		Util::SetConsoleColor(COLOR_VALUE::WhiteFore);
		return;
	}
	else {
		std::cout << "(History) Full Code: " << std::endl;
		std::cout << historyMac[i]->stream << std::endl;
		std::cout << "Use in HW AirLink : " << std::endl;
		std::cout << historyMac[i]->stream.c_str() + 4 << std::endl;
	}

}

void ResetTempScreen() {
	Util::SetConsoleColor(COLOR_VALUE::AquaFore);
	std::cout << "****** This tool is for Harmony OS NFC burning. *************" << std::endl;
	//std::cout << "****** Currently, the stream code default to SKYWORTH. ******" << std::endl;
	//std::cout << "****** If you need to update parameters, please contact me: *" << std::endl;
	//std::cout << "***************** wentuo@mxchip.com *************************" << std::endl;

	std::cout << "Please input NFC Stream Code Sample(Enter M to show example): ";

	std::cin >> stCodeTemp;

	for (auto& c : stCodeTemp) c = toupper(c);

	if (trim_copy(stCodeTemp).length() == 1 && stCodeTemp.compare("M") == 0) {
		Util::SetConsoleColor(COLOR_VALUE::LimeFore);
		std::cout << "Example:" << std::endl;
		std::cout << "0332D2022D687720010048003246535200810800552006850414170406";
		Util::SetConsoleColor(COLOR_VALUE::FuchsiaFore);
		std::cout << "A1B2C3D4E5F6";
		Util::SetConsoleColor(COLOR_VALUE::LimeFore);
		std::cout << "140C";
		Util::SetConsoleColor(COLOR_VALUE::NavyFore);
		std::cout << "413142324333443445354636";
		Util::SetConsoleColor(COLOR_VALUE::LimeFore);
		std::cout << "170100" << std::endl;
		canRun = false;
	}
	else if (trim_copy(stCodeTemp).length() > 32) {
		//Processing....
		canRun = true;
		ParseConfig(stCodeTemp, tmpCfg);
		RefreshScreen();
	}

	Util::SetConsoleColor(COLOR_VALUE::WhiteFore);
}

// Run
void FuncNFC::Run()
{
	std::string mac;

	//ResetTempScreen();

	size_t count = 0;

	while (1) {

		if (!canRun) {
			ResetTempScreen();
			continue;
		}
		Util::SetConsoleColor(COLOR_VALUE::MaroonFore);
		std::cout << std::endl;
		std::cout << "Please input MAC: ";
		Util::SetConsoleColor(COLOR_VALUE::YellowFore);

		std::cin >> mac;

		for (auto& c : mac) c = toupper(c);// to upper case

		if (mac[0] != '[' && mac.length() != 12) {
			std::cout << "Please input valid MAC!!";
			continue;
		}

		if (mac.compare("RESET") == 0) {
			//reset

		}if (mac.find('[') != mac.npos && mac.find(']') != mac.npos) {
			//Parse param to match [x] template

			int countL = std::count(mac.begin(), mac.end(), '[');
			int countR = std::count(mac.begin(), mac.end(), ']');
			std::string _dig;

			if (countL == countR && countL == 1) {
				//show
				for (auto& c : mac) {
					if (c == '[') {
						continue;
					}

					if (c == ']') {
						break;
					}

					if (isdigit(c)) {
						_dig += c;
					}

				}//for each

				int dig = atoi(_dig.c_str());

				if (dig > historyMac.size() - 1 || dig < 0 || historyMac.size() == 0) {
					Util::SetConsoleColor(COLOR_VALUE::MaroonFore);
					std::cout << "xxxxxxxxxxxxxxx Please input correct index. xxxxxxxxxxxxxxxxxxx" << std::endl;;
					Util::SetConsoleColor(COLOR_VALUE::WhiteFore);
				}
				else {
					Util::SetConsoleColor(COLOR_VALUE::AquaFore);
					std::cout << "(History) Full Code: " << std::endl;
					Util::SetConsoleColor(COLOR_VALUE::YellowFore);
					std::cout << historyMac[dig]->stream << std::endl;
					Util::SetConsoleColor(COLOR_VALUE::AquaFore);
					std::cout << "Use in HW AirLink : " << std::endl;
					Util::SetConsoleColor(COLOR_VALUE::YellowFore);
					std::cout << historyMac[dig]->stream.c_str() + 4 << std::endl;
					Util::SetConsoleColor(COLOR_VALUE::WhiteFore);
				}
			}
			else {
				Util::SetConsoleColor(COLOR_VALUE::MaroonFore);
				std::cout << "xxxxxxxxxxxxxxx Please input correct flag. xxxxxxxxxxxxxxxxxxx" << std::endl;;
				Util::SetConsoleColor(COLOR_VALUE::WhiteFore);
			}
		}
		else {
			//mac
			char* result = new char[1024];
			GenerateNFC(tmpCfg.stream.c_str(), mac.c_str(), "SN", tmpCfg.paramex1.c_str(), result);

			//Append mac to history.
			historyMac.push_back(new HisMac{ count, mac, result });
			count++;

			Util::SetConsoleColor(COLOR_VALUE::YellowFore);

			std::cout << "Full Code: " << std::endl;
			std::cout << result << std::endl;
			std::cout << "Use in HW AirLink : " << std::endl;
			std::cout << result + 4 << std::endl;
			std::cout << std::endl;
			Util::SetConsoleColor(COLOR_VALUE::WhiteFore);

			delete[] result;
		}
		std::cout << "Press any key to continue....";
		getch();
		RefreshScreen();

	}//while
}

int GenerateNFC(const char* streamCode, const char* mac, const char* sn, const char* paramEx1, char* result)
{
	int len = strlen(mac);

	char ascii_str[4096] = { 0 };
	// hex string to ascii string
	convert_hexa(mac, ascii_str);
	std::string macAscii = std::string(ascii_str);
	// hex string to int
	unsigned long long macInt = std::stoull(mac, 0, 16);
	// operation of mac + 1
	unsigned long long macIntPlusOne = macInt + 1;
	// int to hex string
	std::stringstream stream;
	stream << std::hex << macIntPlusOne;
	std::string resultHex(stream.str());
	// padding result to 6 bytes (12 hex number).
	while (resultHex.size() < 12) {
		std::string temp = "0";
		temp.append(resultHex);
		resultHex = temp;
	}
	// lower case string to upper case
	std::string resultHexUpperCase = "";
	for (auto& token : resultHex)
		resultHexUpperCase += toupper(token);
	//04578BE5219F

	std::string macPlusOne = resultHexUpperCase;// hex -> hex + 1

	std::string split1 = "140C";

	// std::string _result = streamCode + std::string(mac) + split1 + macHex + paramEx1;
	std::string _result = streamCode + macPlusOne + split1 + macAscii + paramEx1;

	strcpy(result, _result.c_str());

	return 0;
}

int ParseConfig(std::string src, Config& fg)
{
	/*
	* NEW
	码流模板：0332D2022D687720010048003246535200810800552006850414170406A1B2C3D4E5F6140C413142324333443445354636170100
	MAC地址：849DC222FBB2
	合成目标码流算法：
	0332D2022D68772001004800324653520081080055200685041417
	0406 ==> WiFi/Combo 0306==> BLE
	A1B2C3D4E5F6（将0406后面的12位字符替换成MAC地址）
	140C
	413142324333443445354636（将140c后面的24位字符，替换成MAC地址字符串对应的16进制）
	170100

	输入要求：输入码流模板，输入目标MAC地址
	输出要求：输出目标码流
	*/

	size_t foundWIFI = src.find("0406");//find first(WiFi==>0406,BLE==>0306)
	size_t foundBLE = src.find("0306");

	size_t found = std::min(foundWIFI, foundBLE);

	std::string subStreamStr = src.substr(0, found + 4);//stream
	std::string subLastStr = src.substr(found + 4 + MAC_LENGTH + 4 + 2 * MAC_LENGTH);

	fg.stream = subStreamStr;
	fg.paramex1 = subLastStr;
	fg.subStream = src.substr(0, found);

	return 0;
}


/*
* 旧版规则
int Func::GenerateNFCStreamForHuawei(const char* streamCode, const char* mac, const char* sn, const char* paramEx1, char* result)
{
	//old
	// skyworth stream code
	// StreamCode: 0347D202426877200100480032374d310081090057200685041417910406
	// ParamEx1: 1701009112
	// Exp:
	// 0347D202426877200100480032374d310081090057200685041417910406{ MAC + 1 }140c{ SN }1701009112{MAC} {SN}
	// 0347D202426877200100480032374d310081090057200685041417910406
	// 0478636BE436 ==> MAC + 1
	// 140c
	// 303437383633364245343335 ==> Hex SN(hex(Mac+1))
	// 1701009112
	// 0478636BE435 ==>MAC
	// 303437383633364245343335 ==> Hex SN

	int len = strlen(mac);

	char hex_str[4096] = { 0 };
	convert_hexa(mac, hex_str);
	std::string macHex = std::string(hex_str);
	//std::string prefix = "";

	std::string macLastByte = macHex.substr(macHex.length() - 2, macHex.length() - 1);

	std::string macWithoutLastByte = macHex.substr(0, macHex.length() - 2);

	std::string macPlusOneHex = macWithoutLastByte + std::to_string(std::atoi(macLastByte.c_str()) + 1);

	std::string macPlusOne = hexToASCII(macPlusOneHex);//hex -> ascii

	std::string split1 = "140c";

	std::string split2 = paramEx1;

	std::string _result = streamCode + macPlusOne + split1 + macHex + split2 + mac + macHex;

	strcpy(result, _result.c_str());

	return 1;
}
*/


/*
* 旧版使用JSON读取
int Func::ReadConfig(const char* cfgPath, Config& cfg)
{
	std::ifstream cfgStream(cfgPath);
	std::string temp;
	std::string content="";

	while (std::getline(cfgStream, temp)) {
		content.append(temp);
	}

	Document _cfgDoc;
	_cfgDoc.Parse(content.c_str());

	cfg.stream = _cfgDoc["stream"].GetString();
	cfg.paramex1 = _cfgDoc["paramex1"].GetString();


	return 0;
}
*/