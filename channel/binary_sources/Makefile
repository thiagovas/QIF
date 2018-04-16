CC=g++
CC_FLAGS=-O2 -Wall -std=c++11
BIN=./bin

all: prep build


prep:
	mkdir -p $(BIN)


# Main Rules
brutao: prep brutao.o channel.o bayes.o
	$(CC) $(BIN)/brutao.o $(BIN)/channel.o $(BIN)/bayes.o -o brutao $(CC_FLAGS)

randombrutao: prep randombrutao.o channel.o bayes.o
	$(CC) $(BIN)/random_brutao.o $(BIN)/channel.o $(BIN)/bayes.o -o random_brutao $(CC_FLAGS)



# Unique compiles from this folder.
brutao.o: 
	$(CC) -c brutao.cpp -o $(BIN)/brutao.o $(CC_FLAGS)

randombrutao.o: 
	$(CC) -c random_brutao.cpp -o $(BIN)/random_brutao.o $(CC_FLAGS)



# Unique compiles from other folders.
channel.o:
	$(CC) -c ../channel.cpp -o $(BIN)/channel.o $(CC_FLAGS)

bayes.o: channel.o
	$(CC) -c ../vulnerability/bayes.cpp -o $(BIN)/bayes.o $(CC_FLAGS)


clean:
	rm -f main
	rm -f $(BIN)/main
	rm -rf $(BIN)/*.o
	rm -rf $(BIN)/*~
