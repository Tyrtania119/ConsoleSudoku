#include <stdio.h>
#include <string.h>
#include <time.h>
#include "UI.h"
#include "boardBuilder.h"
#include "stats.h"

void PrintText(const char* lines[], int lineCount){

    int maxLen = 0;

    //find longest text
    for (int i = 0; i < lineCount; i++) {
        int len = strlen(lines[i]);
        if (len > maxLen) maxLen = len;
    }

    // add margin
    int margin = (SCREEN_WIDTH - maxLen) / 2;
    if (margin < 0) margin = 0;

    // print lines
    for (int i = 0; i < lineCount; i++) {
        for (int j = 0; j < margin; j++) printf(" ");
        printf("%s\n", lines[i]);
    }

}

void PrintSeparator(){

    const char* sep[] = {"========================================"};
    PrintText(sep,1);

}

void PrintStats(){

    char stats[100];
    time_t sessionTime = time(NULL) - gameTime;
    snprintf(stats, sizeof(stats), "Moves made: %u | Time spent playing: %lld seconds", moveCounter, totalGameTime+sessionTime);
    const char* statsText[]={stats};
    PrintText(statsText,1);

}


void PrintShortManual(){

    char seedString[50];
    snprintf(seedString, sizeof(seedString), "Game Seed: %u", seed);

    const char* manual[] = {
        "Input 0 0 0 to save",
        "Input 9 9 9 to exit",
        "Input 1 1 1 for help",
        "Input R C -1 to delete",
        seedString,
        " "
    };

    PrintSeparator();
    PrintText(manual, sizeof(manual) / sizeof(manual[0]));
}

void Print16(){

    const char* text[] = {
        "While playing on 16x16 board, hexadecimal number system is used.",
        "Number 1-9 are numerical, while 10-16 are A-G\n"
    };

    PrintShortManual();
    PrintText(text,2);
}

void PrintManual(){

    const char* title_01[] = {"HOW TO SOLVE SUDOKU?\n"};
    const char* manual[] = {
        "Sudoku involves filling a NxN grid with numbers from 1 to sqr of N, ensuring no repetition of numbers within each row, column",
        "and sqr(N) x sqr(N) block. The goal is to complete the grid so that every cell contains a unique digit, and each row, column",
        "and block contains all the numbers from 1 to sqr of N\n"
    };

    const char* title_02[] = {"HOW TO PLAY?\n"};
    const char* how_to[] = {
        "- Select new game in the main menu",
        "- Choose board size 4x4, 9x9 or 16x16",
        "- Choose difficulty. Higher difficulty means less shown numbers on the grid",
        "- Input your numbers into the grid in format [ Row number | Column number| Number to insert ]",
        "- Delete your numbers by inputing [ Row number | Column number | Number to delete ]",
        "- You can also save your session and finish it later :)",
        "- !!! Good luck, have fun !!!\n"
    };

    PrintText(title_01, 1);
    PrintText(manual, 3);
    PrintText(title_02, 1);
    PrintText(how_to, 6);
    PrintShortManual();
}

void PrintNumberInBox(int number){

    if(number==0){ printf(". ");}
    else if( number <= 9){ printf("%d ", number);}
    else { printf("%c ", 'A' + (number - 10));} //16 is G
}

void PrintTitleScreen() {
    const char* title[] = {
        ".................................................................................................................",
        ".................................................................................................................",
        "....@@@@@@........@@@@.....@@@@.......@@@@@@@@.............@@@@@@@@.........@@@@...@@@@......@@@@.....@@@@.......",
        ".@@@@.....@@.....@...@....@...@......@.......@@@@.......@@@@......@@@.......@..@@@@@........@...@@...@@..@.......",
        ".@....@@@........@...@@...@...@......@...@@@....@@.....@@....@@@@...@@......@...@@....@.....@...@@...@@..@.......",
        ".@...@@@@@@@.....@...@@...@...@......@...@@@@@...@.....@...@@@.@@@@..@@.....@..@@...@@@.....@...@@...@@..@.......",
        ".@@.....@@@@.....@...@@...@...@......@...@...@@..@.....@...@@....@@...@.....@.....@@@.......@...@@...@@..@.......",
        ".@@@@......@.....@...@@...@...@......@...@...@@..@.....@...@.....@@...@.....@.....@@@.......@...@@...@@..@.......",
        ".@@@@@@@@..@@....@...@@..@@...@......@...@..@@@..@.....@...@@...@@@..@@.....@..@@...@@......@...@@...@@..@.......",
        ".@..@@@@@..@.....@@...@@@@@...@......@...@@@@....@.....@@...@@@@@....@......@...@@...@@@....@@...@@@@@...@.......",
        ".@........@@......@@........@@.......@.........@@.......@@.........@@@......@...@@@..........@@........@@@.......",
        ".@@@@@@@@@@........@@@@@@@@@@........@@@@@@@@@@@.........@@@@@@@@@@@........@@@@..@@@@@@......@@@@@@@@@@.........",
        ".................................................................................................................",
        ".................................................................................................................",
    };

    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    PrintText(title, sizeof(title) / sizeof(title[0]));
}

void PrintKitty() {
    const char* kitty[] = {
        "                          ,''`.         _                        ",
        "                     ,.,'''  '`--- ._,,'|                        ",
        "                   ,'                   /                        ",
        "              __.-'                    |                         ",
        "           ''                ___   ___ |                         ",
        "         ,'                  \\(|\\ /|)/ |                         ",
        "        ,'                 _     _     `._                       ",
        "       /     ,.......-\\    `.      __     `-.                   ",
        "      /     ,' :  .:''`|    `:`.../:.`` ._   `._                ",
        "  ,..,'  _/' .: :'     |     |      '.    \\.    \\               ",
        " /      ,'  :'.:  / \\  |     | / \\   ':.  . \\    \\              ",
        "|      /  .: :' ,'  _)  \".._,;'  _)    :. :. \\    |             ",
        " |     | :'.:  /   |   .,   /   |       :  :  |   |             ",
        " |     |:' :  /____|  /  \\ /____|       :  :  |  ,'             ",
        "  |   /    '         /    \\            :'   : |,/              ",
        "   \\ |  '_          /______\\              , : |                ",
        "  _/ |  \\'`--`.    _            ,_   ,-'''  :.|         __      ",
        " /   |   \\..   ` ./ `.   _,_  ,'  ``'  /'   :'|      _,''/      ",
        "/   /'. :   \\.   _    [_]   `[_]  .__,,|   _....,--=/'  /:      ",
        "|   \\_| :    `.-' `.    _.._     /     . ,'  :. ':/'  /'  `.    ",
        "`.   '`'`.         `. ,.'   ` .,'     :'/ ':..':.    |  .:' `.  ",
        "  \\.      \\          '               :' |    ''''      ''     `.",
        "    `''.   `|        ':     .      .:' ,|         .  ..':.      |",
        "      /'   / '\"-..._  :   .:'    _;:.,'  \\.     .:'   :. ''.    |",
        "     (._,.'        '`''''''''''''          \\.._.:      ':  ':   /",
        "________                                      '`- ._    ,:__,,-' ",
        "  |ooShy                                            ``''         "
    };

    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    PrintText(kitty, sizeof(kitty) / sizeof(kitty[0]));
}

void PrintWin(){

    PrintSeparator();
    const char* win[] = { "!!! YOU WIN !!!"};
    PrintText(win, 1);
    PrintStats();
    PrintSeparator();
    PrintKitty();
}



void PrintBoard(){

    for (int i=0; i<3; i++){printf("\n");}

    int screenWidth = 250;
    int gridWidth = boardSize * 2 + (boardSize/boxSize - 1)*2; //space for number with one white sapce and space for | and --
    int margin = (screenWidth - gridWidth) / 2;

    for (int row = 0; row<boardSize; row++){

        //horizontal lines for readability
        //everywhere except for start
        if(row % boxSize==0 && row !=0){

            for(int i=0; i<margin; i++) {printf(" ");}
            for(int i=0; i< gridWidth; i++) {printf("-");}
            printf("\n");
        }

        //margins
        for(int i =0; i<margin; i++) {printf(" ");}

        for(int col=0; col<boardSize; col++){

            //Vertical lines
            if(col % boxSize == 0 && col !=0){printf("| ");}

            if(board[row][col] == 0){ printf(". ");}
            else{ PrintNumberInBox(board[row][col]);}
        }
        printf("\n");
    }
    printf("\n");
}


void PrintExitGame(){

    const char* text[] = {
        "GAME FORFEIT..."
    };

    PrintSeparator();
    PrintText(text, 1);
    PrintStats();
    PrintSeparator();
}
