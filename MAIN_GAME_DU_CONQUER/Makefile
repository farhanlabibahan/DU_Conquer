# === Configuration ===
CXX = g++
SRC = main.cpp global.cpp menu.cpp map2.cpp eee.cpp lights_on.cpp loading.cpp nuclear.cpp minesweeper.cpp genetics.cpp game_of_life.cpp microbiology.cpp snake.cpp japanesse.cpp kanji.cpp eee_first.cpp signal.cpp character_dept.cpp vc_bunglaw.cpp shooting.cpp lift.cpp
OUT = game

UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
    CXXFLAGS = -std=c++17 -I/opt/homebrew/include
    LDFLAGS = -L/opt/homebrew/lib -lraylib \
              -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo
else
    CXXFLAGS =
    LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

# === Rules ===
all:
	$(CXX) $(SRC) -o $(OUT) $(CXXFLAGS) $(LDFLAGS) && ./$(OUT)

clean:
	rm -f $(OUT)
