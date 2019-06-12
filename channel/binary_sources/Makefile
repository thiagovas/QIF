CC=g++
CC_FLAGS=-O2 -Wall -std=c++14
BIN=./bin

all: prep build


prep:
	mkdir -p $(BIN)


# Main Rules
brutao: prep brutao.o channel.o bayes.o vulnerability.o
	$(CC) $(BIN)/brutao.o $(BIN)/channel.o $(BIN)/bayes.o $(BIN)/vulnerability.o -o brutao $(CC_FLAGS)

random_brutao: prep randombrutao.o channel.o bayes.o
	$(CC) $(BIN)/random_brutao.o $(BIN)/channel.o $(BIN)/bayes.o $(BIN)/vulnerability.o -o random_brutao $(CC_FLAGS)

brutao_memefficient: prep brutao_memefficient.o channel.o bayes.o vulnerability.o
	$(CC) $(BIN)/brutao_memefficient.o $(BIN)/channel.o $(BIN)/bayes.o $(BIN)/vulnerability.o -o brutao_memefficient $(CC_FLAGS)

dining4: prep dining4.o channel.o bayes.o
	$(CC) $(BIN)/dining4.o $(BIN)/channel.o $(BIN)/bayes.o $(BIN)/vulnerability.o -o dining4 $(CC_FLAGS)

crowds9: prep crowds9.o channel.o bayes.o
	$(CC) $(BIN)/crowds9.o $(BIN)/channel.o $(BIN)/bayes.o $(BIN)/vulnerability.o -o crowds9 $(CC_FLAGS)

# Unique compiles from this folder.
brutao.o: 
	$(CC) -c brutao.cpp -o $(BIN)/brutao.o $(CC_FLAGS)

randombrutao.o: 
	$(CC) -c random_brutao.cpp -o $(BIN)/random_brutao.o $(CC_FLAGS)

brutao_memefficient.o: 
	$(CC) -c brutao_memefficient.cpp -o $(BIN)/brutao_memefficient.o $(CC_FLAGS)

dining4.o: 
	$(CC) -c dining4.cpp -o $(BIN)/dining4.o $(CC_FLAGS)

crowds9.o:
	$(CC)	-c	crowds9.cpp	-o	$(BIN)/crowds9.o	$(CC_FLAGS)


# Unique compiles from other folders.
channel.o:
	$(CC) -c ../channel.cpp -o $(BIN)/channel.o $(CC_FLAGS)

bayes.o: channel.o vulnerability.o
	$(CC) -c ../vulnerability/bayes.cpp -o $(BIN)/bayes.o $(CC_FLAGS)

vulnerability.o:
	$(CC) -c ../vulnerability/vulnerability.cpp -o $(BIN)/vulnerability.o $(CC_FLAGS)



clean:
	rm -f main
	rm -f $(BIN)/main
	rm -rf $(BIN)/*.o
	rm -rf $(BIN)/*~
