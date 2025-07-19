==============================
      DU CONQUER - README
==============================

DU Conquer is a campus-themed exploration game packed with fun mini-games, cinematic intro/outro, and diverse challenges. It is built in C++ using the Raylib game development library and runs on Windows, Linux, and macOS.

--------------------------------------
 REQUIREMENTS
--------------------------------------

 
Windows:
- MinGW or similar with g++
- Raylib built for Windows

Linux (Debian/Ubuntu-based):
- g++
- raylib development libraries

To install Raylib and dependencies:

bash :
sudo apt install g++ libraylib-dev libgl1-mesa-dev libglu1-mesa-dev

macOS:
- Homebrew
- Raylib

Install with:

bash
brew install raylib

--------------------------------------
  BUILD AND RUN COMMANDS
--------------------------------------

WINDOWS:

Open terminal in the project directory:

g++ main.cpp global.cpp menu.cpp map2.cpp eee.cpp lights_on.cpp loading.cpp nuclear.cpp minesweeper.cpp genetics.cpp game_of_life.cpp microbiology.cpp snake.cpp japanesse.cpp kanji.cpp eee_first.cpp signal.cpp character_dept.cpp vc_bunglaw.cpp shooting.cpp lift.cpp character_map.cpp cctv.cpp puzzle.cpp maze.cpp iba.cpp business.cpp bank.cpp obstacle.cpp ducsu.cpp memory_match.cpp law.cpp gear.cpp robot.cpp robotics.cpp cse.cpp debug.cpp philosophy.cpp tictactoe.cpp intro.cpp outro.cpp -o game.exe -lraylib -lopengl32 -lgdi32 -lwinmm -static-libgcc -static-libstdc++ && ./game.exe


LINUX:

g++ main.cpp global.cpp menu.cpp map2.cpp eee.cpp lights_on.cpp loading.cpp nuclear.cpp minesweeper.cpp genetics.cpp game_of_life.cpp microbiology.cpp snake.cpp japanesse.cpp kanji.cpp eee_first.cpp signal.cpp character_dept.cpp vc_bunglaw.cpp shooting.cpp lift.cpp character_map.cpp cctv.cpp puzzle.cpp maze.cpp iba.cpp business.cpp bank.cpp obstacle.cpp ducsu.cpp memory_match.cpp law.cpp gear.cpp robot.cpp robotics.cpp cse.cpp debug.cpp philosophy.cpp tictactoe.cpp intro.cpp outro.cpp -o game -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 && ./game


 macOS:
 
g++ main.cpp global.cpp menu.cpp map2.cpp eee.cpp lights_on.cpp loading.cpp nuclear.cpp minesweeper.cpp genetics.cpp game_of_life.cpp microbiology.cpp snake.cpp japanesse.cpp kanji.cpp eee_first.cpp signal.cpp character_dept.cpp vc_bunglaw.cpp shooting.cpp lift.cpp character_map.cpp cctv.cpp puzzle.cpp maze.cpp iba.cpp business.cpp bank.cpp obstacle.cpp ducsu.cpp memory_match.cpp law.cpp gear.cpp robot.cpp robotics.cpp cse.cpp debug.cpp philosophy.cpp tictactoe.cpp outro.cpp intro.cpp -o game -std=c++17 -I/opt/homebrew/include -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo && ./game

