#ifndef __TETRIS_H__
#define __TETRIS_H__

#include <Windows.h>

#define INITIALL_X_POSITION		20
#define INITIALL_Y_POSITION		3

void ShowBlock(char blockInfo[][4]);
void SetCurrentCursorPos(int x, int y);
COORD GetCurrentCursorPos();
void RemoveBlock();
void RemoveCursor();
void CreateNewBlock();
unsigned __stdcall MoveAndRotateBlock();
unsigned __stdcall LowerBlock();

#endif

