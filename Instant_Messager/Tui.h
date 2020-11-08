#pragma once
// by lzy
#include <iostream>
#include <string>
#include <Windows.h>
#include <mutex>
#include <assert.h>
#include <stdlib.h>
//相信你看得懂我就不写注释了（狗头）
extern DWORD Console_Width, Console_Height;
constexpr WORD conAttrText = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
conAttrTextH = BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE,
conAttrInfo = FOREGROUND_GREEN,
conAttrInfoH = BACKGROUND_GREEN;

std::mutex mtx_console;
HANDLE hConsole = nullptr;

using length_t = unsigned long;
using lRect = RECT;
using lCoord = COORD;
bool initConsole();

length_t printXY(const char* str, const unsigned int nX = 0, const unsigned int nY = 0);
length_t printXY(const char* str, const lCoord& coord = { 0,0 });

length_t printXYA(const char* str, const unsigned int nX = 0, const unsigned int nY = 0, const WORD wAttr = conAttrText);
length_t printXYA(const char* str, const lCoord& coord = { 0,0 }, const WORD wAttr = conAttrText);
void drawRectA(const lRect& rect, const char& ch = '+', const WORD wAttr = conAttrText);


class LLabel
{
public:
	enum class LAlign
	{
		middle,
		left,
		right
	};
	LLabel(const lRect&& rect, const char&& chRect = '+', const WORD wAttrRect = conAttrText, const WORD wAttrText = conAttrText, const LAlign align = LAlign::left)
	{
		this->mChar = chRect;
		this->mRect = rect;
		this->mAttrRect = wAttrRect;
		this->mAttrText= wAttrText;
		this->mAlign = align;
#ifdef _DEBUG
		assert(rect.right - rect.left > 0);
		assert(rect.bottom - rect.top > 0);
#endif
		this->width = rect.right - rect.left;
		this->height = rect.bottom - rect.top;
	}
	LLabel() = delete;
	void refreshText()
	{
		char ch[2] = { 0,0 };
		lCoord coord = { 0,0 };
		auto iter = this->mCaption.cbegin();
		switch (this->mAlign)
		{
		case LLabel::LAlign::left:
			for (coord.Y = this->mRect.top + 1; coord.Y < this->mRect.bottom; ++coord.Y)
			{
				for (coord.X = this->mRect.left + 1; coord.X < this->mRect.right; ++coord.X)
				{
					if (iter != this->mCaption.cend())
					{
						ch[0] = *iter;
						printXYA((char*)ch, coord, this->mAttrText);
						++iter;
					}
					else
					{
						ch[0] = ' ';
						printXYA((char*)ch, coord, this->mAttrText);
					}
				}
			}
			break;
		case LLabel::LAlign::middle:
			//todo
			break;
		case LLabel::LAlign::right:
			break;
		default:
			break;
		}

	}
	void refreshRect()
	{
		drawRectA(this->mRect, this->mChar, this->mAttrRect);
	}
	void refresh()
	{
		this->refreshRect();
		this->refreshText();
	}
	void setText(const std::string& caption)
	{
		this->mCaption.assign(caption);
		this->refresh();
	}

private:
	std::string mCaption;
	lRect mRect;
	char mChar;
	WORD mAttrRect;
	WORD mAttrText;
	LAlign mAlign;
	unsigned int width, height;
};
class LTextbox;

bool initConsole()
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD screen_buffer = { Console_Width + 1,Console_Height + 1 };
	SMALL_RECT rect = { 0,0,Console_Width,Console_Height };
	return SetConsoleScreenBufferSize(hConsole, screen_buffer)
		&& SetConsoleWindowInfo(hConsole, 1, &rect);
}
length_t printXY(const char* str, const unsigned int nX, const unsigned int nY)
{
	static COORD coord;
	static DWORD len;
	mtx_console.lock();
	coord.X = nX;
	coord.Y = nY;
	WriteConsoleOutputCharacterA(hConsole, str, lstrlenA(str), coord, &len);
	mtx_console.unlock();
	return len;
}
length_t printXY(const char* str, const lCoord& coord)
{
	static DWORD len;
	mtx_console.lock();
	WriteConsoleOutputCharacterA(hConsole, str, lstrlenA(str), coord, &len);
	mtx_console.unlock();
	return len;
}
length_t printXYA(const char* str, const unsigned int nX, const unsigned int nY, WORD wAttr)
{
	static COORD coord;
	static DWORD len, len2;
	mtx_console.lock();
	coord.X = nX;
	coord.Y = nY;
	WriteConsoleOutputCharacterA(hConsole, str, lstrlenA(str), coord, &len);
	FillConsoleOutputAttribute(hConsole, wAttr, len, coord, &len2);
	mtx_console.unlock();
	return len;
}

length_t printXYA(const char* str, const lCoord& coord, WORD wAttr)
{
	static DWORD len, len2;
	mtx_console.lock();
	WriteConsoleOutputCharacterA(hConsole, str, lstrlenA(str), coord, &len);
	FillConsoleOutputAttribute(hConsole, wAttr, len, coord, &len2);
	mtx_console.unlock();
	return len;
}

void drawRectA(const lRect& rect, const char& ch, const WORD wAttr)
{
	static DWORD len, len2;
	static lCoord coord1;
	coord1.X = rect.left, coord1.Y = rect.top;
	len = rect.right - rect.left + 1;
	FillConsoleOutputCharacterA(hConsole, ch, len, coord1, &len2);
	FillConsoleOutputAttribute(hConsole, wAttr, len, coord1, &len2);
	coord1.Y = rect.bottom;
	FillConsoleOutputCharacterA(hConsole, ch, len, coord1, &len2);
	FillConsoleOutputAttribute(hConsole, wAttr, len, coord1, &len2);
	for (coord1.Y = rect.top + 1; coord1.Y < rect.bottom; ++coord1.Y)
	{
		coord1.X = rect.left;
		WriteConsoleOutputCharacterA(hConsole, &ch, 1, coord1, &len2);
		FillConsoleOutputAttribute(hConsole, wAttr, 1, coord1, &len2);
		coord1.X = rect.right;
		WriteConsoleOutputCharacterA(hConsole, &ch, 1, coord1, &len2);
		FillConsoleOutputAttribute(hConsole, wAttr, 1, coord1, &len2);
	}
}
