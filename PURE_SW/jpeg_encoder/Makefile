# Source, Executable, Includes, Library Defines

INCL   = 
SRC    = jpeg_encoder.c
OBJ    = $(SRC:.c=.o)
EXE=   jpeg_encoder.exe

# Compiler, Linker Defines
CC = arm-linux-gnueabihf-gcc
CFLAGS=  -Wall -O0 -DC
#LIBPATH = -L.
LDFLAGS = -o $(EXE) $(LIBPATH) $(LIBS)
CFDEBUG = -DC  -Wall -g -DDEBUG $(LDFLAGS)
RM      = /bin/rm -f

io: CFLAGS += -DIODATA
hw_image: CFLAGS += -DCREATEJPEG
io_hw: CFLAGS += -DCREATEJPEG -DIODATA


default: all

# Compile and Assemble C Source Files into Object Files
$(OBJ):  
	$(CC) -c -Wall $(CFLAGS) $(INCL)  $*.c



# Link all Object Files with external Libraries into Binaries
all:	$(OBJ)
	$(CC) $(LDFLAGS) $(OBJ)
 

io:	all
hw_image: all
io_hw: all

# Create a gdb/dbx Capable Executable with DEBUG flags turned on
debug:
	 $(CC) $(CFDEBUG) $(SRC)


# Clean Up Objects, Exectuables, Dumps out of source directory
clean:
	 $(RM) $(OBJ) $(EXE) 
