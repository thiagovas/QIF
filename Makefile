CC=g++
CC_FLAGS=-O2 -Wall -std=c++11
BIN=./bin
SRC=./src

all: prep build


prep:
	mkdir -p $(BIN)


build: main.o  parser.o lexer.o
	$(CC) $(BIN)/*.o -o main $(CC_FLAGS)


main.o: parser.o lexer.o
	$(CC) -c $(SRC)/main.cpp -o $(BIN)/main.o $(CC_FLAGS)


parser.o: lexer.o
	$(CC) -c $(SRC)/parser.cpp -o $(BIN)/parser.o $(CC_FLAGS)


lexer.o:
	$(CC) -c $(SRC)/lexer.cpp -o $(BIN)/lexer.o $(CC_FLAGS)


clean:
	rm -f main
	rm -f $(BIN)/main
	rm -rf $(BIN)/*.o
	rm -rf $(BIN)/*~
