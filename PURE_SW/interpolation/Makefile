# Source, Executable, Includes, Library Defines

INCL   = 
SRC    = interp.c
OBJ    = $(SRC:.c=.o)
EXE=   interp.exe

# Compiler, Linker Defines
CC = arm-linux-gnueabihf-gcc
CFLAGS=  -Wall -O0 -DC
#LIBPATH = -L.
LDFLAGS = -o $(EXE) $(LIBPATH) $(LIBS)
CFDEBUG = -DC  -Wall -g -DDEBUG $(LDFLAGS)
RM      = /bin/rm -f

time: CFLAGS += -DEX_TIME

default: all

# Compile and Assemble C Source Files into Object Files
$(OBJ):  
	$(CC) -c -Wall $(CFLAGS) $(INCL)  $*.c

# Link all Object Files with external Libraries into Binaries
all:	$(OBJ)
	$(CC) $(LDFLAGS) $(OBJ)
 

time:	$(OBJ)
	$(CC) $(LDFLAGS) $(OBJ)

# Create a gdb/dbx Capable Executable with DEBUG flags turned on
debug:
	 $(CC) $(CFDEBUG) $(SRC)


# Clean Up Objects, Exectuables, Dumps out of source directory
clean:
	 $(RM) $(OBJ) $(EXE) 
