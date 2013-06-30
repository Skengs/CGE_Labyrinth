#include <Header.h>

// Labyrinth data
extern struct MazeElement** Labyrinth;
extern int width, height;

extern struct ArrayPosition start, end;
struct ArrayPosition currentPosition;

bool gameFinished = false;

// GLUT window
extern int window;

// Movement
struct Movement movement;	// tells timer function what to do

const GLdouble moveFrames = ((GLdouble)MoveDuration / 1000.0 * (GLdouble)FPS);		// number of frames in which walking is done
const GLdouble rotateFrames = ((GLdouble)RotateDuration / 1000.0 * (GLdouble)FPS);	// number of frames in which rotating is done
const GLdouble moveValue = (2*(GLdouble)MazeScale) / moveFrames;					// value to move in each frame
const GLdouble rotateValue = 90.0 / rotateFrames;									// value to rotate in each frame

GLdouble angle_y = 0.0;		// angle of spin around y axis of scene, in degrees
GLdouble angle_x = 0.0;		// angle of spin around x axis  of scene, in degrees
GLdouble advance = 0.0;		// vertical movement ( z-achse )
GLdouble sideways = 0.0;	// horizontal movement ( x-achse )

// Display lists
GLuint DLists[6], texture[3];

// Clock
int timeValue;
int timeValueFinished;
char timeChar[1000];
bool clockStarted = false;


void resize(int width, int height)
{
	// prevent division by zero
	if (height == 0) { height=1; }

	glViewport(0, 0, width, height);  

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (float)width/(float)height, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
	glLoadIdentity();

	GLfloat baseColor[] = {0.5, 0.5, 0.5};

	// Camera lookdirection
	gluLookAt(-sinf(RAD(angle_y)),sinf(RAD(angle_x)), cosf(RAD(angle_y)),
			0, 0, 0,
			0, 1, 0);

	// Movement (Initial camera position already included)
	glTranslatef(-sideways,0,-advance);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_MODULATE);

	if (!gameFinished)	// Quality code right here
	{
		// draw Labyrinth
		glColor3fv(baseColor);

		glPushMatrix();
		for(int i = 0; i < height; i++)
		{
			glPushMatrix();
			for(int j = 0; j < width; j++)
			{
				if (Labyrinth[i][j].type != 0) // element is not a wall
				{
					if(Labyrinth[i][j].rotation != 0) // element needs to be rotated
					{
						glPushMatrix();
						glRotatef(Labyrinth[i][j].rotation, 0, 1, 0); // around y-achse
						drawBlock(Labyrinth[i][j].type);
						glPopMatrix();
					}
					else
					{
						drawBlock(Labyrinth[i][j].type);
					}

					if (i == start.height && j == start.width) // draw startpoint sphere
					{
						glPushMatrix();
						glTranslatef(0, -3*MazeScale/4, 0);
						glColor3f(0, 0, 1);
						glutSolidSphere(MazeScale/8, 50, 25);
						glPopMatrix();
						glColor3fv(baseColor);
					}
					if (i == end.height && j == end.width) // draw endpoint sphere
					{
						glPushMatrix();
						glTranslatef(0, -3*MazeScale/4, 0);
						glColor3f(1, 0, 0);
						glutSolidSphere(MazeScale/8, 50, 25);
						glPopMatrix();
						glColor3fv(baseColor);
					}

				}
				glTranslatef(2*MazeScale, 0, 0); // Move east (positive x-achse) after each block
			}
			glPopMatrix();
			glTranslatef(0, 0, 2*MazeScale); // Move south after each line (positive z-achse)
		}
		glPopMatrix();

		clock_format(timeValue);
		clock_display(timeChar);
	}
	else
	{
		sprintf_s(timeChar, "Congratulations! You finished the Labyrinth in %01d:%02d:%02d! Press [Esc] to exit.",
							timeValueFinished/3600, (timeValueFinished/60)%60, timeValueFinished%60);
		clock_display(timeChar);
	}

	glutSwapBuffers();
}

// Calls corresponding display list to given shapetype
void drawBlock(int shape)
{
	switch(shape)
	{
		case EmptyShape:	glCallList(DLists[5]);	break;

		case BorderShape:	glCallList(DLists[4]);	break;

		case CorridorShape:	glCallList(DLists[3]);	break;

		case CornerShape:	glCallList(DLists[2]);	break;

		case DeadEndShape:	glCallList(DLists[1]);	break;

		case SquareShape:	glCallList(DLists[0]);	break;

		default:			break;
	}
}


void init(int width, int height)  
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);        
	glDepthFunc(GL_LESS);      
	glEnable(GL_DEPTH_TEST);  
	glShadeModel(GL_SMOOTH);  
	resize(width, height);
	
	// load textures
	tgaInfo *info;
		
	info = tgaLoad(Texture_Floor);
	loadTextures(info, &texture[0]);

	info = tgaLoad(Texture_Wall);
	loadTextures(info, &texture[1]);

	info = tgaLoad(Texture_Ceiling);
	loadTextures(info, &texture[2]);

	// Load and precompile display lists for blocks
	initLists();

	// Set current position
	currentPosition = start;
	currentPosition.lookDirection = 0; // always north at start

	// Set initial camera position
	advance += 2*MazeScale*start.height;
	sideways += 2*MazeScale*start.width;

	// Initialize movement
	movement.counter = 0;
}


void keyPressed(unsigned char key, int x, int y) 
{
	switch (key) {

		case 27: // Escape
			freeMemory(); // clean up first
			exit(0);

		case 'w':	// move foreward
			if(movement.counter == 0) // if not moving already
			{
				movement.rotateDirection = '.'; // disable rotation
				switch (currentPosition.lookDirection)
				{
					case 0: // try move north
						tryMove('n');
						break;
					
					case 1: // try move east
						tryMove('e');
						break;

					case 2: // try move south
						tryMove('s');
						break;

					case 3: // try move west
						tryMove('w');
						break;

					default: break;
				}
				movement.rememberedKey = '.'; // delete last key
			}
			else 
				movement.rememberedKey = key; // remember key to process after moving
			break;

		case 's':	// move backward
			if(movement.counter == 0) // if not moving already
			{
				movement.rotateDirection = '.'; // disable rotation
				switch (currentPosition.lookDirection)
				{
					case 0: // look north, try go south
						tryMove('s');
						break;
					
					case 1: // look east, try go west
						tryMove('w');
						break;

					case 2: // look south, try go north
						tryMove('n');
						break;

					case 3: // look west, try go east
						tryMove('e');
						break;

					default: break;
				}
				movement.rememberedKey = '.'; // delete last key
			}
			else 
				movement.rememberedKey = key; // remember key to process after moving
			break;

		case 'a':	// rotate 90° left
			if(movement.counter == 0) // if not moving already
			{
				movement.moveDirection = '.'; // disable walking
				movement.rememberedKey = '.'; // delete last key

				movement.rotateDirection = 'l';
				movement.counter = rotateFrames;
				currentPosition.lookDirection = (currentPosition.lookDirection + 3) % 4; // update current lookdirection
			}
			else 
				movement.rememberedKey = key; // remember key to process after moving
			break;

		case 'd':	// rotate 90° right
			if(movement.counter == 0) // if not moving already
			{
				movement.moveDirection = '.'; // disable walking
				movement.rememberedKey = '.'; // delete last key

				movement.rotateDirection = 'r';
				movement.counter = rotateFrames;
				currentPosition.lookDirection = (currentPosition.lookDirection + 1) % 4; // update current lookdirection
			}
			else 
				movement.rememberedKey = key; // remember key to process after moving
			break;

		default: break;
	}
}

// try move ... "Collision Detection" haha
void tryMove (char direction)
{
	int heightAdd, widthAdd;
	heightAdd = widthAdd = 0;

	switch (direction)
	{
		case 'n':	heightAdd = -1;	break;
		case 'e':	widthAdd = 1;	break;
		case 's':	heightAdd = 1;	break;
		case 'w':	widthAdd = -1;	break;
		default:	break;
	}

	if (Labyrinth[currentPosition.height + heightAdd][currentPosition.width + widthAdd].type != 0)	// Collision Detection ftw!!111
	{
		movement.moveDirection = direction;		// set direction to move
		movement.counter = moveFrames;			// set counter to frames in which to move
		currentPosition.height += heightAdd;	// update currentPosition
		currentPosition.width += widthAdd;
	}
}


// Calls display FPS-times a second, also checks for movement to be done
void timer(int value)
{
	
	glutTimerFunc(1000/FPS, timer, 1); // call next frame

	if(movement.counter > 0)
	{

		if (movement.moveDirection != '.') // do walking
		{
			if(clockStarted == false)
			{
				clockStarted = true;
			}
			switch (movement.moveDirection)
			{
				case 'n':	advance -= moveValue;	break;	// move north
				
				case 's':	advance += moveValue;	break;	// move south

				case 'e':	sideways += moveValue;	break;	// move east

				case 'w':	sideways -= moveValue;	break;	// move west

				default:	break;
			}
		}

		if (movement.rotateDirection != '.') // do rotation
		{
			switch (movement.rotateDirection)
			{
				case 'r':	angle_y += rotateValue;		break;	// rotate right

				case 'l':	angle_y -= rotateValue;		break;	// rotate left

				default:	break;
			}
		}
		movement.counter--; // reduce counter of frames in which to move

		if (movement.counter == 0)
		{
			// manually override position after movement to compensate floating point operation inaccuracy
			advance = 2*MazeScale*currentPosition.height;
			sideways = 2*MazeScale*currentPosition.width;
			angle_y = 90.0 * currentPosition.lookDirection;

			if (currentPosition.height == end.height && currentPosition.width == end.width)
			{
				// Player has reached the end
				gameFinished = true;
				timeValueFinished = timeValue;
			}

			if (movement.rememberedKey != '.') // executes remembered keypress
			{
				keyPressed(movement.rememberedKey, 0, 0);
			}
		}
	}

	glutPostRedisplay();
}


void freeMemory()
{
	glutDestroyWindow(window);

	// free display lists
	for (int i = 0; i < 6; i++)
	{
		glDeleteLists(DLists[i], 1);
	}

	// free Labyrinth struct array
	for(int i = 0; i < height; i++)
	{
		free( (void *) Labyrinth[i] );
	}
	free( (void *) Labyrinth );
}