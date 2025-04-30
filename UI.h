#ifndef UI_H
#define UI_H

#define SCREEN_WIDTH 255
void PrintText(const char* lines[], int lineCount);
void PrintManual();
void PrintShortManual();
void PrintBoard();
void PrintWin();
void PrintExitGame();
void Print16();
void PrintTitleScreen();
int ExitGame();
void ClearConsole();

#endif
