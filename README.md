# Floppy Ball

**Floppy Ball** is a simplified clone of the classic Flappy Bird game developed in C++ using the [raylib](https://www.raylib.com) library. The project focuses on implementing core gameplay mechanics with an emphasis on clean, modular code and smooth performance.

## Features

- **Classic Gameplay:** Enjoy a Flappy Bird-inspired experience with simple controls and engaging mechanics.
- **Physics & Collision:** Implements basic gravity, collision detection, and response to ensure realistic movement and game over conditions.
- **Object-Oriented Design:** Clean separation of game components (e.g., player character, obstacles) to facilitate easy modifications and potential feature expansion.
- **Optimized Rendering:** Uses raylib for efficient 2D graphics rendering, ensuring a responsive and smooth gameplay experience.

## Requirements

- **C++ Compiler:** Ensure you have a C++17 compatible compiler installed.
- **raylib:** The game relies on raylib for graphics and input. Follow the [raylib installation guide](https://www.raylib.com/learn.html) for your platform.

## Build Instructions

1. **Clone the Repository:**

   ```bash
   git clone https://github.com/AishaSari/Floppy-Ball.git
   cd Floppy-Ball
   ```

2. **Compile the Game:**

   Assuming you have raylib installed and set up correctly, compile using g++ (adjust flags as needed for your system):

   ```bash
   g++ -std=c++17 -o FloppyBall main.cpp -lraylib -lopengl -lm -lpthread -ldl -lrt -lX11
   ```

3. **Run the Game:**

   ```bash
   ./FloppyBall
   ```

## Controls

- **Spacebar / Mouse Click:** Make the ball flap to gain altitude.
- **Avoid Obstacles:** Navigate through gaps in obstacles to score points.
- **Restart:** On game over, press the designated key to restart (check in-code instructions).

## Code Structure

- **main.cpp:** Contains the main game loop, handling initialization, input processing, game logic, and rendering.
- **Game Components:** Organized into functions/classes (if applicable) for handling the player character, obstacles, and scoring logic. (Review the code comments for detailed insights on each module.)

## Contributing

Contributions and suggestions are welcome! Please fork the repository and submit a pull request for any improvements or bug fixes.
