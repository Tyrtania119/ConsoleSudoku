# ConsoleSudoku
Sudoku console game in C.

## Instruction

Input your numbers in [ ROW | COLUMN | NUMBER ] format.
Delete your number in [ ROW | COLUMN | -1 ] format.
SPECIAL INPUTS:
* 0 0 0 - SAVE 
* 0 0 1 - HELP
* 0 0 9 - EXIT


You can enable dev mode in main menu. It shows allocation and disallocation of the board.


## New Game gameplay

Player can choose size of board (4x4, 9x9 or 16x16).
When playing on 16x16 board game uses hex number system [Important 16=G not 10 for readibility].

Player can choose difficulty (Novice, Apprentice, Master).
The higher difficulty, the less numbers shown on starting board.

Player can input seed to generate board
When input seed = 0 the board is generated randomly.

Example gameplay with debug mode on to show allocation
Game plays sound on successful input

![Gameplay](gifs/Sudoku_Gameplay.gif)

## Loaded save gameplay
Game allows to save session along with stats
Last save can be loaded 
Stats from before saved and after 
resumed session are combined and displayed on win

![Load](gifs/Sudoku_Loading.gif)

## Known bugs, things to improve

*During game when player inputs numberOfRow <Enter> numberOfCol <Enter> number instead of R|C|N in one line
they can lose track of what was already input
	-> ADD TRACKER TO SHOW PLAYER ROW COL NUMBER SELECTED

*Only one save can be made and loaded
	-> CHANGE SAVING SYSTEM SO PLAYER CAN SAVE MANY FILES

*Folder hierarchy xD


## Majóweczka :]

Me af: 
![Me af:](gifs/Meme.gif)
