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

void initializePopulation() { //fill board with starting numbers

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

void crossover(Individual* child, Individual parent1, Individual parent2) { //randomly mixes set_1 with set_2
    for (int i = 0; i < boardSize; i++) {

        for (int j = 0; j < boardSize; j++) {

            child->board[i][j] = (rand() % 2 == 0) ? parent1.board[i][j] : parent2.board[i][j];
        }
    }
}

void mutate(Individual* indiv) { //for numbers that are 0 sets new number with given mutation_rate
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {

            if (startingBoard[i][j] == 0 && ((float)rand() / RAND_MAX) < MUTATION_RATE) {
                indiv->board[i][j] = rand() % boardSize + 1;
            }
        }
    }
}

void geneticSolver() { //main function

    initializePopulation(); // init population
    Individual best; // init struct for best set
    best.board = AllocBoard(boardSize); // alloc
    best.fitness = 10000; // high fitness for more loops

    for (int gen = 0; gen < NUM_GENERATIONS; gen++) {

        Individual newPop[POPULATION_SIZE]; //create new population

        for (int i = 0; i < POPULATION_SIZE; i++) {

            newPop[i].board = AllocBoard(boardSize);
            Individual p1 = tournamentSelection(); // choose partent1
            Individual p2 = tournamentSelection(); // choose partent2

            if (((float)rand() / RAND_MAX) < CROSSOVER_RATE) {
                crossover(&newPop[i], p1, p2); // mix partents with probability crossover_rate or copy parent 1 board to new population
            } else {
                CopyBoard(p1.board, newPop[i].board);
            }

            mutate(&newPop[i]); //mutate child
            newPop[i].fitness = calculateFitness(newPop[i].board); //get fitness for that child

            if (newPop[i].fitness < best.fitness) { //if child has better fitness than parent -> update best result

                best.fitness = newPop[i].fitness;
                CopyBoard(newPop[i].board, best.board);
            }
        }

        for (int i = 0; i < POPULATION_SIZE; i++) { //release memorty

            DisallocBoard(population[i].board, boardSize);
            population[i] = newPop[i];
        }

        if (best.fitness == 0) break; //if fitness is 0 than its the best result
    }

    CopyBoard(best.board, board);

    printf("Genetic Solver Complete. Best fitness: %d\n", best.fitness);
    PrintBoard(best.board);

    DisallocBoard(best.board, boardSize);
}
