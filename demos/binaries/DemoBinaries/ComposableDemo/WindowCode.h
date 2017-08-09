#pragma once

#pragma once

/*
THIS CODE EXISTS ONLY TO DISPLAY A WINDOW,
AND IS NOT RELEVANT TO THE EXAMPLE CODE
*/

#include "DummyWindow.h"
#include <Windows.h>
#include <windowsx.h>
#include <memory>
#include <functional>
#include <stdint.h>
#include <algorithm>
#include <list>

const static int TILESIZE = 20;
const static int16_t OFFSETX = 0;
const static int16_t OFFSETY = 60;

const static int16_t YSIZE = 20;
const static int16_t XSIZE = 20;

const static int UNBLOCKING_TILE = 0;
const static int BLOCKING_TILE = 1;
const static int START_TILE = 2;
const static int END_TILE = 3;

#pragma pack(1)  
struct PlayerAttributes
{
	uint8_t stage;
	uint8_t level;

	int16_t x, y;

	uint32_t health, maxHealth;
};

PlayerAttributes player;

struct Bullet
{
	Bullet(int16_t x, int16_t y, int16_t vx, int16_t vy) : x(x), y(y), vx(vx), vy(vy) {}

	int16_t x, y;
	int16_t vx, vy;
};
std::list<Bullet> bullets;

std::list<std::pair<int16_t, int16_t>> wins;

int map[YSIZE][XSIZE] = {};
int16_t winx, winy;

#define DUMMY_SET_MESSAGE_HANDLER(dummy, message, callback) \
	do { dummy->setMessageHandler(message, callback); } while(0)

typedef std::shared_ptr<DummyWindow> DummyWindowSharedPtr;

DummyWindowSharedPtr dummyWindow;

LRESULT keyUpHandler(DummyWindow* dummy, UINT message, WPARAM wparam, LPARAM lparam)
{
	if (wparam == VK_LEFT)
		player.x--;
	else if (wparam == VK_RIGHT)
		player.x++;
	else if (wparam == VK_UP)
		player.y--;
	else if (wparam == VK_DOWN)
		player.y++;

	player.x = max(min(player.x, XSIZE - 1), 0);
	player.y = max(min(player.y, YSIZE - 1), 0);


	return DefWindowProc(dummy->getHandle(), message, wparam, lparam);
}
LRESULT mouseUpHandler(DummyWindow* dummy, UINT message, WPARAM wparam, LPARAM lparam)
{
	POINT mousePoint;
	mousePoint.x = GET_X_LPARAM(lparam) - OFFSETX;
	mousePoint.y = GET_Y_LPARAM(lparam) - OFFSETY;
	mousePoint.x /= TILESIZE;
	mousePoint.y /= TILESIZE;

mousePoint.x = mousePoint.x > XSIZE ? XSIZE : mousePoint.x;
mousePoint.y = mousePoint.y > YSIZE ? YSIZE : mousePoint.y;
mousePoint.x = mousePoint.x < 0 ? 0 : mousePoint.x;
mousePoint.y = mousePoint.y < 0 ? 0 : mousePoint.y;

if (map[mousePoint.x][mousePoint.y] == BLOCKING_TILE)
map[mousePoint.x][mousePoint.y] = UNBLOCKING_TILE;
else if (map[mousePoint.x][mousePoint.y] == UNBLOCKING_TILE)
map[mousePoint.x][mousePoint.y] = BLOCKING_TILE;

return DefWindowProc(dummy->getHandle(), message, wparam, lparam);
}

void paintString(HDC hdc, const wchar_t* string, int x, int y)
{
	TextOutW(hdc, x, y, string, wcslen(string));
}

void drawTile(int16_t x, int16_t y, HDC hdc, HBRUSH brush)
{
	RECT location;
	location.left = OFFSETX + x * TILESIZE;
	location.top = OFFSETY + y * TILESIZE;
	location.right = location.left + TILESIZE;
	location.bottom = location.top + TILESIZE;
	FillRect(hdc, &location, brush);
}

void drawBall(int16_t x, int16_t y, HDC hdc, HBRUSH brush)
{
	RECT location;
	location.left = OFFSETX + x * TILESIZE;
	location.top = OFFSETY + y * TILESIZE;
	location.right = location.left + TILESIZE;
	location.bottom = location.top + TILESIZE;

	location.left += (TILESIZE / 4);
	location.right -= (TILESIZE / 4);
	location.top += (TILESIZE / 4);
	location.bottom -= (TILESIZE / 4);

	SelectObject(hdc, brush);
	Ellipse(hdc, location.left, location.top, location.right, location.bottom);
}

void paintMap(HDC hdc)
{
	static HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
	static HBRUSH redBrush = CreateSolidBrush(RGB(225, 40, 40));
	static HBRUSH greenBrush = CreateSolidBrush(RGB(40, 225, 40));
	static HBRUSH yellowBrush = CreateSolidBrush(RGB(255, 255, 0));
	static HBRUSH greyBrush = CreateSolidBrush(RGB(160, 160, 160));

	wchar_t text[128];
	for (size_t i = 0; i < 128; i++)
		text[i] = L' ';
	text[127] = 0;
	paintString(hdc, text, 0, 0);
	paintString(hdc, text, 0, 15);
	paintString(hdc, text, 0, 30);
	paintString(hdc, text, 0, 43);


	swprintf(text, 128, L"Stage: %d", player.stage);
	paintString(hdc, text, 3, 3);

	swprintf(text, 128, L"Level: %d", player.level);
	paintString(hdc, text, 3, 22);

	swprintf(text, 128, L"Health: %d/%d", player.health, player.maxHealth);
	paintString(hdc, text, 3, 41);

	// draw background tiles
	for (int16_t x = 0; x < XSIZE; x++)
	{
		for (int16_t y = 0; y < YSIZE; y++)
		{
			drawTile(x, y, hdc, greyBrush);
		}
	}

	// win tile
	drawTile(winx, winy, hdc, yellowBrush);

	// draw player
	drawTile(player.x, player.y, hdc, greenBrush);

	// draw bullets
	for (auto b = bullets.begin(); b != bullets.end(); b++)
	{
		drawBall(b->x, b->y, hdc, redBrush);
	}


	// draw grid overlay
	for (int16_t x = 0; x < XSIZE; x++)
	{
		int16_t xStart = OFFSETX + x * TILESIZE;
		for (int16_t y = 0; y < YSIZE; y++)
		{
			int16_t yStart = OFFSETY + y * TILESIZE;
			MoveToEx(hdc, OFFSETX, yStart, NULL);
			LineTo(hdc, OFFSETX + XSIZE * TILESIZE, yStart);
		}
		MoveToEx(hdc, xStart + TILESIZE, OFFSETY, NULL);
		LineTo(hdc, xStart + TILESIZE, OFFSETY + YSIZE * TILESIZE);
	}


	// draw winner
	const HFONT font = CreateFont(48, 0, 0, 0, 300, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
	HFONT hFontOld = (HFONT)SelectObject(hdc, font);

	swprintf(text, 128, L"WINNER!", player.health, player.maxHealth);
	for (auto w = wins.begin(); w != wins.end(); w++)
		paintString(hdc, text, w->first, w->second);

	SelectObject(hdc, hFontOld);
}

LRESULT paintMessageHandler(DummyWindow* dummy, UINT message, WPARAM wparam, LPARAM lparam)
{
	RECT rect;
	GetClientRect(dummy->getHandle(), &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom + rect.left;

	PAINTSTRUCT ps;
	auto hdc = BeginPaint(dummy->getHandle(), &ps);
	auto secondhdc = CreateCompatibleDC(hdc);
	auto buffer2 = CreateCompatibleBitmap(hdc, width, height);
	SelectObject(secondhdc, buffer2);

	paintMap(secondhdc);

	BitBlt(hdc, 0, 0, width, height, secondhdc, 0, 0, SRCCOPY);
	DeleteObject(buffer2);
	DeleteDC(secondhdc);
	DeleteDC(hdc);
	EndPaint(dummy->getHandle(), &ps);
	return 0;
}


LRESULT timerMessageHandler(DummyWindow* dummy, UINT message, WPARAM wparam, LPARAM lparam)
{
	static uint32_t ticks = 0;

	if (player.x == winx && player.y == winy)
	{
		wins.push_back(std::make_pair<int16_t, int16_t>(rand() % (OFFSETX + XSIZE * TILESIZE), rand() % (OFFSETY + YSIZE * TILESIZE)));
	}
	else
		wins.clear();



	if (ticks % 28 == 0)
	{
		int16_t newX = ((ticks % (XSIZE / 4)) + 1) * 4;
		bullets.push_back(Bullet(newX, 0, 0, 1));
		bullets.push_back(Bullet(newX - 2, YSIZE - 1, 0, -1));
	}

	if (ticks % 30 == 0 && ticks % 6 == 0)
		bullets.push_back(Bullet(XSIZE - 1, XSIZE / 2 - 1, -1, 0));

	if ((ticks + 10) % 30 == 0 && ticks % 3 == 0)
		bullets.push_back(Bullet(XSIZE - 1, YSIZE / 2 + 1, -1, 0));

	if ((ticks + 20) % 30 == 0 && ticks % 5 == 0)
		bullets.push_back(Bullet(XSIZE - 1, YSIZE - 4, -1, 0));

	if ((ticks + 30) % 30 == 0 && ticks % 8 == 0)
		bullets.push_back(Bullet(XSIZE - 1, 4, -1, 0));

	if (ticks % 2 == 0)
	{
		for (auto b = bullets.begin(); b != bullets.end();)
		{
			b->x += b->vx;
			b->y += b->vy;

			if (b->x < 0 || b->x >= XSIZE || b->y < 0 || b->y >= YSIZE)
				b = bullets.erase(b);
			else
				b++;
		}
	}

	ticks++;

	RedrawWindow(dummy->getHandle(), NULL, NULL, RDW_INVALIDATE);
	return 0;
}

LRESULT closeMessageHandler(DummyWindow* dummy, UINT message, WPARAM wparam, LPARAM lparam)
{
	DestroyWindow(dummy->getHandle());
	return 0;
}
LRESULT destroyMessageHandler(DummyWindow* dummy, UINT message, WPARAM wparam, LPARAM lparam)
{
	PostQuitMessage(0);
	return 0;
}

LRESULT setCursorHandler(DummyWindow* dummy, UINT message, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(dummy->getHandle(), message, wparam, lparam);
}


void showWindow()
{
	memset((void*)&map, 0, sizeof(map));

	winx = XSIZE - 1;
	winy = YSIZE - 5;

	player.stage = 1;
	player.level = 5;

	player.x = 0;
	player.y = YSIZE / 2;

	player.health = 95;
	player.maxHealth = 100;

	dummyWindow = DummyWindowSharedPtr(new DummyWindow(NULL));
	dummyWindow->initialize();

	DUMMY_SET_MESSAGE_HANDLER(dummyWindow, WM_KEYUP, keyUpHandler);
	DUMMY_SET_MESSAGE_HANDLER(dummyWindow, WM_LBUTTONUP, mouseUpHandler);
	DUMMY_SET_MESSAGE_HANDLER(dummyWindow, WM_PAINT, paintMessageHandler);
	DUMMY_SET_MESSAGE_HANDLER(dummyWindow, WM_TIMER, timerMessageHandler);
	DUMMY_SET_MESSAGE_HANDLER(dummyWindow, WM_CLOSE, closeMessageHandler);
	DUMMY_SET_MESSAGE_HANDLER(dummyWindow, WM_DESTROY, destroyMessageHandler);
	DUMMY_SET_MESSAGE_HANDLER(dummyWindow, WM_SETCURSOR, setCursorHandler);

	dummyWindow->doMessageLoop();
}