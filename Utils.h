#ifndef UTILS_H
#define UTILS_H

void SaveGame(const char *filename);
int LoadGame(const char *filename);
int** AllocBoard(int boardSize);
void DisallocBoard(int** board, int boardSize);
#endif
