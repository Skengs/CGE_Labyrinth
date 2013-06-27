#include <Header.h>

// Labyrinth data
extern struct MazeElement** Labyrinth;
extern int width, height;

extern struct ArrayPosition start, end;
struct ArrayPosition currentPosition;

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
GLuint DLists[6],texture[3];

void reportGLError(const char * msg)
{
  GLenum errCode;
  const GLubyte *errString;
  while ((errCode = glGetError()) != GL_NO_ERROR) {
    errString = gluErrorString(errCode);
    fprintf(stderr, "OpenGL Error: %s %s\n", msg, errString);
  }
  return;
}

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

	// Camera lookdirection
	gluLookAt(-sinf(RAD(angle_y)),sinf(RAD(angle_x)), cosf(RAD(angle_y)),
			0, 0, 0,
			0, 1, 0);

	// Movement (Initial camera position already included)
	glTranslatef(-sideways,0,-advance);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_DECAL);
	//glBindTexture(GL_TEXTURE_2D, texture[0]);
	// draw Labyrinth
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
			}
			glTranslatef(2*MazeScale, 0, 0); // Move east (positive x-achse) after each block
		}
		glPopMatrix();
		glTranslatef(0, 0, 2*MazeScale); // Move south after each line (positive z-achse)
	}
	glPopMatrix();

	//glDisable(GL_TEXTURE_2D);
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
	
	//Floor Texture
	GLsizei w, h,w2,h2,w3,h3;
  tgaInfo *info = 0;
  tgaInfo *info2 = 0;
  tgaInfo *info3 = 0;
  int mode, mode2,mode3;

  info = tgaLoad(".\\Textures\\stonesfloor.tga");

  if (info->status != TGA_OK) {
    fprintf(stderr, "error loading texture image: %d\n", info->status);
    
    return;
  }
  //Für Rechteckige Bild überprüfung
  /*if (info->width != info->height) {
    fprintf(stderr, "Image size %d x %d is not rectangular, giving up.\n",
            info->width, info->height);
    return;
  }*/

  mode = info->pixelDepth / 8;  // will be 3 for rgb, 4 for rgba
  glGenTextures(1, &(texture[0]));

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glBindTexture(GL_TEXTURE_2D, texture[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

  // Upload the texture bitmap. 
  w  = info->width; 
  h = info->height; 

  reportGLError("before uploading texture");
  GLint format = (mode == 4) ? GL_RGBA : GL_RGB;
  glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, 
               GL_UNSIGNED_BYTE, info->imageData);
  reportGLError("after uploading texture");

  tgaDestroy(info);


  //Wall Texture
  info2 = tgaLoad(".\\Textures\\wall.tga");

  if (info2->status != TGA_OK) {
    fprintf(stderr, "error loading texture image: %d\n", info2->status);
    
    return;
  }
  //Für Rechteckige Bild überprüfung
  /*if (info->width != info->height) {
    fprintf(stderr, "Image size %d x %d is not rectangular, giving up.\n",
            info->width, info->height);
    return;
  }*/

  mode2 = info2->pixelDepth / 8;  // will be 3 for rgb, 4 for rgba
  glGenTextures(1, &texture[1]);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glBindTexture(GL_TEXTURE_2D, texture[1]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

  // Upload the texture bitmap. 
  w2  = info2->width; 
  h2 = info2->height; 

  reportGLError("before uploading texture");
  GLint format2 = (mode2 == 4) ? GL_RGBA : GL_RGB;
  glTexImage2D(GL_TEXTURE_2D, 0, format2, w2, h2, 0, format2, 
               GL_UNSIGNED_BYTE, info2->imageData);
  reportGLError("after uploading texture");

  tgaDestroy(info2);

  //Ceiling Texture
  info3 = tgaLoad(".\\Textures\\ceiling.tga");

  if (info3->status != TGA_OK) {
    fprintf(stderr, "error loading texture image: %d\n", info3->status);
    
    return;
  }
  //Für Rechteckige Bild überprüfung
  /*if (info->width != info->height) {
    fprintf(stderr, "Image size %d x %d is not rectangular, giving up.\n",
            info->width, info->height);
    return;
  }*/

  mode3 = info3->pixelDepth / 8;  // will be 3 for rgb, 4 for rgba
  glGenTextures(1, &texture[2]);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glBindTexture(GL_TEXTURE_2D, texture[2]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

  // Upload the texture bitmap. 
  w3  = info3->width; 
  h3 = info3->height; 

  reportGLError("before uploading texture");
  GLint format3 = (mode3 == 4) ? GL_RGBA : GL_RGB;
  glTexImage2D(GL_TEXTURE_2D, 0, format3, w3, h3, 0, format3, 
               GL_UNSIGNED_BYTE, info2->imageData);
  reportGLError("after uploading texture");

  tgaDestroy(info3);
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
			break;

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
			}
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
			}
			break;

		case 'a':	// rotate 90° left
			if(movement.counter == 0) // if not moving already
			{
				movement.moveDirection = '.'; // disable walking
				movement.rotateDirection = 'l';
				movement.counter = rotateFrames;
				currentPosition.lookDirection = (currentPosition.lookDirection + 3) % 4; // update current lookdirection
			}
			break;

		case 'd':	// rotate 90° right
			if(movement.counter == 0) // if not moving already
			{
				movement.moveDirection = '.'; // disable walking
				movement.rotateDirection = 'r';
				movement.counter = rotateFrames;
				currentPosition.lookDirection = (currentPosition.lookDirection + 1) % 4; // update current lookdirection
			}
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


// Calls display FPS-times a minute, also checks for movement to be done
void timer(int value)
{
	glutTimerFunc(1000/FPS, timer, 1); // call next frame

	if(movement.counter > 0)
	{
		if (movement.moveDirection != '.') // do walking
		{
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

		if (movement.counter == 0) // manually override position after movement to compensate floating point operation inaccuracy
		{
			advance = 2*MazeScale*currentPosition.height;
			sideways = 2*MazeScale*currentPosition.width;
			angle_y = 90.0 * currentPosition.lookDirection;
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