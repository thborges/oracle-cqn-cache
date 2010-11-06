CC = gcc

CFLAGS=-g `pkg-config glib-2.0 --cflags`

LIBS=`pkg-config glib-2.0 --libs` \
     -Linstantclient_11_2 \
     -locilib \
     -lclntsh \
     -lnnz11 \
     -lpthread \
     -lsqlite3 

DEFINES=-DOCI_IMPORT_LINKAGE -DOCI_CHARSET_ANSI

OBJECTS=config.h \
	sagrcache.o \
	sqlitedb.o \
	oracledb.o \
	oraclecqn.o \
	networkthread.o

all: sagrcache

sagrcache: $(OBJECTS)
	$(CC) $(CFLAGS) -o sagrcache $(OBJECTS) $(INCLUDES) $(LIBS)

#%.o : %.c
#	gcc -Wall -c $< $(INCLUDES) $(DEFINES)

clean:
	rm -f *.o
	rm -f sagrcache
	rm -f cliente

cli:
	gcc -DCLIENTE -g -o cliente cliente.c

