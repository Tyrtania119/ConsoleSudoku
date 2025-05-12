#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "boardBuilder.h"
#include "UI.h"
#include "stats.h"
#include "gameMaster.h"

//need to save boardSize, boxSize, diff, and current board[][]

void SaveGame(const char *filename){


    FILE *file = fopen(filename, "wb");

    if(!file){ printf("!!! File not found !!! \n");
        return; //return byl poza funckajxD
    }

    time_t currentTime = time(NULL); //to later get duration of game
    totalGameTime += (currentTime - gameTime); //add to this session
    gameTime = currentTime;

    fwrite(&boardSize, sizeof(int), 1, file); //what to save | size | num of elements to save| stream to save
    fwrite(&boxSize, sizeof(int), 1, file);
    fwrite(&difficulty, sizeof(int), 1, file);
    fwrite(&seed, sizeof(unsigned int), 1, file);
    fwrite(&moveCounter, sizeof(unsigned int), 1, file);
    fwrite(&totalGameTime, sizeof(time_t), 1 , file);



    for (int i=0; i<boardSize; i++){
        fwrite(board[i], sizeof(int), boardSize, file); //in loop bcs only first row was being saved bcs array is 2 dimensiomal
    }
    fclose(file);

    const char* info[] = {"*** Save succesful ***\n"};
    PrintText(info, 1);

}

int LoadGame(const char *filename){

    ClearConsole();

    if(debugMode){printf("Opening file...");}

    FILE *file = fopen(filename, "rb");

    if(!file){
       printf("!!! File not found !!! \n");
        return 0;
    }



    fread(&boardSize, sizeof(int), 1, file); //what to save | size | num of elements to save| stream to save

    board = malloc(boardSize * sizeof(int*)); //have to alloc bcs wont load when new session
    for (int i = 0; i < boardSize; i++) {
    board[i] = malloc(boardSize * sizeof(int));
    }

    fread(&boxSize, sizeof(int), 1, file);
    fread(&difficulty, sizeof(int), 1, file);
    fread(&seed, sizeof(unsigned int), 1, file);
    fread(&moveCounter, sizeof(unsigned int), 1, file);
    fread(&totalGameTime, sizeof(time_t), 1 , file);

    gameTime = time(NULL); //new session

    for (int i = 0; i < boardSize; i++) {
        fread(board[i], sizeof(int), boardSize, file); //also loop here :]
    }

    fclose(file);

    const char* info[] = {"*** Load successful ***\n"};
    PrintText(info, 1);

    return 1;
}

int** AllocBoard(int boardSize){

    int** board = malloc(boardSize * sizeof(int*));

    for(int i=0; i<boardSize; i++){
        board[i] = malloc(boardSize * sizeof(int));
    }

    if(debugMode){printf("[DEBUG] Allocating board of size %dx%d\n", boardSize, boardSize);}

    return board;
}

void DisallocBoard(int** board, int boardSize){

    for(int i=0; i<boardSize; i++){

        free(board[i]); //Firt elements of board then the board itself
    }

    if(debugMode){printf("[DEBUG] Disallocating board of size %dx%d\n", boardSize, boardSize);}
    free(board);
}


int ExitGame(){

    PrintExitGame();
    return 0;
}
