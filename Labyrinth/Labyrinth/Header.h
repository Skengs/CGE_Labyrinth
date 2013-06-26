#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <conio.h> /* for _getch() (any keypress) */
#include "tga.h"

/* some math.h files don't define pi... */
#ifndef M_PI
#define M_PI 3.141592653
#endif

#define RAD(x) (((x)*M_PI)/180.)

#ifdef __STRICT_ANSI__
#define sinf(x) ((float)sin((x)))
#define cosf(x) ((float)cos((x)))
#define atan2f(x, y) ((float)atan2((x), (y)))
#endif 


// Sets max width and height of a labyrinth
#define MazeLength 100

// Multiplier, defines scale of each block
#define MazeScale 3.0

// Frames per second
#define FPS 60

// time it takes to move one block (in ms)
#define MoveDuration 1000

// time it takes to rotate 90° (in ms)
#define RotateDuration 500

// Shapetypes
#define SquareShape		1
#define DeadEndShape	2
#define CornerShape		3
#define CorridorShape	4
#define BorderShape		5
#define EmptyShape		6


struct MazeElement
{
	unsigned short type;
	GLfloat rotation;
};

struct ArrayPosition {
	int width;
	int height;
	int lookDirection;		// 0=north | 1=east | 2=south | 3=west
};

struct Movement 
{
	char moveDirection;		// 'n' | 'e' | 's' | 'w'
	char rotateDirection;	// 'l' | 'r'
	int counter;			// Number of frames in which movement is done
};


// Functions
FILE* CheckExistence(char*);
bool ValidateContent(FILE*, int*, int*);
struct MazeElement** ConvertToShapes (FILE*, int, int, struct ArrayPosition*, struct ArrayPosition*);
void printInstructions();

// openGL
void resize(int width, int height);
void keyPressed(unsigned char key, int x, int y);
void display();
void init(int width, int height);
void timer(int value);
void drawBlock(int shape);
void freeMemory();
void tryMove (char direction);

void initLists();