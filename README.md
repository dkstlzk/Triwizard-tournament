#  The Triwizard Tournament — A C Game Trilogy (Raylib)

Welcome to The Triwizard Tournament, a fun set of 3 mini-games inspired by the magical world of Harry Potter.

This project is made using C and  basics of Raylib graphics library. The games are played one after another.

---

##  The 3 Challenges

###  1. Get the Dragon’s Egg
In this game, the player must steal a golden egg from a dragon.
The dragon throws fireballs, and the player has only 3 lives — each hit takes one life.
There are 5 locked gates that can only be opened by collecting keys scattered across the area.

To win:
 -Avoid fireballs
 -Collect all 5 keys
 -Reach the golden egg safely

---

###  2. Snitch Escape
In this game, the player moves around a maze to collect all the golden pellets scattered across the map.
But there’s a catch — two Death Eaters are roaming the maze, trying to catch you!

To win:
 -Collect all the pellets
 -Avoid getting caught by the ghosts
 -If a ghost touches you, it's game over.

---

###  3. The Enchanted Maze
In this final game, you’re dropped into a magical maze where the walls keep moving!
Even the exit keeps changing, so you have to stay alert.
Your goal is to find your way to the exit before time runs out.


---

##  Controls

The game will display the controls at the start, but here's a quick rundown:
-
- Arrow Keys – Move around  
- Enter – Confirm, continue. 
- ESC – Quit the game anytime  

The controls are consistent across all three mini-games, ensuring a smooth gameplay experience.

---

##  Game Flow

- Start from the main menu, where you can view the instructions.
- Press ENTER to begin the first challenge.
- Play all three games in sequence, win or lose.
- After completing all tasks:
  - If you win all three, you win the Triwizard Cup.
  - If you lose any game, you lose the tournament.
- At the end, you’ll have the option to replay the tournament or exit.

---

##  Tech & Tools

- Language: C  
- *Library:  [Raylib](https://www.raylib.com/) (Basic Implementation) 
- *Platform: Cross-platform (Windows, Linux)  
- *Build Tool: GCC

---

##  How to Run
Firstly install Raylib and then use this to compile:

bash
gcc -o game.exe main.c dragons_egg.c snitch_escape.c enchanted_maze.c -lraylib -lGL -lm -lpthread -ldl -lrt
./game.exe


---

##  Why This?

This project is a fun way to learn game development by working with basic game mechanics, loops, and controls in C. Plus, it's inspired by the magical world of Harry Potter, making it even more exciting. It’s a simple yet challenging way to test your skills!

---
