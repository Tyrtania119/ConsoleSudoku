#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "boardBuilder.h"
#include "Utils.h"
#include "UI.h"
#include "gameMaster.h"

int foundOptimal = 0;

int** frozenBoard = NULL; //if 0 then number can be inserted

typedef struct{

    int** board;
    int energy; // koszt, czyli liczba konfliktow. Jak duzo to rozwiazanie jest zle
}BoardState;

int CalculateCost(int** board){ //counts conflicts in rows and cols

    int energy = 0;

    for(int i=0; i<boardSize; i++){ //check rows

        int* _count = calloc(boardSize + 1, sizeof(int)); // count quantity of every number i row [_i]

        for(int j=0; j< boardSize; j++){

            _count[board[i][j]]++; // count numbers in row
        }
        for(int k=1; k<=boardSize; k++){

            if(_count[k]>1) {energy += _count[k] - 1;} // if _k appears more than once then add to energy (cost)
        }

        free(_count);
    }

    for (int i = 0; i < boardSize; i++) { //check cols

        int* _count = calloc(boardSize + 1, sizeof(int));

        for (int j = 0; j < boardSize; j++) {

            _count[board[j][i]]++;
        }
        for (int k = 1; k <= boardSize; k++) {

            if (_count[k] > 1) {

                energy += _count[k] - 1;
            }
        }
        free(_count);
    }

    for (int block_row = 0; block_row < boxSize; block_row++) { //check in boxes
        for (int block_col = 0; block_col < boxSize; block_col++) {

        int* _count = calloc(boardSize + 1, sizeof(int));

        for (int i = 0; i < boxSize; i++) {
            for (int j = 0; j < boxSize; j++) {

                int row = block_row * boxSize + i;
                int col = block_col * boxSize + j;
                _count[board[row][col]]++;
            }
        }

        for (int k = 1; k <= boardSize; k++) {
            if (_count[k] > 1) {
                energy += _count[k] - 1;
            }
        }

        free(_count);
    }
    }
    return energy;
}

//Fills every 3x3 block with randomomly placed numbers and checks if frozen and checks starting energy(cost)
void InitGrids(BoardState* state, int** input){ //init 3x3 grids with unique numbers

    frozenBoard = AllocBoard(boardSize);

    state->board = AllocBoard(boardSize);

    if(state->board == NULL) { printf("\n Alloc failed at 64\n");}

    for(int i=0; i<boardSize; i++){
        for(int j=0; j<boardSize; j++){
            state->board[i][j] = input[i][j]; //copy states
        }
    }


    for(int block_row=0; block_row < boxSize; block_row++){

        for( int block_col=0; block_col < boxSize; block_col++){

            int _current[boardSize+1]; //numbers that are already in one 3x3 block
            int _emptyCells[boardSize][2]; //has row col of empty cells in 3x3 block
            int _emptyCount = 0; // count of empty cells in 3x3 block

            memset(_current, 0, sizeof(_current));

            for( int i=0; i<boxSize; i++){
                for(int j=0; j<boxSize; j++){

                    int row = block_row * boxSize + i; //to go beyond 3x3 a and check every block in grid
                    int col = block_col * boxSize + j;
                    int num = input[row][col]; //value in that cell

                    if(num != 0){ // check if cell is already filled

                        _current[num] = 1; //mark that number as already used in block
                        frozenBoard[row][col] = 1; //mark that cell as frozen so it cant be changed
                    }else{

                        _emptyCells[_emptyCount][0] = row;
                        _emptyCells[_emptyCount][1] = col;
                        _emptyCount++; //if num is 0 then save its row and col
                    }
                }
            }

            int fillerArray[boardSize]; //array for numbers that werent used yet
            int index = 0;

            for(int i=1; i<=boardSize; i++) {
                if(!_current[i]) { fillerArray[index++] = i; } //fill with number that werent used yet
            }

            for (int i=0; i<index; i++){

                int random = i+rand() % (index - i);
                int temp = fillerArray[i];
                fillerArray[i] = fillerArray[random];
                fillerArray[random] = temp; //randomize filled numbers
            }

            for( int i=0; i<_emptyCount; i++){

                int row = _emptyCells[i][0];
                int col = _emptyCells[i][1];
                state->board[row][col] = fillerArray[i];  //fill with next number from fill[]
            }
        }
    }

    state->energy = CalculateCost(state->board); //check the start cost of that board config
}

//Generates new state, swaps 2 numbers in 3x3 block and checks the cost of that new state
void GenerateNeighbour(BoardState* neighbour, const BoardState* current_state){ //neighbour is a copy state of current to modify

    neighbour->board = AllocBoard(boardSize);

    if(neighbour->board == NULL){ printf("\n Alloc failed at 132");}

    for (int i = 0; i < boardSize; i++)
        for (int j = 0; j < boardSize; j++)
            neighbour->board[i][j] = current_state->board[i][j]; //copy

    int _block_row = rand() % boxSize; //randomize which block from grid will be modified
    int _block_col = rand() % boxSize;

    int _editable[boardSize][2]; //Array of cells that are not frozen
    int _count = 0; //Count how many editable cells in block

    for(int i=0; i<boxSize; i++){
        for(int j=0; j<boxSize; j++){

            int _row = _block_row * boxSize + i; //row in col in grid
            int _col = _block_col * boxSize + j;

            if( !frozenBoard[_row][_col]){ //check if cell can be edited

                _editable[_count][0] = _row;
                _editable[_count][1] = _col;
                _count++; //save row and col of editable cell
            }
        }
    }

    if ( _count >= 2 ){ //check if call has a neighbour

        int _a = rand() % _count;
        int _b; //randomize 2 numbers

        do{
            _b = rand() % _count;
        }while ( _a == _b); //check if numbers are different

        int _row1 = _editable[_a][0]; //check row and col of first neighbour
        int _col1 = _editable[_a][1];

        int _row2 = _editable[_b][0]; //check row and col of second neighbour
        int _col2 = _editable[_b][1];

        int _temp = neighbour->board[_row1][_col1];
        neighbour->board[_row1][_col1] = neighbour->board[_row2][_col2];
        neighbour->board[_row2][_col2] = _temp; //swap numbers between neighbours

        neighbour->energy = CalculateCost(neighbour->board); //check cost of new board state
    }
}

void Solve(int** input, double temp_start, double temp_end, double alpha, int max_iter){

    if (frozenBoard != NULL) {
        DisallocBoard(frozenBoard, boardSize);
        frozenBoard = NULL;
    }


    BoardState _current;
    BoardState _neighbour;
    BoardState _neighbour_best;

    InitGrids(&_current, input);

    _neighbour_best.board = AllocBoard(boardSize);

    CopyBoard(_current.board, _neighbour_best.board);

    _neighbour_best.energy = _current.energy;

    double _temp_current = temp_start;
    int _iteration = 0;

    if(debugMode){printf("Start energy: %d\n", _current.energy); }


    while ( _temp_current > temp_end && _neighbour_best.energy > 0){ //Generate new neighbours until a valid one

        GenerateNeighbour(&_neighbour, &_current);

        int _delta_E = _neighbour.energy - _current.energy;  //diff in cost

        if (_delta_E < 0 ){ //Check neighbour with better cost and accept OR calculate probability and check if its valid

            DisallocBoard(_current.board, boardSize);
            _current.board = _neighbour.board;
            _current.energy = _neighbour.energy;

        }else{

            double probability = exp(-_delta_E / _temp_current);

            if ((double)rand() / RAND_MAX < probability) {

                DisallocBoard(_current.board, boardSize);
                _current.board = _neighbour.board;
                _current.energy = _neighbour.energy;
            } else {
                DisallocBoard(_neighbour.board, boardSize);
            }
        }


        if (_current.energy < _neighbour_best.energy){ //update best result

            DisallocBoard(_neighbour_best.board, boardSize);

            _neighbour_best.board = AllocBoard(boardSize);

            for(int i=0; i<boardSize; i++){
                for( int j=0; j<boardSize; j++){

                    _neighbour_best.board[i][j] = _current.board[i][j];
                }
            }
            _neighbour_best.energy = _current.energy;
        }

        _temp_current *= alpha; //cooling
        _iteration++;

        if(debugMode){ printf("Iter: %d, T: %.4f, Current Cost: %d, Best Cost: %d\n", _iteration, _temp_current, _current.energy, _neighbour_best.energy), printf("\nFinished\n");; }

    }

    if (_neighbour_best.energy == 0) {
        PrintText((const char*[]){"Found optimal Solution:\n"}, 1);
        foundOptimal = 1;

    } else {
        PrintText((const char*[]){"Optimal Solution not found:\n"}, 1);

    }

    printf("En: %d",_current.energy);
    PrintText((const char*[]){"Solution:\n"}, 1);
    PrintSeparator();
    PrintBoard(_neighbour_best.board);
    PrintSeparator();


    DisallocBoard(_current.board, boardSize);
    DisallocBoard(_neighbour_best.board, boardSize);
    DisallocBoard(frozenBoard, boardSize);
    frozenBoard = NULL;
}

