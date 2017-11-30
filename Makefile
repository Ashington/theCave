
# code details

EXE_DIR = .
EXE = $(EXE_DIR)/theCave

SRC= main.c makeMap.c theCave.c
# generic build details

CC =     gcc
CFLAGS = -Wall -O -std=c99
LIBS =   -lSDL2 -lSDL2_image

# compile to  object code

OBJ= $(SRC:.c=.o)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

# build executable

$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $(EXE) $(LIBS)

# clean up compilation

clean:
	rm -f $(OBJ) $(EXE)

# dependencies

main.o:  main.c controlFunctions.h
theCave.o:  theCave.c gameFunctions.h gameData.h
ai.o: ai.c ai.h
