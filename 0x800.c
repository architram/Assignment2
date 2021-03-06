/*
 ============================================================================
 Name        : 0x800.c
 Author      : Architram Senathi; Hayden Smith; Maurits van der Schee
 Description : Hex based console version of the game "2048" for GNU/Linux
 ============================================================================
 */

// This is your standard includes for external modules
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// This includes a special file used for this assignment 
//  that was made for COMP1911
#include "0x800.h"

int main(int argc, char *argv[]) {
    char c;
    int success;
    int seed = getSeed(argc, argv);
    int finishedGame = FALSE;

    Board board = newBoard();
    
    // Do not move, replace, rename, or delete this function.
    // It carries out some necessary magic - be nice to it!
    //setAntiCrashMode(1);
    ignoreThisFunction1(board, seed, SIZE, DEBUG, TEXT_ONLY);

    // The function to populate the board from the command line
    //  argument is called. If there is no argument in command line
    //  then two random 
    int boardWasPopulated = populateBoard(board, argc, argv);
    if (!boardWasPopulated) {
        addRandom(board);
        addRandom(board);
    }

    drawBoard(board);
    ignoreThisFunction2(board);
    
    // Continually executing the game loop until the game is finished
    while (!finishedGame) {
        c = getcharX();
        success = FALSE;

        // Check what kind of key press was made using getchar
        if (c == KEYBOARD_LEFT) {
            success = moveLeft(board);
        } else if (c == KEYBOARD_RIGHT) {
            success = moveRight(board); 
        } else if (c == KEYBOARD_UP) {
            success = moveUp(board);
        } else if (c == KEYBOARD_DOWN) {
            success = moveDown(board);
        } else if (c == 'r'){
        	printfGame("RESTART? (y/n)");
        	c = getcharX();
        	if (c == 'y'){
				freeBoard(board);
				board = newBoard();
				//drawBoard(board);
				ignoreThisFunction1(board, seed, SIZE, DEBUG, TEXT_ONLY);
				boardWasPopulated = populateBoard(board, argc, argv);
				if (!boardWasPopulated) {
					addRandom(board);
					addRandom(board);
				}
				drawBoard(board);
    			//ignoreThisFunction2(board);	
        	} else {
        		drawBoard(board);
        	}
        } else if (c == 'q') {
            printfGame(">> QUIT? (y/n)");
            c = getcharX();
            if (c == 'y') {
                finishedGame = TRUE;
            } else {
                drawBoard(board);
            }
        }

        // If a move key was pressed and the board was actually moved,
        //  then draw the board again, add a new random ell, draw the
        //  board again, and check if the game is finished
        if (success) {
            drawBoard(board);
            addRandom(board);
            drawBoard(board);
            if (gameEnded(board)) {
                printfGame(">> GAME OVER");
                finishedGame = TRUE;
            }
        }

        // Do not move, replace, rename, or delete this function.
        // It carries out some necessary magic - be nice to it!
        ignoreThisFunction2(board);
    }

    // Because the game is finished, we need to call a function
    //  that frees the board pointer, then set that board pointer to NULL
    //  to ensure that no one can accidentally use free'd memory.
    freeBoard(board);
    board = NULL;

    // Do not move, replace, rename, or delete this function.
    // It carries out some necessary magic - be nice to it!
    ignoreThisFunction3(board);

    return 0;
}

/////////////////////////////////////////////////////////////////////
//////////////////////////// Part 1 /////////////////////////////////
/////////////////////////////////////////////////////////////////////

// This function malloc for board row by row and then for the board
// The function also initialises all cells to NULL
Board newBoard() {
	int row = 0;
	int col = 0;
	
	//malloc for the board	
	Board board = (Board)malloc(sizeof(char**) * SIZE);
	
	//mallocing row by row and initialising each cell to NULL
	while (row < SIZE){
		board[row] = (cell*)malloc(sizeof(char*) * SIZE);
		col = 0;
		while (col < SIZE){
			board[row][col] = NULL;
			col = col + 1;
		}
		row = row + 1;
	}
		
    return board; 
}

// This function frees the malloc  row by row and then the whole board.
void freeBoard(Board board) {
	int row = 0;
	int col = 0;
	while (row < SIZE){
		while (col < SIZE){
			free(board[row][col]);
			board[row][col] = NULL;
			col = col + 1;
		}
		free(board[row]);
		row = row + 1;
	}
	free(board);
}	

//THis function converts the integer input to hexadecimal string and 
//returns the array of string.
char *convertToHex (int integer){
	
	char *str;
	//int length;
	//int i = integer;
	//We have to check the length of the integer to malloc exactly
	//the appropriate space.
	/*while(i > 0){
		i = i/10;
		length = length + 1; 
	}*/
	//length = length + 3; //3 accounts for '0''x' and '\0'
	str = malloc(sizeof(char) * 6);
	
	sprintf(str, "0x%x",integer);
	
	return str;
}

// The second argument passed into the program is the "input string". For
//  more information on this please see the assignment specification.
int populateBoard(Board board, int argc, char* argv[]) {
	
	int i = 0;
	int j = 0;
	int k = 0;
	int n = 0;
	int g = 0;
	int integerArray[SIZE*SIZE];
	
	//checkes if there is only one line of argument in command line except
	//for default one.
	if (argc < 2){
		return FALSE;
	}
	
	//Checks if the length of the argument is of valid size.
	//if its not the size we want, it prints something and exits 
	//the program.
	if (strlen(argv[1]) != SIZE*SIZE){
		printf("Please enter a valid populate string\n");
		exit(1);
	}
	
	//only proceeds with populating the code if the argument meets all the 
	//conditions.
	while (i < SIZE*SIZE){
		// Checking if the input contains A or B and converting them 
		//to integers
		// Is ther any better way to do this? I tried atoi but 
		//it returned 0.
		if (argv[1][i] == 'A'){
			integerArray[i] = 10;
		} else if (argv[1][i] == 'B'){
			integerArray[i] = 11;
		}
		//Checking if the inout is in our desired range
		else if (argv[1][i] >= '0' && argv[1][i] <= '9'){
			integerArray[i] = argv[1][i] - '0';	
		} else {
			printf("Please enter a valid populate string\n");
			exit(1);			
		}
		i = i + 1;
	}
	
	//pretty full on nested while loop which basically populates the board
	i = 0;
	while (i < SIZE){
		j = 0;
		while (j < SIZE){
			//Frees every cell of the board which is not a NULL
			if (board[i][j] != NULL){
				free(board[i][j]);
				board[i][j] = NULL;
			}
			//If n is 0, squaring that will give 1. So have to 
			//populate NULL to that cell of the board.
			// else, we square 2 with the number in integerArray
			//and convert that to hexadecimal which is then populated
			//in the respective cell of the board.
			n = integerArray[k];
			if (n == 0){
				board[i][j] = NULL;
				k = k + 1;
			} else {
				g = (int) pow(2,n);
				board[i][j] = convertToHex(g);
				k = k + 1;
			}
			j = j + 1;
		}
		i = i + 1;
	}
	
 
    return TRUE;
}

// THIS FUNCTION IS ALREADY COMPLETE, IT IS HERE PURELY FOR DEMONSTRATION
// The second argument passed into the program is the "seed" for the
//  random number generator (that you don't have to worry about)
int getSeed(int argc, char* argv[]) {
    int ret = 0;
    if (argc == 3) {
        ret = atoi(argv[2]);
    }
    return ret;
}

/////////////////////////////////////////////////////////////////////
//////////////////////////// Part 2 /////////////////////////////////
/////////////////////////////////////////////////////////////////////


// The function returns TRUE if ANY cells were moved, and FALSE if no
//  cells were moved
int moveUpSlide (Board board){
	int row = 1;
	int col = 0;
	int repeat = 0;
	int count = 0;

//This nested while loop basically checks every cell which has a value,
//if there is empty cell on top of that cell. If that is true, the bottom
//cell replaces the top cell and the bottom cell is made empty. This 
//process then repeats 4 times to check for all cells in board.

	while (repeat < SIZE){
		row = 1;
		while (row < SIZE){
			col = 0;
			while (col < SIZE){
				if (board[row][col] == NULL){
					col = col + 1;
				} else if (board[row][col] != NULL && 
						   board[row-1][col] == NULL){
					board[row-1][col] = board[row][col];
					board[row][col] = NULL;
					col = col + 1;
					count = 1;
				} else {
					col = col + 1;
				}
			}
			row = row + 1;
		}
		repeat = repeat + 1;
	}
	return count;
}

// The function returns TRUE if ANY cells were moved, and FALSE if no
//  cells were moved
int moveUpMerge (Board board){		
	
	int row = 1;
	int col = 0;
	int mergedInt;
	int comp1;
	int comp2;
	int count = 0;
	
//This nested While loop basically checks every cell and checks if the
//cell above them is of the same value. If yes, both cell add up and
//top cell is replaces with the add up value. when doing so, the bottom
//cell is made empty.

	while (row < SIZE){
		col = 0;
		while (col < SIZE){
			if (board[row][col] != NULL && board[row-1][col] != NULL){
				comp1 = strtol(board[row][col] + 2, NULL, 16);
				comp2 = strtol(board[row-1][col] + 2, NULL, 16);
				if (comp1 == comp2){
					count = 1;
					mergedInt = comp1 * 2;
					sprintf(board[row-1][col], "0x%x",mergedInt);
					free(board[row][col]);
					board[row][col] = NULL;
				}
			}
			col = col + 1;
		}
		row = row + 1;
	}
	
	return count;
}

// Function moves all of the cells to the very top
//  of the board. During the process any two cells that are
//  adjacent in the vertical direction should be merged.
int moveUp(Board board) {
	
	int count,a1,a2,a3;
	
	a1 = moveUpSlide(board);
	a2 = moveUpMerge(board);
	a3 = moveUpSlide(board);
	
	count = a1+a2+a3;
	
	if (count != 0){
		return TRUE;
	} else {
    	return FALSE;
    }
}

// The function returns TRUE if ANY cells were moved, and FALSE if no
//  cells were moved
int moveDownSlide (Board board){
	int row = SIZE - 2;
	int col = SIZE - 1;
	int repeat = 0;
	int count = 0;
	
//This nested while loop basically checks every cell which has a value,
//if there is empty cell on bottom of that cell. If that is true, the top
//cell replaces the bottom cell and the top cell is made empty. This 
//process then repeats 4 times to check for all cells in board.

	while (repeat < SIZE){
		row = SIZE - 2;
		while (row >= 0){
			col = SIZE - 1;
			while (col >= 0){
				if (board[row][col] == NULL){
					col = col - 1;
				} else if (board[row][col] != NULL && 
						   board[row+1][col] == NULL){
					board[row+1][col] = board[row][col];
					board[row][col] = NULL;
					col = col - 1;
					count = 1;
				} else {
					col = col - 1;
				}
			}
			row = row - 1;
		}
		repeat = repeat + 1;
	}
	return count;
}

// The function returns TRUE if ANY cells were moved, and FALSE if no
//  cells were moved
int moveDownMerge (Board board){		
	
	int row = SIZE - 2;
	int col = SIZE - 1;
	int mergedInt;
	int comp1;
	int comp2;
	int count = 0;
	
//This nested While loop basically checks every cell and checks if the
//cell below them is of the same value. If yes, both cell add up and
//bottom cell is replaced with the add up value. When doing so, the top
//cell is made empty.

	while (row >= 0){
		col = SIZE - 1;
		while (col >= 0){
			if (board[row][col] != NULL && board[row+1][col] != NULL){
				comp1 = strtol(board[row][col] + 2, NULL, 16);
				comp2 = strtol(board[row+1][col] + 2, NULL , 16);
				if (comp1 == comp2){
					count = 1;
					mergedInt = comp1 * 2;
					sprintf(board[row+1][col],"0x%x",mergedInt);
					free(board[row][col]);
					board[row][col] = NULL;
				}
			}
			col = col - 1;
		}
		row = row - 1;
	}
	
	return count;
}

// Function moves all of the cells to the very bottom
//  of the board. During the process any two cells that are
//  adjacent in the vertical direction should be merged.
int moveDown(Board board) {
	
	int count,a1,a2,a3;
	
	a1 = moveDownSlide(board);
	a2 = moveDownMerge(board);
	a3 = moveDownSlide(board);
	
	count = a1+a2+a3;
	
	if (count != 0){
		return TRUE;
	} else {
    	return FALSE;
    }
}

// The function returns TRUE if ANY cells were moved, and FALSE if no
//  cells were moved
int moveLeftSlide (Board board){
	int row = 0;
	int col = 1;
	int repeat = 0;
	int count = 0;
	
//This nested while loop basically checks every cell which has a value,
//if there is empty cell on left of that cell. If that is true, the right
//cell replaces the left cell and the right cell is made empty. This 
//process then repeats 4 times to check for all cells in board.

	while (repeat < SIZE){
		row = 0;
		while (row < SIZE){
			col = 1;
			while (col < SIZE){
				if (board[row][col] == NULL){
					col = col + 1;
				} else if (board[row][col] != NULL && 
						   board[row][col-1] == NULL){
					board[row][col-1] = board[row][col];
					board[row][col] = NULL;
					col = col + 1;
					count = 1;
				} else {
					col = col + 1;
				}
			}
			row = row + 1;
		}
		repeat = repeat + 1;
	}
	return count;
}

// The function returns TRUE if ANY cells were moved, and FALSE if no
//  cells were moved
int moveLeftMerge (Board board){		
	
	int row = 0;
	int col = 1;
	int mergedInt;
	int comp1;
	int comp2;
	int count = 0;

//This nested While loop basically checks every cell and checks if the
//cell left of them is of the same value. If yes, both cell add up and
//left cell is replaced with the add up value. When doing so, the right
//cell is made empty.

	while (row < SIZE){
		col = 1;
		while (col < SIZE){
			if (board[row][col] != NULL && 
				board[row][col-1] != NULL){
				comp1 = strtol(board[row][col] + 2, NULL, 16);
				comp2 = strtol(board[row][col-1] + 2, NULL, 16);
				if (comp1 == comp2){
					count = 1;
					mergedInt = comp1 * 2;
					sprintf(board[row][col-1],"0x%x",mergedInt);
					free(board[row][col]);
					board[row][col] = NULL;
				}
			}
			col = col + 1;
		}
		row = row + 1;
	}
	
	return count;
}

// Function moves all of the cells to the very left hand
//  side of the board. During the process any two cells that are
//  adjacent in the horizontal direction should be merged.
int moveLeft(Board board) {
	
	int count,a1,a2,a3;
	
	a1 = moveLeftSlide(board);
	a2 = moveLeftMerge(board);
	a3 = moveLeftSlide(board);
	
	count = a1+a2+a3;
	
	if (count != 0){
		return TRUE;
	} else {
    	return FALSE;
    }
}

// The function returns TRUE if ANY cells were moved, and FALSE if no
//  cells were moved
int moveRightSlide (Board board){
	int col = SIZE - 2;
	int row = 0;
	int repeat = 0;
	int count = 0;

//This nested while loop basically checks every cell which has a value,
//if there is empty cell on right of that cell. If that is true, the left
//cell replaces the right cell and the left cell is made empty. This 
//process then repeats 4 times to check for all cells in board.

	while (repeat < SIZE){
		row = 0;
		while (row < SIZE){
			col = SIZE - 2;
			while (col >= 0){
				if (board[row][col] == NULL){
					col = col - 1;
				} else if (board[row][col] != NULL && 
						   board[row][col+1] == NULL){
					board[row][col+1] = board[row][col];
					board[row][col] = NULL;
					col = col - 1;
					count = 1;
				} else {
					col = col - 1;
				}
			}
			row = row + 1;
		}
		repeat = repeat + 1;
	}
	return count;
}

// The function returns TRUE if ANY cells were moved, and FALSE if no
//  cells were moved
int moveRightMerge (Board board){		
	
	int col = SIZE - 2;
	int row = 0;
	int mergedInt;
	int comp1;
	int comp2;
	int count = 0;

//This nested While loop basically checks every cell and checks if the
//cell right of them is of the same value. If yes, both cell add up and
//right cell is replaced with the add up value. When doing so, the left
//cell is made empty.

	while (row < SIZE){
		col = SIZE - 2;
		while (col >= 0){
			if (board[row][col] != NULL && board[row][col+1] != NULL){
				comp1 = strtol(board[row][col] + 2, NULL, 16);
				comp2 = strtol(board[row][col+1] + 2, NULL, 16);
				if (comp1 == comp2){
					count = 1;
					mergedInt = comp1 * 2;
					sprintf(board[row][col+1],"0x%x",mergedInt);
					free(board[row][col]);
					board[row][col] = NULL;
				}
			}
			col = col - 1;
		}
		row = row + 1;
	}
	
	return count;
}

// Function moves all of the cells to the very right hand
//  side of the board. During the process any two cells that are
//  adjacent in the horizontal direction should be merged.
int moveRight(Board board) {
	
	int count,a1,a2,a3;
	
	a1 = moveRightSlide(board);
	a2 = moveRightMerge(board);
	a3 = moveRightSlide(board);
	
	count = a1+a2+a3;
	
	if (count != 0){
		return TRUE;
	} else {
    	return FALSE;
    }
}

/////////////////////////////////////////////////////////////////////
//////////////////////////// Part 3 /////////////////////////////////
/////////////////////////////////////////////////////////////////////

// You probably don't need to write any code down here, you can just
//  write it into the main function.

/////////////////////////////////////////////////////////////////////
//////////////////////////// Part 4 /////////////////////////////////
/////////////////////////////////////////////////////////////////////

// You will need to write code down here AND inside the main function.
// We have not given you any starter code for this section. You will
//  have to start your own
