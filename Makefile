.SUFFIXES : .c .o

CC = gcc
CXX = g++
PROGRAM = bmp2jpeg.bin
CFLAGS = -g

LDFLAGS = -lpthread
INCLUDES = 

SRCS = main.c \
		bmp.c \
		jpegenc.c

OBJS = main.o \
		bmp.o \
		jpegenc.o

all: $(PROGRAM)

bmp2jpeg.bin: $(OBJS) $(SRCS) Makefile
	$(CC) -o $(PROGRAM) $(OBJS) $(LDFLAGS) 
 
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c ${*F}.c -o ${*F}.o

clean:
	rm -f $(PROGRAM) *.o
