# Cell Simulator
2nd year assignment #3 - a.k.a. Conway's game of life
Key aspect - Object Oriented Programming (CLasses, Inheritance etc.)


Populations of cells are randomly distributed on a board of  20 x 75
User selects confluency percentage (percentage of board filled) and type of cell (normal cells - 'O' or cancer cells - 'X')

.exe files runs in command prompt and we progress step by step by hitting 'Enter'
At each new time instant, cell distribution refreshes based on each cell's neighbours on the previous time instant.
Different cell types have different requirements but outcome for each cell can be:
1. Death
  a) Overpopulation
  b) Loneliness
2. Birth
3. Stasis (cell state does not change)
