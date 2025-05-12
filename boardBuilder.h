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
void CopyBoard(int** boardToCopy, int** resBoard);

//to access board in gameMaster
extern int** board;
extern int** startingBoard;
extern int boardSize;
extern unsigned int previousBoardSize; //to properly disallocate memory
extern int boxSize;
extern int difficulty;
extern unsigned int seed;

#endif // BOARDBUILDER

