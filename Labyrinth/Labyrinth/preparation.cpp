#include <Header.h>

struct MazeElement** ConvertToShapes (FILE* file, int width, int height, struct ArrayPosition* start, struct ArrayPosition* end)
{

	int i, j;
	char c;

	// dynamic 2dim array
	char **TextMaze = (char **) malloc(height * sizeof(char *));
	for(i = 0; i < height; i++)
		TextMaze[i] = (char *) malloc(width * sizeof(char));

	//   _-*The Datastructure*-_ ... 2D struct array
	struct MazeElement **Labyrinth = (struct MazeElement **) malloc(height * sizeof(struct MazeElement *));
	for(i = 0; i < height; i++)
		Labyrinth[i] = (struct MazeElement *) malloc(width * sizeof(struct MazeElement));

	// initialising Labyrinth
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			Labyrinth[i][j].type = 0;
			Labyrinth[i][j].rotation = 0;
		}
	}


	rewind(file);
	printf("Converting Labyrinth to useful data ... ");
	
	// reading file into array
	for (i = 0, j = 0, c = fgetc(file); c != EOF; c = fgetc(file))
	{
		if (c != '\n')
		{
			TextMaze[i][j] = c;
			j++;
		}
		else
		{
			i++;
			j = 0;
		}
	}

	// For every non-wall, checking all adjacent segments for walls --> deciding on shape --> saving shape to Labyrinth
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			if (TextMaze[i][j] != '#')
			{

				// Sets start and end to globals in main
				if (TextMaze[i][j] == 'S')
				{
					start->height = i;
					start->width = j;
				}
				else if (TextMaze[i][j] == 'E')
				{
					end->height = i;
					end->width = j;
				}
				

				int value = 0;

				if (TextMaze[i-1][j] == '#') // North
					value += 1;

				if (TextMaze[i][j+1] == '#') // East
					value += 10;

				if (TextMaze[i+1][j] == '#') // South
					value += 100;

				if (TextMaze[i][j-1] == '#') // West
					value += 1000;

				switch (value)
				{
					// Note: Rotation right -> negative value && rotation left -> positive value

					// EmptyShape
					case 0:	// No walls
						Labyrinth[i][j].type = EmptyShape;
						Labyrinth[i][j].rotation = 0;
						break;

					// BorderShapes
					case 1:		// Single wall north
						Labyrinth[i][j].type = BorderShape;
						Labyrinth[i][j].rotation = 0;
						break;

					case 10:	// Single wall east
						Labyrinth[i][j].type = BorderShape;
						Labyrinth[i][j].rotation = -90.0;
						break;

					case 100:	// Single wall south
						Labyrinth[i][j].type = BorderShape;
						Labyrinth[i][j].rotation = -180.0;
						break;

					case 1000:	// Single wall west
						Labyrinth[i][j].type = BorderShape;
						Labyrinth[i][j].rotation = -270.0;
						break;

					// CornerShapes
					case 11:	// Wall north and east
						Labyrinth[i][j].type = CornerShape;
						Labyrinth[i][j].rotation = -90.0;
						break;

					case 110:	// Wall east and south
						Labyrinth[i][j].type = CornerShape;
						Labyrinth[i][j].rotation = -180.0;
						break;

					case 1100:	// Wall south and west
						Labyrinth[i][j].type = CornerShape;
						Labyrinth[i][j].rotation = -270.0;
						break;

					case 1001:	// Wall west and north
						Labyrinth[i][j].type = CornerShape;
						Labyrinth[i][j].rotation = 0;
						break;

					// CorridorShapes
					case 101:	// Wall north and south
						Labyrinth[i][j].type = CorridorShape;
						Labyrinth[i][j].rotation = 0;
						break;

					case 1010:	// Wall east and west
						Labyrinth[i][j].type = CorridorShape;
						Labyrinth[i][j].rotation = -90.0;
						break;

					// DeadEndShapes
					case 1110:	// No wall north
						Labyrinth[i][j].type = DeadEndShape;
						Labyrinth[i][j].rotation = -270.0;
						break;

					case 1101:	// No wall east
						Labyrinth[i][j].type = DeadEndShape;
						Labyrinth[i][j].rotation = 0;
						break;

					case 1011:	// No wall south
						Labyrinth[i][j].type = DeadEndShape;
						Labyrinth[i][j].rotation = -90.0;
						break;

					case 111:	// No wall west
						Labyrinth[i][j].type = DeadEndShape;
						Labyrinth[i][j].rotation = -180.0;
						break;

					// SquareShape
					case 1111:	// Walls everywhere (whyever you would do that)
						Labyrinth[i][j].type = SquareShape;
						Labyrinth[i][j].rotation = 0;
						break;

					default:
						printf("Error! Someone just fucked up the code.\n");
				}

			}
		}
	}


	// free TextMaze array
	for(i = 0; i < height; i++)
		free((void *)TextMaze[i]);
	free((void *)TextMaze);

	printf("Done!\n");

	return Labyrinth;

}