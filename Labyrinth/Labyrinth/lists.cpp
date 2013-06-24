#include <Header.h>

extern GLuint DLists[6];

/*
	Create a display list for every blockshape
*/
void initLists()
{
	GLfloat x, y, z;
	x = y = z = 1 * MazeScale;

	for (int i = 0; i < 6; i++)
	{
		DLists[i] = glGenLists(1);
	}

	// Following builds quads around the center point (0, 0, 0)

	// [0] - SquareShape
	glNewList(DLists[0], GL_COMPILE);
		glBegin(GL_QUADS);
			glColor3f(0.6, 0.6, 0.6);

			// Floor
			glVertex3f(-x, -y,  z);
			glVertex3f( x, -y,  z);
			glVertex3f( x, -y, -z);
			glVertex3f(-x, -y, -z);

			// ceiling
			glVertex3f(-x,  y,  z);
			glVertex3f( x,  y,  z);
			glVertex3f( x,  y, -z);
			glVertex3f(-x,  y, -z);

			glColor3f(0.8, 0.8, 0.8);

			// west wall
			glVertex3f(-x, -y,  z);
			glVertex3f(-x, -y, -z);
			glVertex3f(-x,  y, -z);
			glVertex3f(-x,  y,  z);

			// east wall
			glVertex3f( x, -y,  z);
			glVertex3f( x, -y, -z);
			glVertex3f( x,  y, -z);
			glVertex3f( x,  y,  z);

			// south wall
			glVertex3f(-x, -y,  z);
			glVertex3f( x, -y,  z);
			glVertex3f( x,  y,  z);
			glVertex3f(-x,  y,  z);

			// north wall
			glVertex3f(-x, -y, -z);
			glVertex3f( x, -y, -z);
			glVertex3f( x,  y, -z);
			glVertex3f(-x,  y, -z);
		glEnd();
	glEndList();

	// [1] - DeadEndShape
	glNewList(DLists[1], GL_COMPILE);
		glBegin(GL_QUADS);
			glColor3f(0.6, 0.6, 0.6);

			// floor
			glVertex3f(-x, -y,  z);
			glVertex3f( x, -y,  z);
			glVertex3f( x, -y, -z);
			glVertex3f(-x, -y, -z);

			// ceiling
			glVertex3f(-x,  y,  z);
			glVertex3f( x,  y,  z);
			glVertex3f( x,  y, -z);
			glVertex3f(-x,  y, -z);

			glColor3f(0.8, 0.8, 0.8);

			// west wall
			glVertex3f(-x, -y,  z);
			glVertex3f(-x, -y, -z);
			glVertex3f(-x,  y, -z);
			glVertex3f(-x,  y,  z);

			// south wall
			glVertex3f(-x, -y,  z);
			glVertex3f( x, -y,  z);
			glVertex3f( x,  y,  z);
			glVertex3f(-x,  y,  z);

			// north wall
			glVertex3f(-x, -y, -z);
			glVertex3f( x, -y, -z);
			glVertex3f( x,  y, -z);
			glVertex3f(-x,  y, -z);
		glEnd();
	glEndList();

	// [2] - CornerShape
	glNewList(DLists[2], GL_COMPILE);
		glBegin(GL_QUADS);
			glColor3f(0.6, 0.6, 0.6);

			// Floor
			glVertex3f(-x, -y,  z);
			glVertex3f( x, -y,  z);
			glVertex3f( x, -y, -z);
			glVertex3f(-x, -y, -z);

			// ceiling
			glVertex3f(-x,  y,  z);
			glVertex3f( x,  y,  z);
			glVertex3f( x,  y, -z);
			glVertex3f(-x,  y, -z);

			glColor3f(0.8, 0.8, 0.8);

			// west wall
			glVertex3f(-x, -y,  z);
			glVertex3f(-x, -y, -z);
			glVertex3f(-x,  y, -z);
			glVertex3f(-x,  y,  z);

			// north wall
			glVertex3f(-x, -y, -z);
			glVertex3f( x, -y, -z);
			glVertex3f( x,  y, -z);
			glVertex3f(-x,  y, -z);
		glEnd();
	glEndList();

	// [3] - CorridorShape
	glNewList(DLists[3], GL_COMPILE);
		glBegin(GL_QUADS);
			glColor3f(0.6, 0.6, 0.6);

			// Floor
			glVertex3f(-x, -y,  z);
			glVertex3f( x, -y,  z);
			glVertex3f( x, -y, -z);
			glVertex3f(-x, -y, -z);

			// ceiling
			glVertex3f(-x,  y,  z);
			glVertex3f( x,  y,  z);
			glVertex3f( x,  y, -z);
			glVertex3f(-x,  y, -z);

			glColor3f(0.8, 0.8, 0.8);

			// south wall
			glVertex3f(-x, -y,  z);
			glVertex3f( x, -y,  z);
			glVertex3f( x,  y,  z);
			glVertex3f(-x,  y,  z);

			// north wall
			glVertex3f(-x, -y, -z);
			glVertex3f( x, -y, -z);
			glVertex3f( x,  y, -z);
			glVertex3f(-x,  y, -z);
		glEnd();
	glEndList();

	// [4] - BorderShape
	glNewList(DLists[4], GL_COMPILE);
		glBegin(GL_QUADS);
			glColor3f(0.6, 0.6, 0.6);

			// Floor
			glVertex3f(-x, -y,  z);
			glVertex3f( x, -y,  z);
			glVertex3f( x, -y, -z);
			glVertex3f(-x, -y, -z);

			// ceiling
			glVertex3f(-x,  y,  z);
			glVertex3f( x,  y,  z);
			glVertex3f( x,  y, -z);
			glVertex3f(-x,  y, -z);

			glColor3f(0.8, 0.8, 0.8);

			// north wall
			glVertex3f(-x, -y, -z);
			glVertex3f( x, -y, -z);
			glVertex3f( x,  y, -z);
			glVertex3f(-x,  y, -z);
		glEnd();
	glEndList();

	// [5] - EmptyShape
	glNewList(DLists[5], GL_COMPILE);
		glBegin(GL_QUADS);
			glColor3f(0.6, 0.6, 0.6);

			// floor
			glVertex3f(-x, -y,  z);
			glVertex3f( x, -y,  z);
			glVertex3f( x, -y, -z);
			glVertex3f(-x, -y, -z);

			// ceiling
			glVertex3f(-x,  y,  z);
			glVertex3f( x,  y,  z);
			glVertex3f( x,  y, -z);
			glVertex3f(-x,  y, -z);
		glEnd();
	glEndList();
}