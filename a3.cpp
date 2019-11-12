/*=====================================*\
||  Assignment:   3. Cell Simulator    ||
||  Author:       Andreas Polydorides  ||
||  CID:          01390230             ||
||  Deadline:     14 December 2018     ||
\*=====================================*/

#include <iostream>
#include <string>  //for string implementation
#include <cstdlib> //for rand() implementation
#include <vector>  //for vector implementation

using namespace std;

//  --  Creating the Cell Class  --

class Cell
{
protected:    //private would not allow use of the variables in classes that inherit from this class
  bool state; //true for alive cell and false for dead cell

public:
  //Constructors and destructors
  Cell()
  {
    this->state = false;
  };
  Cell(bool state)
  {
    this->state = state;
  };
  ~Cell(){};

  //Functions
  bool getState() { return state; };
  void giveState(bool newState) { state = newState; };
  virtual bool decide(int neighbours); //counts # of neighbours and changes the state of cell at point (x,y)
  virtual char cellCharacter()             // virtual as it will be used for Cancer
  {
    return 'O'; // per specification character for Normal Cells is set to 'O'
  };
};

bool Cell::decide(int neighbours)
{
  bool temp = state;
  //Based on the number of neighbours, the fate of the cell is decided
  //Death: Overpopulation (alive and surrounded by 4 or more)
  //Death: Loneliness (alive and with at most 1 neighbour)
  if (temp && ((neighbours >= 4) || (neighbours<=1)))
  {
    temp = false;
  }
  
  //Birth
  else if (!temp && (neighbours == 3))
  {
    temp = true;
  }

  return temp;
}
//  -- End of Cell Class creation --  //


//  --  Creating the derived Cancer Cell Class  --  //
class Cancer : public Cell
{ //this derived class inherits from Class Cell
public:
  virtual bool decide(int neighbours);
  virtual char cellCharacter()
  {
    return 'X'; // per specification character for Cancer Cells is set to 'X'
  };
};

bool Cancer::decide(int neighbours)
{
  bool temp = state;
  //Based on the number of neighbours, the fate of the cancer cell is decided
  //Death: Overpopulation (alive and surrounded by 5 or more)
  //Death: Loneliness (alive and with at most 1 neighbour)
  if (temp && ((neighbours >= 5) || (neighbours<=1)))
  {
    temp = false;
  }
  
  //Birth
  else if (!temp && (neighbours == 3))
  {
    temp = true;
  }

  return temp;
}
//  --  End of derived Cancer Class creation  -- //

//  --  Creating the Board Class  --  //
class Board
{
private:
  int rows;
  int cols;
  int time;
  int cellType; // 1 for Cell  &  2 for Cancer
  //using pointer to address of object in vector to avoid issues resulting
  //from a change of address when the vector data is manipulated
  vector<vector<Cell *>> board;
  //making a spare board so as to pass the new states
  vector<vector<Cell *>> newBoard;

public:
  //constructor and deconstructor
  Board(int rows, int cols) //given 2D board size
  {
    this->rows = rows;
    this->cols = cols;
    this->time = 0;
    this->cellType = 1;
  };
  ~Board(){};

  void seed_cells(int selection, int confluence);
  int get_num_cells();
  void next_state();
  int get_time();
  void display();
};

void Board::seed_cells(int selection, int confluence)
{
  board.clear();
  srand(1);
  int coverage = 0.01 * confluence * rows * cols; //maximum number of cells to be strict with the confluency percentage
  int current = 0;                                //counter variable- increments with every cell placed
  cellType = selection;

  if (cellType == 1)
  {
    // initialising a board with a cell type Normal
    // creating the row vectors by passing pointers to Normal Cell objects
    for (int i = 0; i < rows; i++)
    {
      vector<Cell *> boardRow;
      for (int j = 0; j < cols; j++)
      {
        Cell *c = new Cell;
        boardRow.push_back(c);
      }
      // making the 'column' vector that contains the vectors created above
      board.push_back(boardRow); //pushes a vector into another vector's element
    }
  }
  else if (cellType == 2)
  {
    // initialising a board board with a cell type Cancer
    // creating the row vectors by passing pointers to Cancer Cell objects
    for (int i = 0; i < rows; i++)
    {
      vector<Cell *> boardRow;
      for (int j = 0; j < cols; j++)
      {
        Cell *c = new Cancer;
        boardRow.push_back(c);
      }
      // creating the 'column' vector that contains the vectors created above
      board.push_back(boardRow); //pushes a vector into another vector's element
    }
  }

  //same code as before- fill the newBoard with objects of class cell or cancer depending on selection
  for (int i = 0; i < rows; i++)
  {
    vector<Cell *> newBoardRow;
    for (int j = 0; j < cols; j++)
    {
      if (cellType == 1)
      {
        Cell *n = new Cell;
        newBoardRow.push_back(n);
      }
      else if (cellType == 2)
      {
        Cell *n = new Cancer;
        newBoardRow.push_back(n);
      }
    }
    newBoard.push_back(newBoardRow);
  }

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      *(newBoard[i][j]) = *(board[i][j]);
    }
  }

  //There are 2 methods to laying out the Cells in a random order.
  //1 is taking its position and giving it a confluency% chance of having a cell
  //2 is randomising the coordinates and placing a cell there as long as it is empty
  //Using method 2 as it gives EXACT percentage as per the requirements:
  while (current < coverage)
  {
    int row = rand() % rows; //generates random number from 0 to rows-1
    int col = rand() % cols; //same as above for cols
    if (!board[row][col]->getState())
    {
      board[row][col]->giveState(true);
      current++;
    }
  }
}

int Board::get_num_cells()
{
  int cells = 0;
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      if ((*board[i][j]).getState()) //board[i][j]->getState() would also work (value of)
        cells++;
    }
  }
  return cells;
}

// -- Calculates the next state for each element on the board -- //
void Board::next_state()
{
  //increment time
  time++;

  //Since we have two identical boards,
  //we can use the states of the previous one to update the new one
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      //firstly find the number of neighours
      int counter = 0; //will be incremented for each neighbour found
      for (int k = -1; k < 2; k++)
      {
        for (int m = -1; m < 2; m++)
        {
          if ((i + k) >= 0 && (i + k) < rows && (j + m) >= 0 && (j + m) < cols && board[i + k][j + m]->getState())
          { //if bool is true (cell exists) then counter incremented
            counter++;
          }
        }
      }
      //need to handle the case where the mid-point (cell we are checking for) is alive or dead
      if (board[i][j]->getState()) // if statement prevents a counter value of -1
        counter--;

      //Using the Cell Class decide function we use the neighbours to decide whether a cell will live or die
      newBoard[i][j]->giveState(board[i][j]->decide(counter));
    }
  }
  //Finally, having updated our new board we need to copy it into the 'original'
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      *(board[i][j]) = *(newBoard[i][j]);
    }
  }
}

// -- Returns the time -- //
int Board::get_time() { return time; }

// -- Displays the board -- //
void Board::display()
{
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      if (board[i][j]->getState())
      {
        if (cellType == 1)
        {
          cout << "O";
        }
        else if (cellType == 2)
        {
          cout << "X";
        }
      }
      else
        cout << " ";
    }
    cout << endl;
  }
}
//  --  End of Board Class creation  --  //

int main()
{
  int selection = 0;   // stores the selection of 1: Cell or 2: Cancer
  int confluence = 0;  // stores the percentage of the board filled with cells
  int c;               // stores a character input
  Board board(20, 75); // create a 20 row and 75 column board
  string trash;        // collects useless string data
  system("cls");       // clear the screen to display the welcome screen
  cout << "Welcome to the cell simulator" << endl;
  cout << endl;
  cout << "Select your cell type: (1) normal cells or (2) cancer cells" << endl;
  while ((selection < 1) || (selection > 2))
  {
    cout << ">";
    cin >> selection;
  }
  getline(cin, trash); // a crude way of trashing anything beyond the first input
  cout << "Select the confluence percentage (%)" << endl;
  while ((confluence <= 0) || (confluence > 100))
  {
    cout << ">";
    cin >> confluence;
  }
  getline(cin, trash);
  // seed the board with Cell or Cancer
  board.seed_cells(selection, confluence);
  system("cls");   // clear the screen
  while (c != 'q') // keep running the simulator until 'q' is entered
  {
    system("cls"); // clear the screen (note: omit this when debugging)
    cout << "time: " << board.get_time() << endl;
    cout << "number of cells: " << board.get_num_cells() << endl;
    board.display(); // display the board
    c = cin.get();
    board.next_state(); // get the next state (note: the complex algorithm is in here)
  }
  return 0;
}
