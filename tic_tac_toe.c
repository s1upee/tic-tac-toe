#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // For sleep()

// Grid size
#define GRID_SIZE 3

// Function to display the start menu and choose the game mode
int start_menu(int *ai_level) {
    clear();
    mvprintw(5, 5, "Welcome to Tic-Tac-Toe!");
    mvprintw(7, 5, "Choose game mode:");
    mvprintw(8, 5, "1. Two Players");
    mvprintw(9, 5, "2. Play against AI");
    mvprintw(11, 5, "Enter your choice (1 or 2):");
    refresh();

    int choice;
    while (1) {
        choice = getch();
        if (choice == '1' || choice == '2') {
            break;
        }
    }

    if (choice == '2') {
        clear();
        mvprintw(5, 5, "Choose AI level:");
        mvprintw(7, 5, "1. Easy (Random moves)");
        mvprintw(8, 5, "2. Hard (Strategic moves)");
        mvprintw(10, 5, "Enter your choice (1 or 2):");
        refresh();

        while (1) {
            *ai_level = getch();
            if (*ai_level == '1' || *ai_level == '2') {
                *ai_level -= '0'; // Convert char to int
                break;
            }
        }
    }

    return choice - '0'; // Convert char to int
}

// Function to draw the grid
void draw_grid() {
    int y_offset = 2; // Смещение сетки вниз
    for (int i = 1; i <= GRID_SIZE; i++) {
        mvhline(i * 2 + y_offset, 1, '-', 11); // Horizontal lines
    }
    for (int i = 1; i <= GRID_SIZE; i++) {
        mvvline(y_offset + 1, i * 4, '|', 5); // Vertical lines
    }
    for (int y = 2 + y_offset; y <= 6 + y_offset; y += 2) {
        for (int x = 4; x <= 12; x += 4) {
            mvaddch(y, x, '+'); // Intersections
        }
    }
}

// Function to print the current state of the grid
void print_grid(char grid[GRID_SIZE][GRID_SIZE]) {
    int y_offset = 2; // Смещение сетки вниз
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            int y = row * 2 + 1 + y_offset;
            int x = col * 4 + 2;
            mvprintw(y, x, "%c", grid[row][col]);
        }
    }
}

// Check for a winner
char check_winner(char grid[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; i++) {
        if (grid[i][0] != ' ' && grid[i][0] == grid[i][1] && grid[i][1] == grid[i][2])
            return grid[i][0]; // Row match
        if (grid[0][i] != ' ' && grid[0][i] == grid[1][i] && grid[1][i] == grid[2][i])
            return grid[0][i]; // Column match
    }

    if (grid[0][0] != ' ' && grid[0][0] == grid[1][1] && grid[1][1] == grid[2][2])
        return grid[0][0];
    if (grid[0][2] != ' ' && grid[0][2] == grid[1][1] && grid[1][1] == grid[2][0])
        return grid[0][2];

    return ' '; // No winner
}

// Check for a draw
int is_draw(char grid[GRID_SIZE][GRID_SIZE]) {
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            if (grid[row][col] == ' ') {
                return 0; // Empty cell found, not a draw
            }
        }
    }
    return 1; // No empty cells, it's a draw
}

// Function to get player names
void get_player_names(char player_x_name[], char player_o_name[]) {
    clear();
    mvprintw(5, 5, "Enter name for Player X: ");
    echo();
    getstr(player_x_name);
    mvprintw(7, 5, "Enter name for Player O: ");
    getstr(player_o_name);
    noecho();
}

// Function to display end game menu
int display_menu() {
    clear();
    mvprintw(5, 5, "Game Over!");
    mvprintw(7, 5, "Do you want to play again?");
    mvprintw(8, 5, "Press 'y' for Yes or 'n' for No.");
    refresh();

    int choice;
    while (1) {
        choice = getch();
        if (choice == 'y' || choice == 'n') {
            break;
        }
    }
    return choice;
}

// Function for AI to make a move
void ai_move(char grid[GRID_SIZE][GRID_SIZE]) {
    int row, col;
    srand(time(0)); // Seed for random number generator
    while (1) {
        row = rand() % GRID_SIZE; // Random row (0 to 2)
        col = rand() % GRID_SIZE; // Random column (0 to 2)
        if (grid[row][col] == ' ') { // Check if cell is empty
            grid[row][col] = 'O'; // AI places its mark
            break;
        }
    }
}

// Hard AI logic: tries to win or block the player
void hard_ai_move(char grid[GRID_SIZE][GRID_SIZE]) {
    // Try to win or block
    for (int i = 0; i < GRID_SIZE; i++) {
        // Check rows
        if (grid[i][0] == 'O' && grid[i][1] == 'O' && grid[i][2] == ' ') {
            grid[i][2] = 'O';
            return;
        }
        if (grid[i][0] == ' ' && grid[i][1] == 'O' && grid[i][2] == 'O') {
            grid[i][0] = 'O';
            return;
        }
        if (grid[i][0] == 'O' && grid[i][1] == ' ' && grid[i][2] == 'O') {
            grid[i][1] = 'O';
            return;
        }

        // Check columns
        if (grid[0][i] == 'O' && grid[1][i] == 'O' && grid[2][i] == ' ') {
            grid[2][i] = 'O';
            return;
        }
        if (grid[0][i] == ' ' && grid[1][i] == 'O' && grid[2][i] == 'O') {
            grid[0][i] = 'O';
            return;
        }
        if (grid[0][i] == 'O' && grid[1][i] == ' ' && grid[2][i] == 'O') {
            grid[1][i] = 'O';
            return;
        }
    }

    // Check diagonals
    if (grid[0][0] == 'O' && grid[1][1] == 'O' && grid[2][2] == ' ') {
        grid[2][2] = 'O';
        return;
    }
    if (grid[0][0] == ' ' && grid[1][1] == 'O' && grid[2][2] == 'O') {
        grid[0][0] = 'O';
        return;
    }
    if (grid[0][0] == 'O' && grid[1][1] == ' ' && grid[2][2] == 'O') {
        grid[1][1] = 'O';
        return;
    }
    if (grid[0][2] == 'O' && grid[1][1] == 'O' && grid[2][0] == ' ') {
        grid[2][0] = 'O';
        return;
    }
    if (grid[0][2] == ' ' && grid[1][1] == 'O' && grid[2][0] == 'O') {
        grid[0][2] = 'O';
        return;
    }
    if (grid[0][2] == 'O' && grid[1][1] == ' ' && grid[2][0] == 'O') {
        grid[1][1] = 'O';
        return;
    }

    // Random move if no winning/blocking move is found
    ai_move(grid);
}

// Main function
int main() {
    initscr();            // Initialize screen
    noecho();             // Disable automatic echoing of input
    curs_set(FALSE);      // Hide the cursor
    keypad(stdscr, TRUE); // Enable arrow keys

    char player_x_name[50];   // Name for Player X
    char player_o_name[50];   // Name for Player O (or AI)
    int player_x_score = 0;   // Score for Player X
    int player_o_score = 0;   // Score for Player O
    int play_again = 1;       // Flag to control game loop
    int first_player = 0;     // 0 for X, 1 for O
    int ai_level = 0;         // AI difficulty level
    int game_mode = start_menu(&ai_level); // Choose game mode and AI level

    // Set player names based on game mode
    if (game_mode == 1) {
        get_player_names(player_x_name, player_o_name); // Get names for two players
    } else {
        strcpy(player_x_name, "Player");
        strcpy(player_o_name, "AI");
    }

    while (play_again) {
        // Initialize grid
        char grid[GRID_SIZE][GRID_SIZE] = {
            {' ', ' ', ' '},
            {' ', ' ', ' '},
            {' ', ' ', ' '}
        };

        int row = 0, col = 0;          // Starting position (top-left cell)
        char current_player = first_player == 0 ? 'X' : 'O'; // Set first player
        char winner = ' ';
        int draw = 0;
        int y_offset = 2;              // Offset to position the grid lower on the screen

        while (1) {
            clear(); // Clear the screen

            // Display scores and current player
            mvprintw(0, 0, "%s (X): %d | %s (O): %d", player_x_name, player_x_score, player_o_name, player_o_score);
            mvprintw(1, 0, "Current Player: %s (%c)", current_player == 'X' ? player_x_name : player_o_name, current_player);

            draw_grid();      // Draw the grid
            print_grid(grid); // Print the current state of the grid

            if (current_player == 'X') {
                // Highlight the current cell
                int y = row * 2 + 1 + y_offset;
                int x = col * 4 + 2;
                mvprintw(y, x, "[%c]", grid[row][col] == ' ' ? ' ' : grid[row][col]);

                refresh();        // Update the screen
                int key = getch(); // Get user input

                if (key == 'q') { // Exit game
                    play_again = 0;
                    break;
                }

                // Handle key input for movement or placing a mark
                switch (key) {
                    case KEY_UP:
                        if (row > 0) row--;
                        break;
                    case KEY_DOWN:
                        if (row < 2) row++;
                        break;
                    case KEY_LEFT:
                        if (col > 0) col--;
                        break;
                    case KEY_RIGHT:
                        if (col < 2) col++;
                        break;
                    case ' ': // Place a mark
                        if (grid[row][col] == ' ') {
                            grid[row][col] = 'X';
                            current_player = 'O'; // Switch to Player O or AI
                        }
                        break;
                }
            } else {
                // Player O or AI's turn
                if (game_mode == 2) {
                    mvprintw(10, 5, "AI is making its move...");
                    refresh(); // Update screen to show AI's decision
                    sleep(1);  // Pause for a moment to simulate AI thinking
                    if (ai_level == 1) {
                        ai_move(grid); // Easy AI
                    } else {
                        hard_ai_move(grid); // Hard AI
                    }
                } else {
                    // Highlight the current cell for Player O
                    int y = row * 2 + 1 + y_offset;
                    int x = col * 4 + 2;
                    mvprintw(y, x, "[%c]", grid[row][col] == ' ' ? ' ' : grid[row][col]);

                    refresh();        // Update the screen
                    int key = getch(); // Get user input

                    if (key == 'q') { // Exit game
                        play_again = 0;
                        break;
                    }

                    // Handle key input for movement or placing a mark
                    switch (key) {
                        case KEY_UP:
                            if (row > 0) row--;
                            break;
                        case KEY_DOWN:
                            if (row < 2) row++;
                            break;
                        case KEY_LEFT:
                            if (col > 0) col--;
                            break;
                        case KEY_RIGHT:
                            if (col < 2) col++;
                            break;
                        case ' ': // Place a mark
                            if (grid[row][col] == ' ') {
                                grid[row][col] = 'O';
                                current_player = 'X'; // Switch to Player X
                            }
                            break;
                    }
                }
                current_player = 'X'; // After AI or Player O's turn, switch to Player X
            }

            // Check for a winner
            winner = check_winner(grid);
            if (winner != ' ') {
                if (winner == 'X') player_x_score++;
                if (winner == 'O') player_o_score++;

                clear();
                mvprintw(5, 5, "%s (%c) wins! Press any key to continue.", winner == 'X' ? player_x_name : player_o_name, winner);
                refresh();
                getch();
                break;
            }

            // Check for a draw
            draw = is_draw(grid);
            if (draw) {
                clear();
                mvprintw(5, 5, "It's a draw! Press any key to continue.");
                refresh();
                getch();
                break;
            }
        }

        // Display menu after each round
        if (play_again) {
            int choice = display_menu();
            if (choice == 'n') {
                play_again = 0; // Exit game
            } else {
                first_player = 1 - first_player; // Alternate who starts
            }
        }
    }

    endwin(); // End NCurses mode
    return 0;
}