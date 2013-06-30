#include <Header.h>

extern GLuint DLists[6], texture[3];


void reportGLError(const char * msg)
{
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR)
	{
		errString = gluErrorString(errCode);
		fprintf(stderr, "OpenGL Error: %s %s\n", msg, errString);
	}
	return;
}

void loadTextures(tgaInfo *info, GLuint *texture)
{
	GLsizei w, h;

	int mode;

	if (info->status != TGA_OK) {
		fprintf(stderr, "error loading texture image: %d\n", info->status);
    
		return;
	}

	mode = info->pixelDepth / 8;  // will be 3 for rgb, 4 for rgba
	glGenTextures(1, texture);

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
	glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, info->imageData);
	reportGLError("after uploading texture");

	tgaDestroy(info);
}

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
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glBegin(GL_QUADS);
			//glColor3f(0.6, 0.6, 0.6);

			// Floor
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y,  z);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( x, -y,  z);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( x, -y, -z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-x, -y, -z);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glBegin(GL_QUADS);

			// ceiling
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-x,  y,  z);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( x,  y,  z);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( x,  y, -z);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-x,  y, -z);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glBegin(GL_QUADS);
			// west wall
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y,  z);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-x, -y, -z);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-x,  y, -z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-x,  y,  z);

			// east wall
			glTexCoord2f(0.0f, 0.0f); glVertex3f( x, -y,  z);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( x, -y, -z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( x,  y, -z);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( x,  y,  z);

			// south wall
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-x, -y,  z);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( x, -y,  z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( x,  y,  z);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-x,  y,  z);

			// north wall
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y, -z);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( x, -y, -z);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( x,  y, -z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-x,  y, -z);

		glEnd();
	glEndList();

	// [1] - DeadEndShape
	glNewList(DLists[1], GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glBegin(GL_QUADS);

			// floor
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y,  z);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( x, -y,  z);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( x, -y, -z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-x, -y, -z);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glBegin(GL_QUADS);
			// ceiling
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-x,  y,  z);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( x,  y,  z);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( x,  y, -z);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-x,  y, -z);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glBegin(GL_QUADS);

			// west wall
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y,  z);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-x, -y, -z);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-x,  y, -z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-x,  y,  z);

			// south wall
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-x, -y,  z);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( x, -y,  z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( x,  y,  z);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-x,  y,  z);

			// north wall
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y, -z);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( x, -y, -z);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( x,  y, -z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-x,  y, -z);

		glEnd();
	glEndList();

	// [2] - CornerShape
	glNewList(DLists[2], GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glBegin(GL_QUADS);
			
			// Floor
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y,  z);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( x, -y,  z);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( x, -y, -z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-x, -y, -z);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glBegin(GL_QUADS);

			// ceiling
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-x,  y,  z);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( x,  y,  z);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( x,  y, -z);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-x,  y, -z);
			glEnd();

		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glBegin(GL_QUADS);

			// west wall
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y,  z);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-x, -y, -z);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-x,  y, -z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-x,  y,  z);

			// north wall
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y, -z);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( x, -y, -z);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( x,  y, -z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-x,  y, -z);

		glEnd();
	glEndList();

	// [3] - CorridorShape
	glNewList(DLists[3], GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glBegin(GL_QUADS);

			// Floor
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y,  z);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( x, -y,  z);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( x, -y, -z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-x, -y, -z);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glBegin(GL_QUADS);

			// ceiling
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-x,  y,  z);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( x,  y,  z);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( x,  y, -z);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-x,  y, -z);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glBegin(GL_QUADS);

			// south wall
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-x, -y,  z);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( x, -y,  z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( x,  y,  z);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-x,  y,  z);

			// north wall
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y, -z);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( x, -y, -z);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( x,  y, -z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-x,  y, -z);
			
		glEnd();
	glEndList();

	// [4] - BorderShape
	glNewList(DLists[4], GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glBegin(GL_QUADS);
			
			// Floor
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y,  z);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( x, -y,  z);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( x, -y, -z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-x, -y, -z);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glBegin(GL_QUADS);

			// ceiling
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-x,  y,  z);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( x,  y,  z);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( x,  y, -z);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-x,  y, -z);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glBegin(GL_QUADS);

			// north wall
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y, -z);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( x, -y, -z);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( x,  y, -z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-x,  y, -z);

		glEnd();
	glEndList();

	// [5] - EmptyShape
	glNewList(DLists[5], GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glBegin(GL_QUADS);
			
			// Floor
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y,  z);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( x, -y,  z);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( x, -y, -z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-x, -y, -z);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glBegin(GL_QUADS);

			// ceiling
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-x,  y,  z);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( x,  y,  z);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( x,  y, -z);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-x,  y, -z);
		glEnd();
	glEndList();
}