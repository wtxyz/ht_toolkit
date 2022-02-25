#include "pch.h"
#include "Util.h"

int Util::ClearScreen()
{
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return 0;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return 0;
	cellCount = csbi.dwSize.X * csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR)' ',
		cellCount,
		homeCoords,
		&count
	)) return 0;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
	)) return 0;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
	return 0;
}


/*
Black            0           0
Blue             1           1
Green            2           2
Cyan             3           3
Red              4           4
Magenta          5           5
Brown            6           6
White            7           7
Gray             -           8
Intense Blue     -           9
Intense Green    -           10
Intense Cyan     -           11
Intense Red      -           12
Intense Magenta  -           13
Yellow           -           14
Intense White    -           15
*/

void Util::SetConsoleColor(COLOR_VALUE textColor, COLOR_VALUE bgColor)
{
	//SetConsoleTextAttribute(console, ((int)background << 4) + (int)forground);
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD _color = textColor | bgColor;

	SetConsoleTextAttribute(hConsole, _color);

}
