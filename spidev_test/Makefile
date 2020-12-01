CXX		  := gcc
#CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb -pthread
#CXX_FLAGS2 := -lrt -lgpiod -lpigpio

BIN		:= .#bin
SRC		:= .#src
INCLUDE	:= include
LIB		:= lib

LIBRARIES	:=
EXECUTABLE	:= bin


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/spidev_test_simple.c
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES) $(CXX_FLAGS2)

clean:
#Sungsu-uncomment below when needed..
#	-rm $(BIN)/*
	rm -rf $(BIN)/bin
