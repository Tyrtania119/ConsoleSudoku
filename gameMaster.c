#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gameMaster.h"
#include "boardBuilder.h"
#include "Utils.h"
#include "UI.h"
#include "stats.h"

int kill = 0;

int isBoardComplete(){

    for(int i=0; i<boardSize; i++){
        for( int j=0; j<boardSize; j++){

            if ( board[i][j] == 0){return 0;}
        }
    }
    return 1;
}

void StartGame(){
    int row,col,n;

    //stats
    moveCounter =0;
    totalGameTime = 0;
    gameTime = time(NULL);
    kill=0;

    if (boardSize != 16){PrintShortManual();}
    else{Print16();}


    while (!isBoardComplete() && (kill!=1)){

        PrintBoard();

        printf("\n Enter your move [ row | column | number ]");

        if (scanf("%d %d %d", &row, &col, &n) != 3) { //to avoid bugs with spaces and invalid inputs
            printf("\n!!! Invalid input. Please enter three numbers separated by spaces !!!\n");
            while (getchar() != '\n');
            continue;
        }

        if (row == 0 && col == 0 && n == 0) {
            SaveGame("save.dat");
            printf("Game saved!\n");
            continue;
        }

        if (row == 9 && col == 9 && n == 9){

            ExitGame();
            kill=1;
            continue;
        }

        if (row == 1 && col == 1 && n == 1){

            PrintManual();
            continue;
        }

        row--;
        col--; //to index from 1 not 0

        if (row < -1 || row >= boardSize || col < -1 || col >= boardSize) {

            printf("\n !!!Invalid row or col position! Enter values between 1 and %d!!!\n", boardSize);
            continue;
        }

        if (n == -1) { //deleting mechanic

            if( startingBoard[row][col] != 0){

                const char* error[] = {"!!! You can only erase numbers you input !!!"};
                PrintText(error,1);
                continue;
            }

            if(board[row][col] == 0){

                const char* error[] = {"!!! That cell is already empty !!!"};
                PrintText(error,1);
                continue;
            }

            board[row][col] = 0;
            moveCounter++;
            printf("Number deleted at [R: %d|C: %d]", row+1, col+1);
            continue;
        }


        if (board[row][col] != 0){

            printf("\n !!!That cell is already filled!!!\n");
            continue;
        }

        if (!isValid(row, col, n)){

            printf("\n !!!Cannot place this number there!!!\n");
            continue;
        }



        board[row][col] = n;
        moveCounter++;
        printf("\a"); //sound fx :]]
    }

    if(kill != 1){ PrintWin(); }

}


