#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_HEIGHT 100
#define MAX_WIDTH 100

// Structure to represent the coordinates
typedef struct {
    int x;
    int y;
} Coord;

// Structure to represent a maze
typedef struct {
    char map[MAX_HEIGHT][MAX_WIDTH]; // 2D array to store the maze
    int height;
    int width;
    Coord start; // Start coordinate
    Coord end;   // End coordinate
} Maze;

// Function prototypes
void carve_path(Maze *maze, int x, int y);
void generate_maze(Maze *maze);
void place_start_and_end(Maze *maze);
void save_maze(Maze *maze, char *filename);

/**
 * @brief Carve paths in the maze using depth-first search (DFS) algorithm
 *
 * @param maze Pointer to the maze object
 * @param x Current x-coordinate
 * @param y Current y-coordinate
 */

void carve_path(Maze *maze, int x, int y) {
    // Lines 42-70 were adapted from code provided by the following conversation with ChatGPT:
    // https://chat.openai.com/share/8f1e1f34-7a5c-47d7-a9d8-11fdc33022c3 
    // Mark the current cell as visited
    maze->map[y][x] = ' ';

    // Define directions: Up, Down, Left, Right
    int directions[4][2] = {{0, -2}, {0, 2}, {-2, 0}, {2, 0}};

    // Shuffle the directions randomly
    for (int i = 0; i < 4; i++) {
        int j = rand() % 4;
        int tempX = directions[i][0];
        int tempY = directions[i][1];
        directions[i][0] = directions[j][0];
        directions[i][1] = directions[j][1];
        directions[j][0] = tempX;
        directions[j][1] = tempY;
    }

    // Visit neighbors in random order
    for (int i = 0; i < 4; i++) {
        int newX = x + directions[i][0];
        int newY = y + directions[i][1];

        // Check if the new position is within the maze boundaries
        if (newX > 0 && newX < maze->width && newY > 0 && newY < maze->height && maze->map[newY][newX] == '#') {
            // Carve a path to the new position
            maze->map[y + directions[i][1] / 2][x + directions[i][0] / 2] = ' ';
            carve_path(maze, newX, newY);
        }
    }
}

/**
 * @brief Generate a maze using depth-first search (DFS) algorithm
 *
 * @param maze Pointer to the maze object to be initialised
 */
void generate_maze(Maze *maze) {
    // Initialize maze with walls
    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            maze->map[i][j] = '#';
        }
    }

    // Carve path from start to end
    carve_path(maze, 1, 1);
}

/**
 * @brief Place start and end points in the maze (For now I will be printing the maze
 * and will implement the feature of adding the maze inside a text file as per the width
 * and height given by the user)
 *
 * @param maze Pointer to the maze object
 */
void place_start_and_end(Maze *maze) {
    // Generate random positions for start and end points
    // Lines 93-100 were adapted from code provided by the following conversation with ChatGPT:
    // https://chat.openai.com/share/acee34f1-f788-44d0-8ecb-2b3ac17d3757
    maze->start.x = rand() % (maze->width - 2) + 1;
    maze->start.y = rand() % (maze->height - 2) + 1;
    maze->end.x = rand() % (maze->width - 2) + 1;
    maze->end.y = rand() % (maze->height - 2) + 1;

    // Place start and end points in the maze
    maze->map[maze->start.y][maze->start.x] = 'S';
    maze->map[maze->end.y][maze->end.x] = 'E';
}

// Function to save the maze to a file
void save_maze(Maze *maze, char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Failed to open file for writing.\n");
        return;
    }

    // Write the maze to the file
    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            fputc(maze->map[i][j], file);
            fputc(' ', file); // Add space between maze characters for readability
        }
        fputc('\n', file); // Add newline after each row
    }

    fclose(file);
}


int main(int argc, char *argv[]) {
    // Check if the number of command-line arguments is not equal to 4
    if (argc != 4) {
        printf("Usage: %s <filename> <height> <width>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *filename = argv[1]; // Store filename from command-line argument
    int height = atoi(argv[2]); // Convert height from string to integer
    int width = atoi(argv[3]); // Convert width from string to integer

    // Check if height or width is not within valid range
    if (height < 5 || width < 5 || height > MAX_HEIGHT || width > MAX_WIDTH) {
        printf("Error: Invalid height or width.\n");
        return EXIT_FAILURE;
    }

    Maze maze;
    maze.height = height;
    maze.width = width;

    // Seed the random number generator
    srand(time(NULL));

    // Generate the maze
    generate_maze(&maze);

    // Place start and end points
    place_start_and_end(&maze);

    // Save the maze to the specified file
    save_maze(&maze, filename);

    printf("Maze generated and saved to %s.\n", filename);

    return EXIT_SUCCESS;
}
