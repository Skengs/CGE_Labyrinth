#include <Header.h>

// Tries to open file, returns filehandler on success, NULL on error
FILE* CheckExistence(char* name)
{
	FILE* file = NULL;
	char filename[150];
	errno_t err;

	// get rid of \n at the end of string
	for (int i = 0; i < 100; i++)
	{
		if(*(name + i) == '\n')
		{
			*(name + i) = '\0';
			break;
		}
	}


	printf("Checking file for existence ... ");

	strcpy_s(filename, "./Files/");
	strcat_s(filename, name);
	strcat_s(filename, ".txt");

	err = fopen_s(&file, filename, "r");

	if(file == NULL)
	{
		printf("Error! Unable to open Files/%s.txt.\n", name);
		return file;
	}
	printf("Done!\n");



	printf("Printing Content ...\n\n");

	char buffer[MazeLength + 1];
	while(fgets(buffer, 101, file) != NULL)
	{
		printf("%s", buffer);
	}
	printf("\n\n");
	

	return file;
}

bool ValidateContent(FILE* file, int* _width, int* _height)
{
	char line[MazeLength + 1];
	char c;
	bool lastline = false;

	int width, height;
	bool start = false;
	bool exit = false;

	rewind(file);

	printf("Validating Labyrinth's content ... ");

	// check first line for # (borderline). Also counts width.
	for (width = 0, height = 0, c = fgetc(file); c != EOF;)
	{
		if (c == '#')
		{
			width++;
		}
		else if (c == '\n')
		{
			height++;
			break;
		}
		else
		{
			printf("Error! First line has no correct border.\n");
			return false;
		}

		c = fgetc(file);
	}

	if (width > MazeLength)
	{
		printf("Error! Labyrinth is too wide (<= %d).\n", MazeLength);
		return false;
	}

	// read all other lines
	while(fgets(line, 101, file) != NULL)
    {
		height++;
		if(height > MazeLength)
		{
			printf("Error! Labyrinth is too high (<= %d).\n", MazeLength);
			return false;
		}

		//compare this line's length with first line's
		int length = strlen(line);
		if(length-1 != width)
		{
			if(length == width)
			{
				// current line is last line (has no ending '\n')
				for (int i = 0; i < length; i++)
				{
					if (line[i] != '#')
					{
						printf("Error! Last line has no correct border.\n\t... or there are extra empty lines.");
						return false;
					}
				}
				break; // breaks loop to finish validating
			}
			else
			{
				printf("Error! Line %d has wrong length.\n", height);
				return false;
			}
		}

		// check beginng and end of line
		if (line[0] != '#' || line[length-2] != '#')
		{
			printf("Error! Line %d has no correct border.\n", height);
			return false;
		}

		//check content of line
		for (int i = 1; i < length-2; i++)
		{
			if (line[i] != '#' && line[i] != '.')	// Normal characters
			{
				switch (line[i])					// Special characters
				{
					case 'S':
						if (!start) // S not found yet
						{
							start = true;
						}
						else
						{
							printf("Error! Found 'S' a second time in line %d.\n", height);
							return false;
						}
						break;

					case 'E':
						if (!exit) // E not found yet
						{
							exit = true;
						}
						else
						{
							printf("Error! Found 'E' a second time in line %d.\n", height);
							return false;
						}
						break;

					default:
						printf("Error! Found unknown character '%c' in line %d.\n", line[i], height);
						return false;
				}
			}
		}
    }

	if (!start || !exit)
	{
		printf("Error! No start and/or exit found.\n");
		return false;
	}

	printf("Done!\n");

	*_width = width;
	*_height = height;

	return true;
}