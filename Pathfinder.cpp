#include "Pathfinder.h"

//Part 1-----------------------------------------------------------------------------------
/*
* toString
*
* Returns a string representation of the current maze. Returns a maze of all 1s if no maze
* has yet been generated or imported.
*
* A valid string representation of a maze consists only of 125 1s and 0s (each separated
* by spaces) arranged in five 5x5 grids (each separated by newlines) with no trailing newline. A valid maze must
* also have 1s in the entrance cell (0, 0, 0) and in the exit cell (4, 4, 4).
*
* Cell (0, 0, 0) is represented by the first number in the string representation of the
* maze. Increasing in x means moving toward the east, meaning cell (1, 0, 0) is the second
* number. Increasing in y means moving toward the south, meaning cell (0, 1, 0) is the
* sixth number. Increasing in z means moving downward to a new grid, meaning cell
* (0, 0, 1) is the twenty-sixth cell in the maze. Cell (4, 4, 4) is represented by the last number.
*
* Returns:		string
*				A single string representing the current maze
*/
string Pathfinder::toString() const {

	stringstream ss;
	for (int x = 0; x < X_SIZE; x++) {
		for(int y = 0; y < Y_SIZE; y++) {
			for (int z = 0; z < Z_SIZE; z++) {
				if (mazeGen){
					ss << maze[x][y][z];
				}
				// if maze has not been imported or generated
				else {
					ss << "1";
				}
				// no space at end of line
				if (z < Z_SIZE - 1) {
					ss << " ";
				}
			}
			ss << endl;
		}
		// no newline at end of maze
		if (x < X_SIZE - 1) {
			ss << endl;
		}

	}
	return ss.str();
}

/*
* createRandomMaze
*
* Generates a random maze and stores it as the current maze.
*
* The generated maze must contain a roughly equal number of 1s and 0s and must have a 1
* in the entrance cell (0, 0, 0) and in the exit cell (4, 4, 4).  The generated maze may be
* solvable or unsolvable, and this method should be able to produce both kinds of mazes.
*/
void Pathfinder::createRandomMaze() {
	for (int x = 0; x < X_SIZE; x++) {
		for(int y = 0; y < Y_SIZE; y++) {
			for (int z = 0; z < Z_SIZE; z++) {
				maze[x][y][z] = rand() % 2;
			}
		}
	}
	// ensure entry and exit available
	maze[0][0][0] = 1;
	maze[X_SIZE-1][Y_SIZE-1][Z_SIZE-1] = 1;
	mazeGen = true;
}
//-----------------------------------------------------------------------------------------

//Part 2-----------------------------------------------------------------------------------
/*
* importMaze
*
* Reads in a maze from a file with the given file name and stores it as the current maze.
* Does nothing if the file does not exist or if the file's data does not represent a valid
* maze.
*
* The file's contents must be of the format described above to be considered valid.
*
* Parameter:	file_name
*				The name of the file containing a maze
* Returns:		bool
*				True if the maze is imported correctly; false otherwise
*/
bool Pathfinder::importMaze(string file_name) {
	int tempMaze[X_SIZE][Y_SIZE][Z_SIZE];
	ifstream file(file_name);
	cout << endl << file_name << endl;
	if(file.is_open()) {
		string line;
		for (int x = 0; x < X_SIZE; x++) {
			for(int y = 0; y < Y_SIZE; y++) {
				// not enough inputs
				if (!getline(file, line)){
					return false;
				}
				stringstream ss(line);
				for (int z = 0; z < Z_SIZE; z++) {
					int value;
					ss >> value;
					// non-integer value
					if (ss.fail()) {
						return false;
					} else if (value == 1 || value == 0) {
						tempMaze[x][y][z] = value;
					} else {
						// values other than 1 and 0
						return false;
					}
				}
			}
			getline(file, line);
		}
		//too many inputs
		if (getline(file, line)) { 
			return false;
		}
		//missing entry or exit
		if (tempMaze[0][0][0] != 1 || tempMaze[X_SIZE-1][Y_SIZE-1][Z_SIZE-1] != 1){
			return false;
		}
		//only overwrite existing maze if successfully imported
		mazeCopy(tempMaze, maze);
		mazeGen = true;
		return true;
	} else {
		return false;
	}
}

void Pathfinder::mazeCopy(const int maze1[][Y_SIZE][Z_SIZE], int maze2[][Y_SIZE][Z_SIZE]) {
	for (int x = 0; x < X_SIZE; x++) {
		for(int y = 0; y < Y_SIZE; y++) {
			for (int z = 0; z < Z_SIZE; z++) {
				maze2[x][y][z] = maze1[x][y][z];
			}
		}
	}
}
//-----------------------------------------------------------------------------------------

//Part 3-----------------------------------------------------------------------------------
/*
* solveMaze
*
* Attempts to solve the current maze and returns a solution if one is found.
*
* A solution to a maze is a list of coordinates for the path from the entrance to the exit
* (or an empty vector if no solution is found). This list cannot contain duplicates, and
* any two consecutive coordinates in the list can only differ by 1 for only one
* coordinate. The entrance cell (0, 0, 0) and the exit cell (4, 4, 4) should be included
* in the solution. Each string in the solution vector must be of the format "(x, y, z)",
* where x, y, and z are the integer coordinates of a cell.
*
* Understand that most mazes will contain multiple solutions
*
* Returns:		vector<string>
*				A solution to the current maze, or an empty vector if none exists
*/
vector<string> Pathfinder::solveMaze() {
	path.clear();
	int tempMaze[X_SIZE][Y_SIZE][Z_SIZE];
	mazeCopy(maze, tempMaze);
	stack<string> tempPath;
	if(findPath(tempMaze, 0, 0, 0, tempPath)) {
		while (!tempPath.empty()) {
			path.push_back(tempPath.top());
			tempPath.pop();
		}
	}
	return path;
}
//-----------------------------------------------------------------------------------------
bool Pathfinder::findPath(int tMaze[][Y_SIZE][Z_SIZE], int x, int y, int z, stack<string>& tPath) {
	//cell out of bounds or not usable
	if(x < 0 || y < 0 || z < 0 || x >= X_SIZE || y >= Y_SIZE || z >= Z_SIZE
		|| tMaze[x][y][z] != OPEN) {
		return false;
	}
	// cell at exit
	else if (x == X_SIZE-1 && y == Y_SIZE-1 && z == Z_SIZE-1) {
		tMaze[x][y][z] = PATH;
		tPath.push("(" + to_string(z) + ", " + to_string(y) + ", " + to_string(x) + ")");
		return true;
	}
	// cell is possible
	else {
		tMaze[x][y][z] = PATH;
		if (findPath(tMaze, x - 1, y, z, tPath)
	        || findPath(tMaze, x + 1, y, z, tPath)
	        || findPath(tMaze, x, y - 1, z, tPath)
	        || findPath(tMaze, x, y + 1, z, tPath)
			|| findPath(tMaze, x, y, z - 1, tPath)
			|| findPath(tMaze, x, y, z + 1, tPath) ) {
	      	tPath.push("(" + to_string(z) + ", " + to_string(y) + ", " + to_string(x) + ")");
	      	return true;
	    }
		// cell wasn't on path
	    else {
	      	tMaze[x][y][z] = USED;  // Dead end.
	      	return false;
	    }
	}
}
