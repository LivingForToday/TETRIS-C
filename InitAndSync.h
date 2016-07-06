#ifndef __INIT_AND_SYNC_H__
#define __INIT_AND_SYNC_H__

// init functions
void SetDownDelay(int delay);
void SetInputDelay(int delay);
void InitMutex();
void RemoveCursor();
int GetDownDelay();
int GetInputDelay();

//functions which set or update shared data
void SetCurrentYPos(int y);
void SetCurrentXPos(int x);
void SetCurrentBlockIdx(int idx);

// functions which assign the shared data to the parameters passed this function 
void GetCurrentYPos(int * y);
void GetCurrentXPos(int * x);
void GetCurrentBlockIdx(int * idx);

//functions which manipulate the console screen
void RemoveBlock();
void ShowBlock();

#endif
