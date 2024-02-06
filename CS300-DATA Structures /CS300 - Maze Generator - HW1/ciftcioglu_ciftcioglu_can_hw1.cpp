
#include <iostream>
#include <string>
#include "Stack.h"
#include <fstream>
#include <vector>
#include <random>
using namespace std;


//I USED LECTURE SLIDES TO WRITE THIS STACK RELATED PART

//Constructs the stack
template <class Object>
Stack<Object>::Stack( )
{
    topOfStack = NULL;}

//Test if the stack is logically full.Return false always, in this implementation.
template <class Object>
bool Stack<Object>::isFull( ) const{
    return false;}

//Test if the stack is logically empty. Return true if empty, false, otherwise.
template <class Object>
bool Stack<Object>::isEmpty( ) const
{
    return topOfStack == NULL;}

//Get the most recently inserted item in the stack.
//Return the most recently inserted item in the stack or throw an exception if empty.
template <class Object>
const Object & Stack<Object>::top( ) const{
    if ( isEmpty( ) )
        throw ("Empty stack!");
        return topOfStack->element;}

//Get the most recently inserted item in the stack.
//Return the most recently inserted item in the stack
template <class Object>
void Stack<Object>::pop(){
    if (isEmpty())
        throw ("Empty stack!");/*or throw an exception if empty.*/

    ListNode *oldTop = topOfStack;
    topOfStack = topOfStack->next;
    delete oldTop;}

//Insert x into the stack.
template <class Object>
void Stack<Object>::push( const Object & x ){
    topOfStack = new ListNode( x, topOfStack );}

//Return and remove the most recently, inserted item from the stack.
template <class Object>
Object Stack<Object>::topAndPop( )
{
    Object topItem = top( );
    pop( );
    return topItem;}

//Make the stack logically empty.
template <class Object>
void Stack<Object>::makeEmpty( )
{
    while ( ! isEmpty( ) )
        pop( );}

//Deep copy.
template <class Object>
const Stack<Object> & Stack<Object>::operator=( const Stack<Object> & rhs ){
    if ( this != &rhs ) {
        makeEmpty( );
        if ( rhs.isEmpty( ) )
            return *this;
        ListNode *rptr = rhs.topOfStack;
        ListNode *ptr  = new ListNode( rptr->element );
        topOfStack = ptr;
        for ( rptr = rptr->next; rptr != NULL; rptr = rptr->next )
            ptr = ptr->next = new ListNode( rptr->element );
        }
        return *this;}

//Copy constructor.
template <class Object>
Stack<Object>::Stack( const Stack<Object> & rhs ){
    topOfStack = NULL;
    *this = rhs; /* deep copy */}

//Destructor.
template <class Object>
Stack<Object>::~Stack( )
{
    makeEmpty( );}

// important part starts from here:)
//creating struct for the maze process
struct mazecell{
	int x_coordinate;
	int y_coordinate;
	bool up_wall;
	bool down_wall;
	bool left_wall;
	bool right_wall;
	bool isVisited;
	mazecell (int x = 0, int y=0){
		this->x_coordinate = x;
		this->y_coordinate = y;
	};
};

//controls the neighbor cells, if all the neighbors are visited -> it will return true, if one or none of the neighbors are visited -> it will return false.
bool isNeighborVisited(const std::vector<std::vector<mazecell> >& maze, int y, int x, int row, int column) {
    // Check for corners
    if (y == 0 && x == 0) { // Lower left corner
        return maze[y + 1][x].isVisited && maze[y][x + 1].isVisited;
    } else if (y == row - 1 && x == 0) { // Upper left corner
        return maze[y - 1][x].isVisited && maze[y][x + 1].isVisited;
    } else if (y == 0 && x == column - 1) { // Lower right corner
        return maze[y][x - 1].isVisited && maze[y + 1][x].isVisited;
    } else if (y == row - 1 && x == column - 1) { // Upper right corner
        return maze[y][x - 1].isVisited && maze[y - 1][x].isVisited;
    }

    // Check for edges
    bool leftVisited = (x > 0) ? maze[y][x - 1].isVisited : true;
    bool rightVisited = (x < column - 1) ? maze[y][x + 1].isVisited : true;
    bool upVisited = (y > 0) ? maze[y - 1][x].isVisited : true;
    bool downVisited = (y < row - 1) ? maze[y + 1][x].isVisited : true;

    if (y == 0) { // Lower edge
        return leftVisited && rightVisited && downVisited;
    } else if (y == row - 1) { // Upper edge
        return leftVisited && rightVisited && upVisited;
    } else if (x == 0) { // Left edge
        return upVisited && downVisited && rightVisited;
    } else if (x == column - 1) { // Right edge
        return upVisited && downVisited && leftVisited;
    }

    // For non-edge, non-corner cells
    return leftVisited && rightVisited && upVisited && downVisited;
}
//This function controls paths, if one neighbor cell is a visited and the wall which connects them are empty it will
//	return true.
bool pathCheck(vector<vector<mazecell> > maze, int y, int x) {
    bool up = (y < maze.size() - 1) && (maze[y][x].up_wall == 0) && (maze[y + 1][x].isVisited == 0);
    bool down = (y > 0) && (maze[y][x].down_wall == 0) && (maze[y - 1][x].isVisited == 0);
    bool left = (x > 0) && (maze[y][x].left_wall == 0) && (maze[y][x - 1].isVisited == 0);
    bool right = (x < maze[0].size() - 1) && (maze[y][x].right_wall == 0) && (maze[y][x + 1].isVisited == 0);

    return up || down || left || right;
}

vector<vector<mazecell> > mazeCreator(int row, int column){
	mazecell dummyCell; 
	vector<vector<mazecell> > maze(row,vector<mazecell>(column,dummyCell)); /*cell Matrix to construct maze.*/
	for (int k=0; k<row; k++){ /*assigns values to cells. by default all walls are full and all cells are visited.*/
		for (int j=0; j<column;j++){
			maze[k][j].x_coordinate = j;maze[k][j].y_coordinate = k;
			maze[k][j].up_wall = 1;	maze[k][j].down_wall = 1;
			maze[k][j].left_wall = 1; maze[k][j].right_wall = 1;
			maze[k][j].isVisited = 0;
		}
	}
	int currX = 0, currY = 0, visitCounter = 0, cellCounter = row * column, direction;
Stack<mazecell> visitedCells;
visitedCells.push(maze[currY][currX]);
visitCounter++;
maze[currY][currX].isVisited = 1;

while (visitCounter < cellCounter) {
    currX = visitedCells.top().x_coordinate;
    currY = visitedCells.top().y_coordinate;
    direction = rand() % 4;

    switch (direction) {
        case 0:
            if (currY < row - 1 && maze[currY + 1][currX].isVisited == 0) {
                maze[currY + 1][currX].isVisited = 1;
                maze[currY][currX].up_wall = 0;
                maze[currY + 1][currX].down_wall = 0;
                visitedCells.push(maze[currY + 1][currX]);
                currY += 1;
                visitCounter += 1;
            }
            break;

        case 1:
            if (currX > 0 && maze[currY][currX - 1].isVisited == 0) {
                maze[currY][currX - 1].isVisited = 1;
                maze[currY][currX].left_wall = 0;
                maze[currY][currX - 1].right_wall = 0;
                visitedCells.push(maze[currY][currX - 1]);
                currX -= 1;
                visitCounter += 1;
            }
            break;

        case 2:
            if (currX < column - 1 && maze[currY][currX + 1].isVisited == 0) {
                maze[currY][currX + 1].isVisited = 1;
                maze[currY][currX + 1].left_wall = 0;
                maze[currY][currX].right_wall = 0;
                visitedCells.push(maze[currY][currX + 1]);
                currX += 1;
                visitCounter += 1;
            }
            break;

        case 3:
            if (currY > 0 && maze[currY - 1][currX].isVisited == 0) {
                maze[currY - 1][currX].isVisited = 1;
                maze[currY - 1][currX].up_wall = 0;
                maze[currY][currX].down_wall = 0;
                visitedCells.push(maze[currY - 1][currX]);
                currY -= 1;
                visitCounter += 1;
            }
            break;
    }

    if (isNeighborVisited(maze, currY, currX, row, column) == true) {
        visitedCells.pop();
    }
}

visitedCells.makeEmpty();
return maze;

}



void pathfinder(vector<vector<mazecell> > maze, int id, int entryX, int entryY, int exitX, int exitY) {
    int row = maze.size();
    int column = maze[0].size();

    // Reset isVisited for all cells
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            maze[i][j].isVisited = false;
        }
    }

    Stack<mazecell> pathStack;
    maze[entryY][entryX].isVisited = true;
    pathStack.push(maze[entryY][entryX]);

    while (!pathStack.isEmpty()) {
        mazecell currentCell = pathStack.top();
        int currX = currentCell.x_coordinate;
        int currY = currentCell.y_coordinate;

        if (currX == exitX && currY == exitY) {
            // Path found, output to file
            pathfinder(maze, id, entryX, entryY, exitX, exitY);
            break;
        }

        bool pathFound = false;
        for (int direction = 0; direction < 4 && !pathFound; direction++) {
            int newX = currX, newY = currY;
            switch (direction) {
                case 0: // Up
                    if (currY > 0 && !maze[currY - 1][currX].isVisited && !currentCell.up_wall) newY--;
                    break;
                case 1: // Left
                    if (currX > 0 && !maze[currY][currX - 1].isVisited && !currentCell.left_wall) newX--;
                    break;
                case 2: // Right
                    if (currX < column - 1 && !maze[currY][currX + 1].isVisited && !currentCell.right_wall) newX++;
                    break;
                case 3: // Down
                    if (currY < row - 1 && !maze[currY + 1][currX].isVisited && !currentCell.down_wall) newY++;
                    break;
            }

            if (newX != currX || newY != currY) {
                maze[newY][newX].isVisited = true;
                pathStack.push(maze[newY][newX]);
                pathFound = true;
            }
        }

        if (!pathFound) {
            pathStack.pop();
        }
    }
}
// check this againnnnn!!!! emin degilim bi daha bak
void outputMazePath(Stack<mazecell>& pathStack, int id, int entryX, int entryY, int exitX, int exitY) {
    string filename = "maze_" + to_string(id) + "_path_" + to_string(entryX) + "_"
        + to_string(entryY) + "_" + to_string(exitX) + "_" + to_string(exitY) + ".txt";
    ofstream output(filename.c_str());

    Stack<mazecell> printStack;
    while (!pathStack.isEmpty()) {
        printStack.push(pathStack.topAndPop());
    }

    while (!printStack.isEmpty()) {
        output << printStack.top().x_coordinate << " " << printStack.top().y_coordinate << endl;
        printStack.pop();
    }
}



void creatMazeOutput(vector<vector<mazecell> > maze, int id) {
    int row = maze.size();
    int column = maze[0].size();
    ofstream output;
    string filename = "maze_" + to_string(id) + ".txt";
    output.open(filename.c_str());
    output << row << " " << column << endl;
    
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            const mazecell &cell = maze[i][j];
            output << "x=" << cell.x_coordinate << " y=" << cell.y_coordinate << " l=" << cell.left_wall <<
                " r=" << cell.right_wall << " u=" << cell.up_wall << " d=" << cell.down_wall << endl;
        }
    }
    
    output.close();
}

int main(){
    vector<vector<vector<mazecell> > > mazeVector;
	int numberOfMazes, mazeID, rows, columns, entryX, entryY, exitX,exitY;
	cout << "Enter the number of mazes: "; /*Input from the user.*/
	cin >> numberOfMazes;
	cout << endl;
	cout << "Enter the number of rows and columns (M and N): ";
	cin >> rows >> columns;
	cout << endl;
	for(int z = 0; z < numberOfMazes; z++){ /*Create the mazes.*/
		mazeVector.push_back(mazeCreator(rows,columns));
        creatMazeOutput(mazeVector[z], z+1); /*Create the maze files. */
	}

	cout << "All mazes are generated." << endl << endl;
	cout << "Enter a maze ID between 1 to " << numberOfMazes << " inclusive to find a path: ";
	cin >> mazeID;
	cout << endl;
	cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): ";
	cin >> entryX >> entryY;
	cout << endl;
	cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): ";
	cin >> exitX >> exitY;
	
	pathfinder(mazeVector[mazeID-1], mazeID, entryX,entryY,exitX, exitY); /*create the pathfinder file. */
}
