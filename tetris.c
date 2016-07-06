#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>
#include <process.h>
#include "tetris.h"
#include "InitAndSync.h"

void DrawBoard()
{
	int x, y;

	for (y = 0; y <= BOARD_HEIGHT; y++)
	{
		SetCurrentCursorPos(BOARD_ORIGIN_X, BOARD_ORIGIN_Y + y);

		if (y == BOARD_HEIGHT)
			fputs("¦¦", stdout);
		else
			fputs("¦¢", stdout);
	}

	for (y = 0; y <= BOARD_HEIGHT; y++)
	{
		SetCurrentCursorPos(BOARD_ORIGIN_X + (BOARD_WIDTH + 1) * 2, BOARD_ORIGIN_Y + y);

		if (y == BOARD_HEIGHT)
			fputs("¦¥", stdout);
		else
			fputs("¦¢", stdout);
	}

	for (x = 1; x < BOARD_WIDTH + 1; x++)
	{
		SetCurrentCursorPos(BOARD_ORIGIN_X + x * 2, BOARD_ORIGIN_Y + BOARD_HEIGHT);

		fputs("¦¡", stdout);
	}

	SetCurrentCursorPos(0, 0);
}

void SetCurrentCursorPos(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

COORD GetCurrentCursorPos()
{
	COORD curPoint;
	CONSOLE_SCREEN_BUFFER_INFO curInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curPoint.X = curInfo.dwCursorPosition.X;
	curPoint.Y = curInfo.dwCursorPosition.Y;

	return curPoint;
}

void CreateNewBlock()
{
	SetCurrentXPos(INITIALL_X_POSITION);
	SetCurrentYPos(INITIALL_Y_POSITION);

	// randomly select a block to be shown on the screen 
	srand((unsigned int)time(NULL));
	SetCurrentBlockIdx(rand() % 7 * 4);
}

unsigned __stdcall ProcKbInput()
{
	int n = 0; // 0 <= n <= 3
	int inputNum;
	int i, j;
	int baseIdxOfCurBlock;
	int delay;

	GetCurrentBlockIdx(&baseIdxOfCurBlock);
	delay = GetInputDelay();

	while (1) 
	{
		while (!_kbhit())
			for (i = 0; i < delay; i++)
				for (j = 0; j < delay; j++)
					;

		inputNum = _getch();

		switch (inputNum)
		{
		case UP:	// rotate block

			RemoveBlock();
			n = (n + 1) % 4;

			SetCurrentBlockIdx(baseIdxOfCurBlock + n);

			ShowBlock();
			break;
		
		case LEFT:	// move block to left
			RemoveBlock();
			SetCurrentXPos(-2);
			ShowBlock();
			break;

		case RIGHT:	//move block to right
			RemoveBlock();
			SetCurrentXPos(+2);
			ShowBlock();
			break;

		case DOWN:	//move block down
			RemoveBlock();
			SetCurrentYPos(+1);
			ShowBlock();
			break;

		case SPACE:	//put block down on the bottom immediately 
			break;

		}
	}	
	_endthreadex(0);
	return 0;
}

unsigned __stdcall LowerBlock()
{
	int i, j;
	int yPos;
	int delay;

	delay = GetDownDelay();
	GetCurrentYPos(&yPos);

	while (yPos <= BOARD_HEIGHT - 2)
	{
		ShowBlock();

		for (i = 0; i < delay; i++)
			for (j = 0; j < delay; j++)
				;

		RemoveBlock();
		SetCurrentYPos(+1);
		GetCurrentYPos(&yPos);
	}
	_endthreadex(0);
	return 0;
}
