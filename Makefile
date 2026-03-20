CC = g++
CFLAGS = -std=c++17 -Wall -Wextra
LDFLAGS = -lSDL2 -lSDL2_ttf -lSDL2_image

TARGET = alamo
SRC = main.cpp engine/Engine.cpp engine/Layout.cpp

all:
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(TARGET)