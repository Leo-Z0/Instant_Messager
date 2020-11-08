// Instant_Messager.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <mutex>
#include <stdlib.h>

constexpr auto Console_Width = 72, Console_Height = 24;
constexpr WORD conAttrText = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
conAttrTextH = BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE,
conAttrInfo = FOREGROUND_GREEN,
conAttrInfoH = BACKGROUND_GREEN;

std::mutex mtx_console;
HANDLE hConsole = nullptr;

using length_t = unsigned long;

bool initConsole();
length_t printXY(const char* sStr, unsigned int nX = 0, unsigned int nY = 0);
length_t printXYA(const char* sStr, unsigned int nX = 0, unsigned int nY = 0, WORD wAttr = conAttrText);
length_t printXY(std::string& sStr, unsigned int nX = 0, unsigned int nY = 0);
length_t printXY(std::wstring& wsStr, unsigned int nX = 0, unsigned int nY = 0);
length_t printXY(const wchar_t* wsStr, unsigned int nX = 0, unsigned int nY = 0);

int main()
{
	initConsole();
	std::string str;
	while (std::cin >> str)
	{
		printXYA(str.c_str(), 3, 11,conAttrInfo);
	}
	_getchar_nolock();
}


bool initConsole()
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD screen_buffer = { Console_Width + 1,Console_Height + 1 };
	SMALL_RECT rect = { 0,0,Console_Width,Console_Height };
	return SetConsoleScreenBufferSize(hConsole, screen_buffer)
		&& SetConsoleWindowInfo(hConsole, 1, &rect);
}
length_t printXY(const char* sStr, unsigned int nX, unsigned int nY)
{
	static COORD crood;
	static DWORD len;
	mtx_console.lock();
	crood.X = nX;
	crood.Y = nY;
	WriteConsoleOutputCharacterA(hConsole, sStr, lstrlenA(sStr), crood, &len);
	mtx_console.unlock();
	return len;
}
length_t printXYA(const char* sStr, unsigned int nX, unsigned int nY, WORD wAttr)
{
	static COORD crood;
	static DWORD len;
	mtx_console.lock();
	crood.X = nX;
	crood.Y = nY;
	SetConsoleTextAttribute(hConsole, wAttr);
	WriteConsoleOutputCharacterA(hConsole, sStr, lstrlenA(sStr), crood, &len);
	SetConsoleTextAttribute(hConsole, conAttrText);
	HRESULT a = GetLastError();
	mtx_console.unlock();
	return len;
}
length_t printXY(std::string& sStr, unsigned int nX, unsigned int nY)
{
	static COORD crood;
	static DWORD len;
	mtx_console.lock();
	crood.X = nX;
	crood.Y = nY;
	WriteConsoleOutputCharacterA(hConsole, sStr.c_str(), sStr.length(), crood, &len);
	mtx_console.unlock();
	return len;
}
length_t printXY(std::wstring& wsStr, unsigned int nX, unsigned int nY)
{
	static COORD crood;
	static DWORD len;
	mtx_console.lock();
	crood.X = nX;
	crood.Y = nY;
	WriteConsoleOutputCharacterW(hConsole, wsStr.c_str(), wsStr.length(), crood, &len);
	mtx_console.unlock();
	return len;
}
length_t printXY(const wchar_t* wsStr, unsigned int nX, unsigned int nY)
{
	static COORD crood;
	static DWORD len;
	mtx_console.lock();
	crood.X = nX;
	crood.Y = nY;
	WriteConsoleOutputCharacterW(hConsole, wsStr, lstrlenW(wsStr), crood, &len);
	mtx_console.unlock();
	return len;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
