#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>
#include <process.h>
#include "BlockModel.h"
#include "tetris.h"

int CUR_BLOCK_IDX;
int CUR_X_POS;
int CUR_Y_POS;

void ShowBlock(char blockInfo[][4])
{
	int y, x;
	//COORD curPos = GetCurrentCursorPos();

	for (y = 0; y < 4; y++)
	{
		for (x = 0; x < 4; x++)
		{
			//SetCurrentCursorPos(curPos.X + (x * 2), curPos.Y + y);
			SetCurrentCursorPos(CUR_X_POS + (x * 2), CUR_Y_POS + y);
			if (blockInfo[y][x] == 1)
				fputs("бс", stdout);
		}
	}
	//SetCurrentCursorPos(curPos.X, curPos.Y);
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
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, MoveAndRotateBlock, NULL, 0, &threadID[1]);

	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);
}

unsigned __stdcall MoveAndRotateBlock()
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

		if (inputNum == 75) //move to left
		{
			RemoveBlock();
			CUR_X_POS -= 2;
			SetCurrentCursorPos(CUR_X_POS, CUR_Y_POS);
			ShowBlock(blockModel[CUR_BLOCK_IDX]);
		}
		else if (inputNum == 77) //move to right
		{
			RemoveBlock();
			CUR_X_POS += 2;
			SetCurrentCursorPos(CUR_X_POS, CUR_Y_POS);
			ShowBlock(blockModel[CUR_BLOCK_IDX]);
		}
		else if (inputNum == 72) //rotate block
		{
			RemoveBlock();
			n = (n + 1) % 4;
			CUR_BLOCK_IDX = baseIdxOfCurBlock + n;
			//SetCurrentCursorPos(CUR_X_POS, CUR_Y_POS);
			ShowBlock(blockModel[CUR_BLOCK_IDX]);
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

		for (i = 0; i < 20000; i++)
			for (j = 0; j < 20000; j++)
				;

		RemoveBlock();
		CUR_Y_POS++;
	}
	_endthreadex(0);
	return 0;
}