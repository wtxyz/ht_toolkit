#include "pch.h"
#include <iostream>
#include <sstream>
#include "Func.h"
#include "Util.h"
#include "strings.h"
#include <vector>
#include <conio.h>

struct HisMac {
	size_t idx;
	std::string mac;
	std::string stream;
	std::string subStream;
};


std::string stCodeTemp = "";
Config tmpCfg;
Util ut;
Func func;
std::vector<const HisMac*> historyMac;

bool canRun = false;

void RefreshScreen() {

	ut.ClearScreen();
	Util::SetConsoleColor(COLOR_VALUE::LimeFore);
	std::cout << "****** Welcome to Harmony Toolkit for NFC! @Winter" << std::endl;
	std::cout << "****** Current Parameters:" << std::endl;
	std::cout << "****** Stream: ";
	Util::SetConsoleColor(COLOR_VALUE::TealFore);
	std::cout << tmpCfg.subStream;
	Util::SetConsoleColor(COLOR_VALUE::RedFore);
	std::cout << "0406";
	Util::SetConsoleColor(COLOR_VALUE::TealFore);
	std::cout << " {MAC}";
	Util::SetConsoleColor(COLOR_VALUE::RedFore);
	std::cout << " 140C ";
	Util::SetConsoleColor(COLOR_VALUE::TealFore);
	std::cout << "{MAC HEX} ";
	Util::SetConsoleColor(COLOR_VALUE::YellowFore);
	std::cout << tmpCfg.paramex1 << std::endl;
	Util::SetConsoleColor(COLOR_VALUE::LimeFore);
	std::cout << "****** ParamEX: ";
	Util::SetConsoleColor(COLOR_VALUE::YellowFore);
	std::cout<< tmpCfg.paramex1 << std::endl;
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

	if (trim_copy(stCodeTemp).length() == 1 && stCodeTemp.compare("M") == 0) {
		Util::SetConsoleColor(COLOR_VALUE::LimeFore);
		std::cout << "Example:" << std::endl;
		std::cout << "0332D2022D687720010048003246535200810800552006850414170406A1B2C3D4E5F6140C413142324333443445354636170100" << std::endl;
		canRun = false;
	}
	else {
		//Processing....
		canRun = true;
		func.ParseConfig(stCodeTemp, tmpCfg);
		RefreshScreen();
	}

	Util::SetConsoleColor(COLOR_VALUE::WhiteFore);
}

int main()
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
					Util::SetConsoleColor(COLOR_VALUE::YellowFore);
					std::cout << "Full Code: " << std::endl;
					std::cout << historyMac[dig]->stream << std::endl;
					std::cout << "Use in HW AirLink : " << std::endl;
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
			func.GenerateNFC(tmpCfg.stream.c_str(), mac.c_str(), "SN", tmpCfg.paramex1.c_str(), result);

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
