CC=gcc
CFLAGS=-Wall -std=c99

ifeq ($(shell uname -s),Darwin)
	INCL=
	LIBS=-framework OpenCL
	RPTH=
else
	INCL=-I/usr/include
	LIBS=-L/usr/lib64/beignet -lcl
	RPTH=-Wl,-rpath,/usr/lib64/beignet
endif

all: find-platform find-devices add-matrices

find-platform: find-platform.c
	$(CC) $(CFLAGS) $(INCL) find-platform.c $(RPTH) $(LIBS) -o find-platform

find-devices: find-devices.c
	$(CC) $(CFLAGS) $(INCL) find-devices.c $(RPTH) $(LIBS) -o find-devices

add-matrices: add-matrices.c
	$(CC) $(CFLAGS) $(INCL) add-matrices.c $(RPTH) $(LIBS) -o add-matrices
