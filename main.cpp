#include "pch.h"
#include "FuncNFC.h"
#include "FuncJson.h"
#include <iostream>


#define GET_FUNC(name) strcmp(name, argc[1]) == 0 ? 1 : 0

int main(int argv, char* argc[])
{
	FuncNFC funNFCHandle;
	FuncJson funJsonHandle;

	if (argv == 1) {
		std::cout << "Please input help to get more function." << std::endl;
	}

	if (argv == 2) {

		if (GET_FUNC("help")) {
			std::cout << "ht_toolkit nfc ==> HarmonyOS NFC." << std::endl;
			std::cout << "ht_toolkit json ==> JSON Utils." << std::endl;
		}
		else if (GET_FUNC("nfc")) funNFCHandle.Run();
		else if (GET_FUNC("json")) funJsonHandle.Run();

		//if (strcmp)
	}
	//funNFCHandle.Run();

}
