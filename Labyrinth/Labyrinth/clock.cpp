#include <Header.h>

extern int width, height;

extern int timeValue;
extern int timeValueFinished;
extern char timeChar[1000];
extern bool clockStarted;


void clock_format(int t)
{
	int seconds = t % 60;
	int minutes = (t / 60) % 60;
	int hours = t / 3600;

	sprintf_s(timeChar,"%01d:%02d:%02d", hours, minutes, seconds);
}

//Display 2dText
void clock_display(char *text)
{
	size_t i;
	// backup current modelview matrix
    glPushMatrix();
    // reset modelview matrix
    glLoadIdentity();
 
    // set to 2D orthogonal projection =======
    // switch to projection matrix
    glMatrixMode(GL_PROJECTION);
    // backup projection matrix
    glPushMatrix();
    // reset projection matrix
    glLoadIdentity();
    // set to orthogonal projection
	glOrtho(0, width, 0, height, -1.0, 1.0);
 
    // draw the text ============================
    //GLUT glut = new GLUT();
    glColor3f(10.0f, 10.0f, 0.0f);
	glRasterPos2f(0.1, 0.1);
	//glutBitmapString(GLUT_BITMAP_HELVETICA_18, string);
	for(i = 0;i<strlen(text);i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*(text+i));
	}
    //===========================================
 
    // restore previous projection matrix
    glPopMatrix();
 
    // restore previous modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

//Clock thread
void tikk()
{
	timeValue = 0;

	while( timeValue < 3601 )
	{
		if(clockStarted == true)
		{
			timeValue++;
			Sleep(1000);
		}
	}
	char end[] = {" You Lost!\0"};
	clock_display(end);
	freeMemory();
	exit(0);
}