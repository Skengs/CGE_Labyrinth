#include <Header.h>


// Labyrinth data
int width, height;
struct MazeElement** Labyrinth;

struct ArrayPosition start, end;

// GLUT window
int window;

int main(int argc, char **argv) 
{  

	char name[100];
	FILE* file;

	printf("Please name your Labyrinth textfile:\n");
	fgets(name, 100, stdin);

	if ( (file = CheckExistence(name)) == NULL)
	{
		printf("\nPress any key to exit.\n");
		_getch();
		return 1;
	}

	// If successful, automatically sets width and height
	if (!ValidateContent(file, &width, &height))
	{
		fclose(file);
		printf("\nPress any key to exit.\n");
		_getch();
		return 1;
	}

	// Automatically sets start and end position
	Labyrinth = ConvertToShapes(file, width, height, &start, &end);

	fclose(file);

	// GLUT
	printf("Initialising openGL ... ");

	glutInit(&argc, argv);  
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);  
	glutInitWindowSize(640, 480);  
	glutInitWindowPosition(0, 0);  
	window = glutCreateWindow(name);  
	glutDisplayFunc(&display);  
	glutReshapeFunc(&resize);
	glutKeyboardFunc(&keyPressed);
	init(640, 480);
	glutTimerFunc(15, timer, 1);

	printf("Done!\n\nStarting 3D visualisation!\n");
	printInstructions();

	//glutFullScreen();
	std::thread t(tikk);

	glutMainLoop();
	
	
	return 0;
}

void printInstructions()
{
	printf("\nThe blue orb marks your start, the red orb marks the exit.\n\nUse [W] and [S] to move, [A] and [D] to rotate.\nPress [ESC] to exit.");
}