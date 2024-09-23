/**
 * @file maze.c
 * @author Sinjini Sarkar (sc23ss2)
 * @brief Code for the maze game for COMP1921 Assignment 2
 * NOTE - You can remove or edit this file however you like - this is just a provided skeleton code
 * which may be useful to anyone who did not complete assignment 1.
 */


#include <stdio.h>
#include <stdlib.h>
#include "maze.h"



int main(int argc, char *argv[]) 
{
    // Print the title "MAZE GAME" using ASCII art
    printf("  ____                 __         ____              __  __                                                                     __\n");
    printf("   / __ \\___  ____ _____/ /_  __   / __/___  _____   / /_/ /_  ___     ____ ___  ____ _____  ___     ____ _____ _____ ___  ___  / /\n");
    printf("  / /_/ / _ \\/ __ `/ __  / / / /  / /_/ __ \\/ ___/  / __/ __ \\/ _ \\   / __ `__ \\/ __ `/_  / / _ \\   / __ `/ __ `/ __ `__ \\/ _ \\/ / \n");
    printf(" / _, _/  __/ /_/ / /_/ / /_/ /  / __/ /_/ / /     / /_/ / / /  __/  / / / / / / /_/ / / /_/  __/  / /_/ / /_/ / / / / / /  __/_/  \n");
    printf("/_/ |_|\\___/\\__,_/\\__,_/\\__, /  /_/  \\____/_/      \\__/_/ /_/\\___/  /_/ /_/ /_/\\__,_/ /___/\\___/   \\__, /\\__,_/_/ /_/ /_/\\___(_)   \n");
    printf("                       /____/                                                                     /____/                            \n");


    // Check args
    if (argc != 2) {
        printf("Usage: %s <maze_file>\n", argv[0]);
        return EXIT_ARG_ERROR;
    }

    // Open and validate mazefile
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error: Could not open file.\n");
        return EXIT_FILE_ERROR;
    }

    // Get maze dimensions
    int width = get_width(file);
    int height = get_height(file);
    
    if (width == 0 || height == 0) {
        printf("Error: Invalid maze dimensions.\n");
        fclose(file);
        return EXIT_MAZE_ERROR;
    }

    // Check maze dimensions
    if (width > MAX_DIM || height > MAX_DIM || width < MIN_DIM || height < MIN_DIM) {
        printf("Error: Maze dimensions out of bounds.\n");
        fclose(file);
        return EXIT_MAZE_ERROR;
    }

    // Allocate memory for maze
    maze this_maze;
    if (create_maze(&this_maze, height, width) != 0) {
        printf("Error: Memory allocation failed.\n");
        fclose(file);
        return EXIT_MAZE_ERROR;
    }

    // Read maze from file
    if (read_maze(&this_maze, file) != 0) {
        printf("Error: Invalid maze format.\n");
        free_maze(&this_maze);
        fclose(file);
        return EXIT_MAZE_ERROR;
    }

    // Close file
    fclose(file);

    // Set player start position
    coord player = this_maze.start;

    // Maze game loop
    char move_input;
    while (!has_won(&this_maze, &player)) {
        printf("Use WASD or wasd to move, M or m to see map: "); // Move this line here
        scanf(" %c", &move_input);
        if (move_input == 'M' || move_input == 'm') {
            print_maze(&this_maze, &player);
        } else {
            move(&this_maze, &player, move_input);
        }
    }

    // Player has won
    printf("Congratulations! You have solved the maze!\n");

    // Free allocated memory
    free_maze(&this_maze);

    return EXIT_SUCCESS;
}


/**
 * @brief Initialise a maze object - allocate memory and set attributes
 *
 * @param this pointer to the maze to be initialised
 * @param height height to allocate
 * @param width width to allocate
 * @return int 0 on success, 1 on fail
 */
int create_maze(maze *this, int height, int width) 
{
    // Allocate memory for maze map
    this->map = (char **)malloc(height * sizeof(char *));
    if (this->map == NULL) {
        printf("Error malloc failed\n");
        return 1; // Allocation failed
    }

    // Allocate memory for each row of the maze map
    // Lines 150-156 were adapted from code provided by the following conversation with ChatGPT:
    // https://chat.openai.com/share/74a7eb43-904a-41bb-a9b2-8a20c68044dc
    for (int i = 0; i < height; i++) {
        this->map[i] = (char *)malloc((width + 1) * sizeof(char));
        if (this->map[i] == NULL) {
            // Free previously allocated memory
            for (int j = 0; j < i; j++) {
                free(this->map[j]);
            }
            free(this->map);
            printf("Error: malloc failed\n");
            return 1; // Allocation failed
        }
    }

    // Set attributes for maze dimensions
    this->height = height;
    this->width = width;


    return 0; // Success
}


/**
 * @brief Free the memory allocated to the maze struct
 *
 * @param this the pointer to the struct to free
 */
void free_maze(maze *this) 
{
    if (this->map != NULL) {
        // Free memory for each row of the maze map
        for (int i = 0; i < this->height; i++) {
            free(this->map[i]);
        }
        // Free memory for the array of row pointers
        free(this->map);
    }
}


/**
 * @brief Validate and return the width of the mazefile
 *
 * @param file the file pointer to check
 * @return int 0 for error, or a valid width (5-100)
 */
int get_width(FILE *file) 
{
    int width = 0;
    char ch;

    // Iterate through the file character by character until reaching newline
    // Lines 203-206 and 226-229 were adapted from code provided by the following conversation with ChatGPT:
    // https://chat.openai.com/share/7ee35d83-cb41-43d9-beda-1f9519f1064a
    while ((ch = fgetc(file)) != '\n') {
            width++;
    }
    // 
    fseek(file, 0, SEEK_SET); // Reset file position indicator to the beginning of file
    return width;

    // Validation check done in the main() function
}


/**
 * @brief Validate and return the height of the mazefile
 *
 * @param file the file pointer to check
 * @return int 0 for error, or a valid height (5-100)
 */
int get_height(FILE *file) 
{
    int height = 0;
    char ch;

    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            height++;
        }
    }
    fseek(file, 0, SEEK_SET); // Reset file position indicator to the beginning of file
    return height;

    // Validation check done in the main() function
}

/**
 * @brief read in a maze file into a struct
 *
 * @param this Maze struct to be used
 * @param file Maze file pointer
 * @return int 0 on success, 1 on fail
 */
int read_maze(maze *this, FILE *file) 
{
    // Get width and height
    int width = get_width(file);
    int height = get_height(file);


    // Validate maze dimensions 
    if (width == 0 || height == 0) {
        printf("Invalid maze dimensions\n");
        return 1;
    }

    // Allocate memory for maze
    if (create_maze(this, height, width) != 0) {
        printf("Error malloc failed\n");
        return 1;
    }

    fseek(file, 0, SEEK_SET); // Reset file position indicator to the beginning of file

    // Read maze from file
    int start_count = 0, end_count = 0;
    for (int i = 0; i < height; i++) {
        fgets(this->map[i], width + 2, file);  // Read row into map row

        for (int j = 0; j < width; j++) {
            char c = this->map[i][j];
            

            if (c == 'S') {
                this->start.x = j;
                this->start.y = i;
                start_count++;
            } else if (c == 'E') {
                this->end.x = j;
                this->end.y = i;
                end_count++;
            } else if (!(c == ' ' || c == '#' || c == 'S' || c == 'E')) {
                return 1;
            }
        }
    }

    // Check if there is exactly one start and one end point
    if (start_count != 1 || end_count != 1) {
        return 1;
    }

    return 0;
}



/**
 * @brief Prints the maze out - code provided to ensure correct formatting
 *
 * @param this pointer to maze to print
 * @param player the current player location
 */
void print_maze(maze *this, coord *player)
{
    // make sure we have a leading newline..
    printf("\n");
    for (int i = 0; i < this->height; i++)
    {
        for (int j = 0; j < this->width; j++)
        {
            // decide whether player is on this spot or not
            if (player->x == j && player->y == i)
            {
                printf("X");
            }
            else
            {
                printf("%c", this->map[i][j]);
            }
        }
        // end each row with a newline.
        printf("\n");
    }
}

/**
 * @brief Validates and performs a movement in a given direction
 *
 * @param this Maze struct
 * @param player The player's current position
 * @param direction The desired direction to move in
 */
void move(maze *this, coord *player, char direction) 
{
    // Copy the player's current coordinates
    int new_x = player->x;
    int new_y = player->y;

     // Update the coordinates based on the given direction
    switch (direction) {
        case 'W':
        case 'w':
            new_y--;
            break;
        case 'A':
        case 'a':
            new_x--;
            break;
        case 'S':
        case 's':
            new_y++;
            break;
        case 'D':
        case 'd':
            new_x++;
            break;
        default:
            printf("Invalid move. Use WASD or wasd to move.\n");
            return;
    }

    // Check if new position is within bounds and is not a wall
    if (new_x >= 0 && new_x < this->width && new_y >= 0 && new_y < this->height &&
        this->map[new_y][new_x] != '#') {
        // Update the player's position if the move is valid
        player->x = new_x;
        player->y = new_y;
    } else {
        printf("Invalid move. You cannot move there.\n");
    }
}

/**
 * @brief Check whether the player has won and return a pseudo-boolean
 *
 * @param this current maze
 * @param player player position
 * @return int 0 for false, 1 for true
 */
int has_won(maze *this, coord *player) 
{
    return player->x == this->end.x && player->y == this->end.y;
}
