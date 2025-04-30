# ConsoleSudoku

![Title Screen](gifs/Title.JPG)

**Sudoku console game in C**

------

## MANUAL

Input number in format:  
`ROW COL NUMBER`  
Delete number in format:  
`ROW COL -1`

### SPECIAL COMMANDS:
- `0 0 0` – SAVE  
- `0 0 1` – HELP  
- `0 0 9` – EXIT

You can turn on dev mode in main menu - shows allocs and disallocs of the board 
When the game ends, player is shown their stats - number of moves (valid and invalid) and time spent playing

**The player is rewarded for winning :]**

------

### NEW GAME

- Choose *board size*: `4x4`, `9x9`, `16x16`  
- In `16x16` board game uses hexadecimal system  
  **(16 = G, not 10 for better readibility)**  
  Input numbers in decimal format. Example - 10 instead of A

- Choose *difficulty*:  
  `Novice`, `Apprentice`, `Master`  
  The higher difficulty, the less numbers shown on the starting board

- Set *seed*:  
  `0` = random seed  
  Other than 0 = seeded session

-----

## NEW GAME GAMEPLAY (WITH DEBUG MODE ON)

### Gameplay shows:

- Debug mode on:
  - Can switch `ON`/`OFF` in main menu
  - Shows alloc and disalloc 

- Valid inputs: 
  - Plays sound
  - Updates and prints board

- Invalid inputs:
  - Shows error depending on mistake made

- Deleting inputs

- Special inputs:
  - Shows *Save* mechanic
  - Shows *Help* info

- On win:
  - Shows stats `number of moves`, `time`
  - Shows *reward*
  - App doesn't close and shows Main Menu 


![Gameplay](gifs/Sudoku_Gameplay.gif)

-----

## LOADING SAVE

### Gameplay shows:

- Loading save
  - All stats and progress made are also loaded
  - If board is 16x16 shows special instruction

- Shows 16x16 grid with hexadecimal number system

- Inputs:
  - Inputs are regular decimal numbers

- Special input:
  - *Exit* input shows stats and end game screen

You can save and later load your session. Even after closing console.
Stats before save and after loading are summed on win.

![Loading](gifs/Sudoku_Loading.gif)

---

## Bugs and things to fix

- If player gives inputs (`R` `Enter` `C` `Enter` `N`), instead of one line, they can loose track of what was already input:
  ➤ **Add tracker of inputs**

- Player can save only one session at a time:
  ➤ **Make multi save system**

- Folder structure xDD

- All other bugs I don't know about :]

---

## Majóweczka

**Me af:**  
![Majóweczka](gifs/Meme.gif)
