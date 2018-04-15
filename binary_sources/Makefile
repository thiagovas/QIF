CC=g++
CC_FLAGS=-O2 -Wall -std=c++11
BIN=./bin

all: prep build


prep:
	mkdir -p $(BIN)


# Main Rules
main: prep parser.o lexer.o main.o
	$(CC) $(BIN)/parser.o $(BIN)/lexer.o $(BIN)/main.o -o main $(CC_FLAGS)

channel_tst: prep channel.o channel_tst.o
	$(CC) $(BIN)/channel.o $(BIN)/channel_tst.o -o channel_tst $(CC_FLAGS)
	


# Unique compiles from this folder.
channel_tst.o:
	$(CC) -c channel_tst.cpp -o $(BIN)/channel_tst.o $(CC_FLAGS)

main.o:
	$(CC) -c main.cpp -o $(BIN)/main.o $(CC_FLAGS)



# Unique compiles from other folders.
channel.o:
	$(CC) -c ../channel/channel.cpp -o $(BIN)/channel.o $(CC_FLAGS)

parser.o: lexer.o
	$(CC) -c $(SRC)/parser.cpp -o $(BIN)/parser.o $(CC_FLAGS)

lexer.o:
	$(CC) -c $(SRC)/lexer.cpp -o $(BIN)/lexer.o $(CC_FLAGS)


clean:
	rm -f main
	rm -f $(BIN)/main
	rm -rf $(BIN)/*.o
	rm -rf $(BIN)/*~
