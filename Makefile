CC = g++
CFLAGS = -Wall -Iinclude

SRC = src/main.cpp src/auth/auth.cpp src/encryption/crypto.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = secure_transfer

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

