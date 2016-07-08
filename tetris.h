#ifndef __TETRIS_H__
#define __TETRIS_H__

#include <Windows.h>

#define UP			72
#define LEFT		75
#define RIGHT		77
#define DOWN		80
#define SPACE		32
#define ESC			27

#define BOARD_WIDTH			20
#define BOARD_HEIGHT		30
#define BOARD_ORIGIN_X		4
#define BOARD_ORIGIN_Y		2

#define INITIALL_X_POSITION		22
#define INITIALL_Y_POSITION		2

#define TRUE	1
#define FALSE	0

void DrawBoard();

void SetCurrentCursorPos(int x, int y);
COORD GetCurrentCursorPos();

void CreateNewBlock();
unsigned __stdcall ProcKbInput();
unsigned __stdcall LowerBlock();

#endif

