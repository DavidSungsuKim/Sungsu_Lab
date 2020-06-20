CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb -pthread
CXX_FLAGS2 := -lrt

BIN		:= bin
SRC		:= src
INCLUDE	:= include
LIB		:= lib

LIBRARIES	:=
EXECUTABLE	:= bin


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES) $(CXX_FLAGS2)


exe: #Execute the program
	./$(BIN)/$(EXECUTABLE)
clean:
#Sungsu-uncomment below when needed..
#	-rm $(BIN)/*
