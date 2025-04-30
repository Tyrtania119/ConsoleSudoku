# ConsoleSudoku
Sudoku console game in C

##New Game gameplay
Example gameplay with debug mode on to show allocation
Game plays sound on successful input

![Gameplay](gifs/Sudoku_Gameplay.gif)

##Loaded save gameplay
Game allows to save session along with stats
Last save can be loaded 
Stats from before saved and after 
resumed session are combined and displayed on win

![Load](gifs/Sudoku_Loading.gif)

##Known bugs, things to improve

-During game when player inputs numberOfRow <Enter> numberOfCol <Enter> number instead of R|C|N in one line
they can lose track of what was already input
	-> ADD TRACKER TO SHOW PLAYER ROW COL NUMBER SELECTED

-Only one save can be made and loaded
	-> CHANGE SAVING SYSTEM SO PLAYER CAN SAVE MANY FILES
