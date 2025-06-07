#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Utils.h"
#include "UI.h"
#include "boardbuilder.h"

#define POPULATION_SIZE 100
#define NUM_GENERATIONS 10000
#define MUTATION_RATE 0.05f
#define CROSSOVER_RATE 0.8f

typedef enum {TOURNAMENT, ROULETTE, RANK} SelectionType; //diferent types for selection, crossover and mutation
typedef enum { SINGLE_POINT, MULTI_POINT, UNIFORM } CrossoverType;
typedef enum { SWAP_IN_ROW, SWAP_IN_COL, SWAP_IN_BOX } MutationType;

SelectionType selectionType = TOURNAMENT; //defaults
CrossoverType crossoverType = UNIFORM;
MutationType mutationType = SWAP_IN_ROW;

typedef struct {
    int** board;
    int fitness; //less value is better
} Individual; // one result

Individual population[POPULATION_SIZE]; // array of population_size

int calculateFitness(int** board) {

    int fitness = 0; //starting val

    for (int i = 0; i < boardSize; i++) { //count rwos and check for repeats in rows

        int* rowCount = calloc(boardSize + 1, sizeof(int));

        for (int j = 0; j < boardSize; j++) {
            rowCount[board[i][j]]++;
        }

        for (int n = 1; n <= boardSize; n++) {
            if (rowCount[n] > 1) fitness += rowCount[n] - 1;
        }

        free(rowCount);
    }

    for (int j = 0; j < boardSize; j++) { // count cols and check for repeats in cols

        int* colCount = calloc(boardSize + 1, sizeof(int));
        for (int i = 0; i < boardSize; i++) {
            colCount[board[i][j]]++;
        }

        for (int n = 1; n <= boardSize; n++) {
            if (colCount[n] > 1) fitness += colCount[n] - 1;
        }

        free(colCount);
    }

    for (int bi = 0; bi < boardSize; bi += boxSize) { // check for repeats in 3x3 boxes

        for (int bj = 0; bj < boardSize; bj += boxSize) {

            int* boxCount = calloc(boardSize + 1, sizeof(int));

            for (int i = 0; i < boxSize; i++) {

                for (int j = 0; j < boxSize; j++) {
                    boxCount[board[bi + i][bj + j]]++;
                }
            }
            for (int n = 1; n <= boardSize; n++) {
                if (boxCount[n] > 1) {
                    fitness += boxCount[n] - 1;
                }
            }

            free(boxCount);
        }
    }

    return fitness;
}

void initializePopulation() { //fill board with starting numbers from 1 to boardSize where number is 0

    for (int p = 0; p < POPULATION_SIZE; p++) {
        population[p].board = AllocBoard(boardSize); //alloc

        for (int i = 0; i < boardSize; i++) {

            for (int j = 0; j < boardSize; j++) {

                if (startingBoard[i][j] != 0) {

                    population[p].board[i][j] = startingBoard[i][j];
                } else {

                    population[p].board[i][j] = rand() % boardSize + 1;
                }
            }
        }
        population[p].fitness = calculateFitness(population[p].board); //get fitness for that set
    }
}

Individual tournamentSelection() { //selection between sets

    int a = rand() % POPULATION_SIZE;
    int b = rand() % POPULATION_SIZE;
    return population[a].fitness < population[b].fitness ? population[a] : population[b]; // return set with less fitness
}

Individual rouletteSelection() {

    int totalFitness = 0;

    for (int i = 0; i < POPULATION_SIZE; i++) { //SPIIINSS
        totalFitness += (1000 - population[i].fitness);
    }

    int pick = rand() % totalFitness; //pick random
    int sum = 0; //times of set picked

    for (int i = 0; i < POPULATION_SIZE; i++) {

        sum += (1000 - population[i].fitness);
        if (sum > pick) {return population[i]; }
    }

    return population[POPULATION_SIZE - 1];
}

int compareIndividuals(const void* a, const void* b) { //helper for ranking
    return ((Individual*)a)->fitness - ((Individual*)b)->fitness;
}

Individual rankingSelection() {

    qsort(population, POPULATION_SIZE, sizeof(Individual), compareIndividuals); //sort by fitness

    int totalRank = POPULATION_SIZE * (POPULATION_SIZE + 1) / 2;
    int pick = rand() % totalRank;
    int sum = 0;

    for (int i = 0; i < POPULATION_SIZE; i++) {

        sum += (POPULATION_SIZE - i);
        if (sum >= pick) { return population[i]; }
    }

    return population[0]; // return first in list bcs its the best
}

Individual selectParent() { //check which setting whas chosen

    if (selectionType == TOURNAMENT) return tournamentSelection();
    if (selectionType == ROULETTE) return rouletteSelection();
    return rankingSelection();
}


void crossoverSinglePoint(Individual* child, Individual p1, Individual p2) {

    int point = rand() % (boardSize * boardSize); //cross point

    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {

            int idx = i * boardSize + j;
            child->board[i][j] = (idx < point) ? p1.board[i][j] : p2.board[i][j]; //crossing
        }
    }
}

void crossoverMultiPoint(Individual* child, Individual p1, Individual p2) {

    for (int i = 0; i < boardSize; i++) {
        int useParent1 = rand() % 2;

        for (int j = 0; j < boardSize; j++) {
            child->board[i][j] = useParent1 ? p1.board[i][j] : p2.board[i][j];
        }
    }
}

void crossoverUniform(Individual* child, Individual p1, Individual p2) {

    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {

            child->board[i][j] = (rand() % 2) ? p1.board[i][j] : p2.board[i][j]; //random cross
        }
    }
}

void applyCrossover(Individual* child, Individual p1, Individual p2) { //apply chosen crossover

    if (crossoverType == SINGLE_POINT) crossoverSinglePoint(child, p1, p2);
    else if (crossoverType == MULTI_POINT) crossoverMultiPoint(child, p1, p2);
    else crossoverUniform(child, p1, p2);
}

void mutateRowSwap(Individual* indiv) { //mutation by rows

    for (int i = 0; i < boardSize; i++) {

        int j1 = rand() % boardSize;
        int j2 = rand() % boardSize;

        if (startingBoard[i][j1] == 0 && startingBoard[i][j2] == 0 && ((float)rand() / RAND_MAX) < MUTATION_RATE) {

            int temp = indiv->board[i][j1];
            indiv->board[i][j1] = indiv->board[i][j2];
            indiv->board[i][j2] = temp; // randomly swap bit in row
        }
    }
}

void mutateColumnSwap(Individual* indiv) {

    for (int j = 0; j < boardSize; j++) {

        int i1 = rand() % boardSize;
        int i2 = rand() % boardSize;

        if (startingBoard[i1][j] == 0 && startingBoard[i2][j] == 0 && ((float)rand() / RAND_MAX) < MUTATION_RATE) {

            int temp = indiv->board[i1][j];
            indiv->board[i1][j] = indiv->board[i2][j];
            indiv->board[i2][j] = temp;
        }
    }
}

void mutateBlockSwap(Individual* indiv) {

    for (int bi = 0; bi < boardSize; bi += boxSize) {
        for (int bj = 0; bj < boardSize; bj += boxSize) {

            int i1 = rand() % boxSize;
            int j1 = rand() % boxSize;
            int i2 = rand() % boxSize;
            int j2 = rand() % boxSize;
            int x1 = bi + i1, y1 = bj + j1, x2 = bi + i2, y2 = bj + j2;

            if (startingBoard[x1][y1] == 0 && startingBoard[x2][y2] == 0 && ((float)rand() / RAND_MAX) < MUTATION_RATE) {
                int temp = indiv->board[x1][y1];
                indiv->board[x1][y1] = indiv->board[x2][y2];
                indiv->board[x2][y2] = temp;
            }
        }
    }
}

void mutate(Individual* indiv) { //main muatuion

    switch (mutationType) {

        case SWAP_IN_ROW:
            for (int i = 0; i < boardSize; i++) {
                if (((float)rand() / RAND_MAX) < MUTATION_RATE) {

                    int a = rand() % boardSize;
                    int b = rand() % boardSize;
                    while (startingBoard[i][a] != 0 || startingBoard[i][b] != 0) {
                        a = rand() % boardSize;
                        b = rand() % boardSize;
                    }
                    int tmp = indiv->board[i][a];
                    indiv->board[i][a] = indiv->board[i][b];
                    indiv->board[i][b] = tmp;
                }
            }
            break;

        case SWAP_IN_COL:
            for (int j = 0; j < boardSize; j++) {
                if (((float)rand() / RAND_MAX) < MUTATION_RATE) {

                    int a = rand() % boardSize;
                    int b = rand() % boardSize;
                    while (startingBoard[a][j] != 0 || startingBoard[b][j] != 0) {
                        a = rand() % boardSize;
                        b = rand() % boardSize;
                    }
                    int tmp = indiv->board[a][j];
                    indiv->board[a][j] = indiv->board[b][j];
                    indiv->board[b][j] = tmp;
                }
            }
            break;

        case SWAP_IN_BOX:
            for (int bi = 0; bi < boardSize; bi += boxSize) {
                for (int bj = 0; bj < boardSize; bj += boxSize) {

                    if (((float)rand() / RAND_MAX) < MUTATION_RATE) {
                        int i1, j1, i2, j2;
                        do {

                            i1 = bi + rand() % boxSize;
                            j1 = bj + rand() % boxSize;
                            i2 = bi + rand() % boxSize;
                            j2 = bj + rand() % boxSize;

                        } while (startingBoard[i1][j1] != 0 || startingBoard[i2][j2] != 0);
                        int tmp = indiv->board[i1][j1];
                        indiv->board[i1][j1] = indiv->board[i2][j2];
                        indiv->board[i2][j2] = tmp;
                    }
                }
            }
            break;
    }
}

void applyMutation(Individual* indiv) {
    if (mutationType == SWAP_IN_ROW) mutateRowSwap(indiv);
    else if (mutationType == SWAP_IN_COL) mutateColumnSwap(indiv);
    else mutateBlockSwap(indiv);
}

void geneticSolver() {

    initializePopulation(); //init population
    Individual best; // init for best result
    best.board = AllocBoard(boardSize);
    best.fitness = 10000; //set big fitness value for best

    for (int gen = 0; gen < NUM_GENERATIONS; gen++) { //for each generation

        Individual newPop[POPULATION_SIZE]; //init new population

        for (int i = 0; i < POPULATION_SIZE; i++) {

            newPop[i].board = AllocBoard(boardSize);
            Individual p1 = selectParent(); //select 2 parents from poulations
            Individual p2 = selectParent();

            if (((float)rand() / RAND_MAX) < CROSSOVER_RATE) {applyCrossover(&newPop[i], p1, p2);} // cross randmly
            else { CopyBoard(p1.board, newPop[i].board); } // copy

            applyMutation(&newPop[i]); //mutation
            newPop[i].fitness = calculateFitness(newPop[i].board); //get fitness for this gen

            if (newPop[i].fitness < best.fitness) { //check if gen is better
                best.fitness = newPop[i].fitness;
                CopyBoard(newPop[i].board, best.board);
            }
        }

        for (int i = 0; i < POPULATION_SIZE; i++) { //swap old population with new

            DisallocBoard(population[i].board, boardSize);
            population[i] = newPop[i];
        }

        if (best.fitness == 0) break; //if fitness is 0 that means its the best
    }

    printf("Genetic Solving Done - Best fitness found: %d\n", best.fitness);

    PrintBoard(best.board);
    DisallocBoard(best.board, boardSize);
}

void SettingsGeneticSolver() {
    int choice;

    printf("Wybierz typ selekcji:\n");
    printf("1. Turniej\n2. Ruletka\n3. Ranking\n> ");
    scanf("%d", &choice);
    selectionType = (choice == 2) ? ROULETTE : (choice == 3) ? RANK : TOURNAMENT;

    printf("Wybierz typ krzy¿owania:\n");
    printf("1. Jednopunktowe\n2. Wielopunktowe\n3. Jednorodne\n> ");
    scanf("%d", &choice);
    crossoverType = (choice == 1) ? SINGLE_POINT : (choice == 2) ? MULTI_POINT : UNIFORM;

    printf("Wybierz typ mutacji:\n");
    printf("1. Zamiana w wierszu\n2. Zamiana w kolumnie\n3. Zamiana w bloku\n> ");
    scanf("%d", &choice);
    mutationType = (choice == 2) ? SWAP_IN_COL : (choice == 3) ? SWAP_IN_BOX : SWAP_IN_ROW;

    printf("\nUstawienia:\nSelekcja: %d, Krzy¿owanie: %d, Mutacja: %d\n\n", selectionType, crossoverType, mutationType);
}
