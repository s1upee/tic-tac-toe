# Tic-Tac-Toe

A terminal-based implementation of the classic Tic-Tac-Toe game written in C using the NCurses library. This project demonstrates a clean approach to game logic and user interaction in a terminal environment.

---

## Screenshots

### Game Welcome Page
![Game Welcome](pictures/Screenshot_welcome.png)

### Game Start
![Game Start](pictures/Screenshot_start.png)

### Gameplay
![Gameplay](pictures/Screenshot_gameplay.png)

### Winner Announcement
![Winner Announcement](pictures/Screenshot_winner.png)

### AI Feature
![AI Feature](pictures/Screenshot_AI.png)

### Game Over
![Game Over](pictures/Screenshot_game_over.png)

---

## Features

- **Interactive Gameplay**: Play against another player or challenge an AI opponent with selectable difficulty levels.
- **Customizable Names**: Enter player names for a personalized experience when playing in two-player mode.
- **AI Difficulty Levels**: Choose between "Easy" (random moves) and "Hard" (strategic moves) when playing against AI.
- **Score Tracking**: The game keeps track of wins for each player across multiple rounds.
- **Alternating Turns**: The first player alternates between rounds to ensure fairness.
- **Dynamic Feedback**: Clear messages and highlights make the game intuitive, including notifications for AI moves.
- **Endgame Menu**: Option to replay or exit the game after each round.

---

## How to Run

1. Clone the repository:
   ```bash
   git clone https://github.com/s1upee/tic-tac-toe.git
   cd tic-tac-toe
2. Compile the code: 
   ```bash
   gcc -o tic_tac_toe tic_tac_toe.c -lncurses
3. Run the game
   ```bash
   ./tic_tac_toe