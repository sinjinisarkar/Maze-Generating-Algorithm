# Maze Game and Generator

# Summary
This project is a simple Maze Game implemented in C. Players navigate through a maze loaded from a file, starting at a designated point (S) and making their way to the exit (E). The game takes user input for movement (WASD) and displays the maze on request, marking the player's current position (X). 

# Specification
The maze files must follow these rules:

* Contain only the following characters:
    * `#` for walls
    * `S` for the starting point
    * `E` for the exit point
    * (space) for navigable paths
    * `\n` for newlines
* The maze must be rectangular, with consistent row lengths.
* Maximum maze size: 100 x 100.
* Minimum maze size: 5 x 5.
* Each maze must have exactly **one starting point** (`S`) and **one exit** (`E`).

# Compilation & Running
### To Compile:
1) gcc maze.c -o maze
2) gcc mazegen.c -o mazegen

### To Run the Game:
./maze <mazefile>

(Replace mazefile with the maze files given)

### To Run the Maze Generator:
./mazegen <output_mazefile> <width> <height> 

(Replace output_mazefile with your desired file name and replace width and height with your preferred dimensions) 

# Documentation
You can find a detailed report about the maze genertor here in my [Powerpoint](https://leeds365-my.sharepoint.com/:p:/g/personal/sc23ss2_leeds_ac_uk/EXXVEnmYD2lDp3KTpl0VbbcBouivRl5WdrkAw0T5oXBJWg)
