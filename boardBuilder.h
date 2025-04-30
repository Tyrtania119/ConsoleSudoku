#ifndef BOARDBUILDER_H
#define BOARDBUILDER_H

#define MAX_SIZE 16 //has to be here to be gloabl:p

void InitMenu();
void ChooseBoardSize();
void ChooseDifficulty();
int isValid(int row, int col, int n);
int FillBoard(int row, int col);
void InitBoard();
void RandomizeBoard();
void ApplyDifficultyToBoard();

//to access board in gameMaster
extern int board[MAX_SIZE][MAX_SIZE];
extern int startingBoard[MAX_SIZE][MAX_SIZE];
extern int boardSize;
extern int boxSize;
extern int difficulty;
extern unsigned int seed;

#endif // BOARDBUILDER

