#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "boardBuilder.h"
#include "gameMaster.h"
#include "UI.h"
#include "Utils.h"
#include "DevMode.h"

#define MAX_SIZE 16

//default values
int board[MAX_SIZE][MAX_SIZE];
int startingBoard[MAX_SIZE][MAX_SIZE];
int boardSize = 9; //number of squares
int boxSize = 3; //number of boxes per square
int difficulty = 1;

unsigned int seed;

void ChooseBoardSize() {

    int choice;

    const char* sizes[] = {
        "*** Select board size ***\n",
        "1. 4x4",
        "2. 9x9",
        "3. 16x16"
    };

    PrintText(sizes, 4);

    scanf("%d", &choice);

       switch (choice) {
        case 1:
            boardSize = 4;
            boxSize = 2;
            break;
        case 2:
            boardSize = 9;
            boxSize = 3;
            break;
        case 3:
            boardSize = 16;
            boxSize = 4;
            break;
        default:
            printf("Set to default 9x9.\n");
            boardSize = 9;
            boxSize = 3;
        }

}

void ChooseDifficulty() {

    int choice;

    const char* diff[] = {
        "*** Set difficulty ***\n",
        "1. NOVICE",
        "2. APPRENTICE",
        "3. MASTER"
    };

    PrintText(diff, 4);

    scanf("%d", &choice);

        switch (choice) {
    case 1:
        difficulty = 1;
        break;
    case 2:
        difficulty = 2;
        break;
    case 3:
        difficulty = 3;
        break;
    default:
        printf("Try again (1-3) >:c");
        }
}

int isValid(int row, int col, int n){

    for (int i = 0; i < boardSize; i++) { //check if number is only once in row
        if (board[row][i] == n) {
            return 0;
        }
    }

    for (int i = 0; i < boardSize; i++) { //check if number is only once in col
        if (board[i][col] == n) {
            return 0;
        }
    }

    int startRow = row - row % boxSize; //check if number is only once in box
    int startCol = col - col % boxSize;
    for (int i = 0; i < boxSize; i++) {
        for (int j = 0; j < boxSize; j++) {
            if (board[startRow + i][startCol + j] == n) {
                return 0;
            }
        }
    }

    return 1;
}

void FillBox(int rowStart, int colStart){

    int n;

    for(int i=0; i<boxSize; i++){
        for (int j=0; j<boxSize; j++){
            do{
                n = rand() % boardSize + 1;
            }while (!isValid(rowStart+i, colStart+j, n)); //keep checking until that number is valid  HAVE TO CHECK LATER IF IT IS NOT TOO SLOW

            board[rowStart+i][colStart+j]=n;
        }
    }
}

void FillDiagonal(){

     for (int i = 0; i < boardSize; i += boxSize) {
        FillBox(i, i);
    }
}

int FillBoard(int row, int col){


    if(row == boardSize - 1 && col == boardSize) {return 1;} //board is complete if true

    if(col == boardSize) { //check next row when last element of col is checked

        row++;
        col = 0;
    }

    if (board[row][col] != 0) { return FillBoard(row, col + 1); } //if spot already filled go to next

    for (int num = 1; num <= boardSize; num++) { // fill with numbers (from 1 to boardSize) ex [boardSize = 9]
        if (isValid(row, col, num)) {
            board[row][col] = num;

            if (FillBoard(row, col + 1)) { return 1; } //fill next boxes and return

            board[row][col] = 0; //if failed to fill change to 0
        }
    }

    return 0;
}

void CopyBoard(int boardToCopy[MAX_SIZE][MAX_SIZE], int board[MAX_SIZE][MAX_SIZE]){

    for (int i=0; i<boardSize; i++){
        for (int j=0; j<boardSize; j++){
            board[i][j] = boardToCopy[i][j];
        }
    }
}

void ZeroBoard(int board[MAX_SIZE][MAX_SIZE]){
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            board[i][j] = 0;
        }
    }
}


void InitBoard(){


    ZeroBoard(board); //fill with zeroes first
    ZeroBoard(startingBoard); //init of board with origianl numbers for the deleting mechanic

    FillDiagonal(); // First fill diagonals
    FillBoard(0, 0); // Then the rest

    CopyBoard(board, startingBoard);
}

void ApplyDifficultyToBoard(){

    int cellNum = boardSize*boardSize;
    int cellToDel;

    if ( difficulty == 1){ cellToDel = cellNum * 0.2;} //multipliers
    else if ( difficulty == 2){ cellToDel = cellNum * 0.6;}
    else { cellToDel = cellNum * 0.8; }


    int positions[cellNum]; //get array with all positons bcs earlier they could be deleted many times
    for (int i = 0; i < cellNum; i++) { positions[i] = i;}

    for (int i = cellNum - 1; i > 0; i--) { //shuffle the array

        int j = rand() % (i + 1);
        int temp = positions[i];

        positions[i] = positions[j];
        positions[j] = temp;
    }

    for (int i = 0; i < cellToDel; i++) { //zero selected spots

        int pos = positions[i];
        int row = pos / boardSize;
        int col = pos % boardSize;

        board[row][col] = 0;
    }
}

int GetSeed(){

    const char* text[] = {"Input game seed [0 for random game]: "};
    PrintText(text,1);

    scanf("%u", &seed);

    if(seed == 0){

        seed = (unsigned int)time(NULL); //random based on time so every game is different
    }
    srand(seed);

    return seed;
}


void initNewGame(){

    const char* settings[] = {"___SETTINGS___\n"};
    PrintText(settings, 1);

    //printf("\n___SETTINGS___\n");
    ChooseBoardSize();
    ChooseDifficulty();
    GetSeed();

    InitBoard();
    ApplyDifficultyToBoard();
    CopyBoard(board, startingBoard);

    StartGame();
}

void InitMenu(){

    int choice;

    PrintTitleScreen();

    do {
        const char* menu[] = {
            "___MAIN MENU___\n",
            "1. NEW GAME",
            "2. LOAD SAVE",
            "3. MANUAL",
            "4. EXIT"
        };

        PrintText(menu, 5);
        scanf("\n%d", &choice);

        switch(choice) {
            case 1:
                initNewGame();
                break;
            case 2:
                if(LoadGame("save.dat")){StartGame();}
                break;
            case 3:
                PrintManual();
                break;
            case 4:
                const char* gb[] = {"...Exiting"};
                PrintText(gb,1);
            default:
                printf("\n");
        }
    } while(choice != 4);

}

