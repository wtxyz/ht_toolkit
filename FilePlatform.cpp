#include "pch.h"
#include "FilePlatform.h"
#include <sstream>
#include "FuncJson.h"

#ifdef UNICODE
typedef std::wostringstream tstringstream;
#else
typedef std::ostringstream tstringstream;
#endif

std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

std::string ws2s(const std::wstring& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
	char* buf = new char[len];
	WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, buf, len, 0, 0);
	std::string r(buf);
	delete[] buf;
	return r;
}

void FilePlatform::ReadFromFile(const std::wstring& szFileName, unsigned char* readBuffer, unsigned long* byteReadNums) {

	OVERLAPPED ol = { 0 };

	HANDLE hFile;
	DWORD nIn = 0;

	hFile = ::CreateFile(
		szFileName.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,// | FILE_FLAG_OVERLAPPED(async),
		NULL);

	LPDWORD _fileSize = NULL;

	GetAFileSize(szFileName, _fileSize);

	if (_fileSize == 0) *byteReadNums = 0;

	if (hFile != INVALID_HANDLE_VALUE) {

		//存储字节
		::ReadFile(hFile, readBuffer, *_fileSize, &nIn, NULL);
		*byteReadNums = nIn;

		CloseHandle(hFile);
	}

}//

void FilePlatform::WriteToFile(const std::wstring outPath, const unsigned char* writeBuffer, size_t byteNums) {

	std::wstring out = outPath;

	HANDLE hOutFile;
	hOutFile = ::CreateFile(
		out.c_str(),                // name of the write
		GENERIC_WRITE,          // open for writing
		0,                      // do not share
		NULL,                   // default security
		CREATE_ALWAYS,             // create new file only
		FILE_ATTRIBUTE_NORMAL,  // normal file
		NULL);

	DWORD dwBytesWritten = 0;

	::WriteFile(
		hOutFile,           // open file handle
		writeBuffer,      // start of data to write
		byteNums,  // number of bytes to write
		&dwBytesWritten, // number of bytes that were written
		NULL);
	CloseHandle(hOutFile);
}

void FilePlatform::GetAFileSize(const std::wstring filePath, unsigned long* fileSize)
{
	std::wstring out = filePath;

	HANDLE hFile;
	hFile = ::CreateFile(
		out.c_str(),                // name of the write
		GENERIC_READ,          // open for reading
		0,                      // do not share
		NULL,                   // default security
		CREATE_ALWAYS,             // create new file only
		FILE_ATTRIBUTE_NORMAL,  // normal file
		NULL);

	if (hFile == INVALID_HANDLE_VALUE) *fileSize = 0;

	::GetFileSize(hFile, fileSize);

	CloseHandle(hFile);
}
