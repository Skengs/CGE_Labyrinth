#include <Header.h>

/*
	Siehe Aufbau der Textfiles in Files/
	Beim start der exe muss Files im selben Ordner sein, beim Debugging im Ordner der Source Files

	Idee ist folgende: 6 Fixfertige 3D-Bausteine verwenden, zusammengebastelt (und rotiert) ergeben sie unser Labyrinth.
	Jeder character im textfile stellt einen Baustein dar. Wände (#) werden ignoriert und dienen nur dazu typ der Gänge (.) nebenan zu bestimmen (zB: . mit # oben und rechts grenzend ist eine ecke).
	Man müsste zum rendern dann nur über alle Blöcke iterieren, anhand des typs, rotation und position im array objekte laden und fertig!

	// Einlesen und umwandeln zu structarray funktioniert
	// Glut-library geladen
	// 3D Labyrinth wird angezeigt
	// Man kann sich mit Maus und WASD bewegen (allerdings sehr fundamental)
	// Bei start wird kamera auf startposition gesetzt
	// Exitfunktion erstellt und eingebaut
	// Fixe kamerbewegung fertig
	// "Collision detection" fertig, man kann nicht mehr durch wände gehen :)

	BUGS:
		Aus irgendeinem grund zeigt er bei mir manchmal schwarze linien an wenn man herumgeht.
			Ka warum ... vielleicht einfach weil openGL nix drauf hat :P

	TODO:
		Irgendwas tun wenn man am ende ankommt
		Texturen für Wände
		Licht
		Spezielle Objekte
			Sachen die man einsammeln muss bevor man ans ende kann?
		Zeit stoppen?
		Sonstiges? :)
*/

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

	//t.join();

	//glutFullScreen();
	std::thread t(tikk);
	glutMainLoop();
	
	

	return 0;
}

void printInstructions()
{
	printf("\nThe blue orb marks your start, the red orb marks the end.\n\nUse [W] and [S] to move, [A] and [D] to rotate.\nPress [ESC] to exit.");
}