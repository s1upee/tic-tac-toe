#include <ncurses.h>
#include <string.h>

// Grid size
#define GRID_SIZE 3

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

// Main function
int main() {
    initscr();            // Initialize screen
    noecho();             // Disable automatic echoing of input
    curs_set(FALSE);      // Hide the cursor
    keypad(stdscr, TRUE); // Enable arrow keys

    char player_x_name[50];
    char player_o_name[50];
    int player_x_score = 0; // Player X's score
    int player_o_score = 0; // Player O's score
    int play_again = 1;     // Flag to control game loop
    int first_player = 0;   // 0 for X, 1 for O

    get_player_names(player_x_name, player_o_name); // Get player names

    while (play_again) {
        char grid[GRID_SIZE][GRID_SIZE] = { // Game grid
            {' ', ' ', ' '},
            {' ', ' ', ' '},
            {' ', ' ', ' '}
        };
        int row = 0, col = 0; // Starting position (top-left cell)
        char current_player = first_player == 0 ? 'X' : 'O';
        char winner = ' ';
        int draw = 0;
        int y_offset = 2; // Offset to position the grid lower on the screen

        while (1) {
            clear(); // Clear the screen

            // Display player scores and current player
            mvprintw(0, 0, "%s (X): %d | %s (O): %d", player_x_name, player_x_score, player_o_name, player_o_score);
            mvprintw(1, 0, "Current Player: %s (%c)", current_player == 'X' ? player_x_name : player_o_name, current_player);

            draw_grid();      // Draw the grid
            print_grid(grid); // Print the current state of the grid

            // Highlight the current cell
            int y = row * 2 + 1 + y_offset;
            int x = col * 4 + 2;
            mvprintw(y, x, "[%c]", grid[row][col] == ' ' ? ' ' : grid[row][col]);

            refresh();        // Update the screen
            int key = getch(); // Get user input

            if (key == 'q') { // Exit the game
                play_again = 0;
                break;
            }

            // Move the cursor
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
                    if (grid[row][col] == ' ') { // Only place if cell is empty
                        grid[row][col] = current_player;
                        current_player = (current_player == 'X') ? 'O' : 'X'; // Switch player
                    }
                    break;
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

        if (play_again) {
            int choice = display_menu();
            if (choice == 'n') {
                play_again = 0;
            } else {
                first_player = 1 - first_player; // Alternate who starts
            }
        }
    }

    endwin(); // End NCurses mode
    return 0;
}