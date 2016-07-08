#include "InitAndSync.h"
#include "BlockModel.h"
#include "tetris.h"
#include <Windows.h>
#include <stdio.h>

static int DOWN_DELAY;
static int INPUT_DELAY;

static int CUR_BLOCK_IDX;
static int CUR_X_POS;
static int CUR_Y_POS;

static HANDLE SCREEN_MUTEX;
static HANDLE X_POS_MUTEX;
static HANDLE Y_POS_MUTEX;
static HANDLE BLOCK_IDX_MUTEX;

// init functions
void SetDownDelay(int delay)
{
	DOWN_DELAY = delay;
}

void SetInputDelay(int delay)
{
	INPUT_DELAY = delay;
}

void InitMutex()
{
	SCREEN_MUTEX = CreateMutex(NULL, FALSE, NULL);

	if (SCREEN_MUTEX == NULL)
	{
		printf("CreateMutex error: %d\n", GetLastError());
		exit(1);
	}

	Y_POS_MUTEX = CreateMutex(NULL, FALSE, NULL);

	if (Y_POS_MUTEX == NULL)
	{
		printf("CreateMutex error: %d\n", GetLastError());
		exit(1);
	}

	X_POS_MUTEX = CreateMutex(NULL, FALSE, NULL);

	if (X_POS_MUTEX == NULL)
	{
		printf("CreateMutex error: %d\n", GetLastError());
		exit(1);
	}

	BLOCK_IDX_MUTEX = CreateMutex(NULL, FALSE, NULL);

	if (BLOCK_IDX_MUTEX == NULL)
	{
		printf("CreateMutex error: %d\n", GetLastError());
		exit(1);
	}
}

void RemoveCursor()
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

int GetDownDelay()
{
	return DOWN_DELAY;
}

int GetInputDelay()
{
	return INPUT_DELAY;
}


// funtions which set or update shared data
void SetCurrentYPos(int y)
{
		
		WaitForSingleObject(Y_POS_MUTEX, INFINITE);

		CUR_Y_POS += y;
		SetCurrentCursorPos(CUR_X_POS, CUR_Y_POS);

		ReleaseMutex(Y_POS_MUTEX);
}

void SetCurrentXPos(int x)
{

		WaitForSingleObject(X_POS_MUTEX, INFINITE);

		CUR_X_POS += x;
		SetCurrentCursorPos(CUR_X_POS, CUR_Y_POS);

		ReleaseMutex(X_POS_MUTEX);

}

void SetCurrentBlockIdx(int idx)
{
	WaitForSingleObject(BLOCK_IDX_MUTEX, INFINITE);

	CUR_BLOCK_IDX = idx;

	ReleaseMutex(BLOCK_IDX_MUTEX);
}


// functions which assign the shared data to parameters
void GetCurrentYPos(int * y)
{
	WaitForSingleObject(Y_POS_MUTEX, INFINITE);

	*y = CUR_Y_POS;

	ReleaseMutex(Y_POS_MUTEX);
}

void GetCurrentXPos(int * x)
{
	WaitForSingleObject(X_POS_MUTEX, INFINITE);

	*x = CUR_X_POS;

	ReleaseMutex(X_POS_MUTEX);
}

void GetCurrentBlockIdx(int * idx)
{
	WaitForSingleObject(BLOCK_IDX_MUTEX, INFINITE);

	*idx = CUR_BLOCK_IDX;

	ReleaseMutex(BLOCK_IDX_MUTEX);
}


// functions which manipulate the console screen
void RemoveBlock()
{
	int y, x;

	// acquire mutex lock
	WaitForSingleObject(SCREEN_MUTEX, INFINITE);

	for (y = 0; y < 4; y++)
	{
		for (x = 0; x < 4; x++)
		{
			SetCurrentCursorPos(CUR_X_POS + (x * 2), CUR_Y_POS + y);
			puts("  ");
		}
	}
	SetCurrentCursorPos(CUR_X_POS, CUR_Y_POS);	
	// free mutext lock
	ReleaseMutex(SCREEN_MUTEX);
}

void ShowBlock()
{
	int y, x;
	char (*blockInfo)[4] = blockModel[CUR_BLOCK_IDX];

	// acquire mutex lock
	WaitForSingleObject(SCREEN_MUTEX, INFINITE);

	for (y = 0; y < 4; y++)
	{
		for (x = 0; x < 4; x++)
		{
			
			SetCurrentCursorPos(CUR_X_POS + (x * 2), CUR_Y_POS + y);
			if (blockInfo[y][x] == 1) {
				fputs("бс", stdout);				
			}
		}
	}
	SetCurrentCursorPos(CUR_X_POS, CUR_Y_POS);

	// free mutex lock
	ReleaseMutex(SCREEN_MUTEX);
}

int CheckMoveToLeftOneMoreCol()
{
	int i;

	for (i = 0; i < NUM_OF_BLOCKS_ONE_COL_ZERO; i++)
	{
		if (blocksHavingOneColZero[i] == CUR_BLOCK_IDX)
			return TRUE;
	}
	return FALSE;
}

void MoveToLeft()
{
	int n = 0;
	int i, x, y;
	char(*blockInfo)[4] = blockModel[CUR_BLOCK_IDX];

	//whether the values of column 1 are all zero or not.
	//if zero, the starting point of x is 1 or if not zero, it starts from 0. 
	for (i = 0; i < NUM_OF_BLOCKS_ONE_COL_ZERO; i++)
	{
		if (blocksHavingOneColZero[i] == CUR_BLOCK_IDX)
		{
			n = 1;
			break;
		}
	}

	if (n == 1) // all values of column 1 are zero, so the inner loop starts from 1.
	{
	//	puts("zz");
		//WaitForSingleObject(SCREEN_MUTEX, INFINITE);
		SetCurrentXPos(-2);
		for (y = 0; y < 4; y++)
		{
			for (x = 1; x < 4; x++)
			{
			//	puts("hh");
				SetCurrentCursorPos(CUR_X_POS + (x * 2), CUR_Y_POS + y);
				if (blockInfo[y][x] == 1) {
					fputs("бс", stdout);
				}
			}
		}
		SetCurrentCursorPos(CUR_X_POS, CUR_Y_POS);
	}
	else
	{
		ShowBlock();
	}
}