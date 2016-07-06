#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>
#include <process.h>
#include "BlockModel.h"
#include "tetris.h"

#define UP		72
#define LEFT	75
#define RIGHT	77
#define DOWN	80
#define SPACE	32
#define ESC		27
#define BOARD_WIDTH		20
#define BOARD_HEIGHT	30
#define BOARD_ORIGIN_X	4
#define BOARD_ORIGIN_Y	2

static int CUR_BLOCK_IDX;
static int CUR_X_POS;
static int CUR_Y_POS;
static int DOWN_DELAY;

void SetDownDelay(int delay)
{
	DOWN_DELAY = delay;
}

void ShowBlock(char blockInfo[][4])
{
	int y, x;

	for (y = 0; y < 4; y++)
	{
		for (x = 0; x < 4; x++)
		{
			SetCurrentCursorPos(CUR_X_POS + (x * 2), CUR_Y_POS + y);
			if (blockInfo[y][x] == 1)
				fputs("¡á", stdout);
		}
	}
	SetCurrentCursorPos(CUR_X_POS, CUR_Y_POS);
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

void RemoveCursor()
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void RemoveBlock()
{
	int y, x;
	for (y = 0; y < 4; y++)
	{
		for (x = 0; x < 4; x++)
		{
			SetCurrentCursorPos(CUR_X_POS + (x * 2), CUR_Y_POS + y);
			puts("  ");
		}
	}
}

void CreateNewBlock()
{
	//int i, j;
	HANDLE hThread[2];
	unsigned threadID[2];

	CUR_X_POS = INITIALL_X_POSITION;
	CUR_Y_POS = INITIALL_Y_POSITION;

	srand((unsigned int)time(NULL));

	// randomly select a block to be shown on the screen 
	CUR_BLOCK_IDX = rand() % NUM_OF_BLOCK_MODEL * 4;		

	// create threads
	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, LowerBlock, NULL, 0, &threadID[0]);
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, ProcKbInput, NULL, 0, &threadID[1]);

	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);
}

unsigned __stdcall ProcKbInput()
{
	int n = 0; // 0 <= n <= 3
	int inputNum;
	int i, j;
	const int baseIdxOfCurBlock = CUR_BLOCK_IDX;

	while (1) 
	{
		while (!_kbhit())
			for (i = 0; i < 3000; i++)
				for (j = 0; j < 3000; j++)
					;

		inputNum = _getch();

		switch (inputNum)
		{
		case UP:	// rotate block
			RemoveBlock();
			n = (n + 1) % 4;
			CUR_BLOCK_IDX = baseIdxOfCurBlock + n;
			ShowBlock(blockModel[CUR_BLOCK_IDX]);
			break;
		
		case LEFT:	// move block to left
			RemoveBlock();
			CUR_X_POS -= 2;
			SetCurrentCursorPos(CUR_X_POS, CUR_Y_POS);
			ShowBlock(blockModel[CUR_BLOCK_IDX]);
			break;

		case RIGHT:	//move block to right
			RemoveBlock();
			CUR_X_POS += 2;
			SetCurrentCursorPos(CUR_X_POS, CUR_Y_POS);
			ShowBlock(blockModel[CUR_BLOCK_IDX]);
			break;

		case DOWN:	//move block down
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
	while (CUR_Y_POS <= 20)
	{
		SetCurrentCursorPos(CUR_X_POS, CUR_Y_POS);
		ShowBlock(blockModel[CUR_BLOCK_IDX]);

		for (i = 0; i < DOWN_DELAY; i++)
			for (j = 0; j < DOWN_DELAY; j++)
				;

		RemoveBlock();
		CUR_Y_POS++;
	}
	_endthreadex(0);
	return 0;
}

void DrawBoard()
{
	int x, y;

	for (y = 0; y <= BOARD_HEIGHT; y++)
	{
		SetCurrentCursorPos(BOARD_ORIGIN_X, BOARD_ORIGIN_Y+y);

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
