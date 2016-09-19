CC 		= g++
CFLAGS 	= -g -Wall -ansi

MDEP 	= func.o dct.o MEMC.o rlc.o
DEP 	= $(patsubst %.o, %.h, $(MDEP)) def.h
OBJ		= $(patsubst %.h, %.o, $(DEP)) main.o
TARGET 	= main


$(TARGET):$(MDEP) main.o
	$(CC) -o main $(MDEP) main.o
main.o:main.cpp $(DEP)
	$(CC) $(CFLAGS) -c main.cpp
dct.o:dct.cpp $(DEP)
	$(CC) $(CFLAGS) -c dct.cpp
MEMC.o:MEMC.cpp $(DEP)
	$(CC) $(CFLAGS) -c MEMC.cpp
rlc.o:rlc.cpp $(DEP)
	$(CC) $(CFLAGS) -c rlc.cpp
func.o:func.cpp $(DEP)
	$(CC) $(CFLAGS) -c func.cpp

clean:
	rm -f main
	rm -f $(OBJ)
