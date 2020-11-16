CC 		 := g++
OBJECTS  := main.o functions.o Game.o Menu.o Deck.o Card.o Player.o Server.o Client.o
CFLAGS 	 := -std=c++17 -std=gnu++17 -pthread
FLAGS	 := -g -c
EXE 	 := uno.exe
INCLUDES := -ISFML/include
LIBS	 := -LSFML/sfml-widgets-master/lib -lsfml-widgets -LSFML/lib -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -lGL 

uno.exe: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o $(EXE) 
	export LD_LIBRARY_PATH=./SFML/lib
	rm *.o

main.o: main.cpp uno.h
	$(CC) $(INCLUDES) $(FLAGS) main.cpp

%.o: classes/%.cpp classes/%.h
	$(CC) $(INCLUDES) $(FLAGS) $< -o $@

clean:
	rm *.o *.exe

run:
	./$(EXE)
