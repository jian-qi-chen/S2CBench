# Source, Executable, Includes, Library Defines

INCL   = 
SRC    = fir.c
OBJ    = $(SRC:.c=.o)
EXE=   fir.exe

# Compiler, Linker Defines
CC = arm-linux-gnueabihf-gcc
CFLAGS=  -Wall -O0 -DC
#LIBPATH = -L.
LDFLAGS = -o $(EXE) $(LIBPATH) $(LIBS)
CFDEBUG = -DC  -Wall -g -DDEBUG $(LDFLAGS)
RM      = /bin/rm -f


default: all

# Compile and Assemble C Source Files into Object Files
$(OBJ):  
	$(CC) -c -Wall $(CFLAGS) $(INCL)  $*.c



# Link all Object Files with external Libraries into Binaries
all:	$(OBJ)
	$(CC) $(LDFLAGS) $(OBJ)
 


# Create a gdb/dbx Capable Executable with DEBUG flags turned on
debug:
	 $(CC) $(CFDEBUG) $(SRC)


# Clean Up Objects, Exectuables, Dumps out of source directory
clean:
	 $(RM) $(OBJ) $(EXE) 