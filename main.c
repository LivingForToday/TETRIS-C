#include <stdio.h>
#include <process.h>
#include <Windows.h>
#include "tetris.h"
#include "InitAndSync.h"

int main()
{
	HANDLE hThread[2];
	unsigned threadID[2];

	// create threads
	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, (LPTHREAD_START_ROUTINE)LowerBlock, NULL, CREATE_SUSPENDED, &threadID[0]);
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, (LPTHREAD_START_ROUTINE)ProcKbInput, NULL, CREATE_SUSPENDED, &threadID[1]);

	RemoveCursor();
	InitMutex();
	DrawBoard();

	// set the speed at which a block goes down
	SetDownDelay(11000);

	// set the sensetivity of keyboard input
	SetInputDelay(3500);

	while (1)
	{
		CreateNewBlock();
		ResumeThread(hThread[0]);
		ResumeThread(hThread[1]);
		WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
	}

	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);

	return 0;
}