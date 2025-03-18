CC=gcc
CFLAGS=-Wall -pthread
SERVER_OBJS=TCP_server.o course.o student.o parser.o
CLIENT_OBJS=TCP_client.o

all: server client

server: $(SERVER_OBJS)
	$(CC) $(CFLAGS) -o server $(SERVER_OBJS)

TCP_server.o: TCP_server.c course.h student.h parser.h
	$(CC) $(CFLAGS) -c TCP_server.c

course.o: course.c course.h student.h
	$(CC) $(CFLAGS) -c course.c

student.o: student.c student.h
	$(CC) $(CFLAGS) -c student.c

parser.o: parser.c parser.h student.h course.h
	$(CC) $(CFLAGS) -c parser.c

client: $(CLIENT_OBJS)
	$(CC) $(CFLAGS) -o client $(CLIENT_OBJS)

TCP_client.o: TCP_client.c
	$(CC) $(CFLAGS) -c TCP_client.c

clean:
	rm -f server client *.o output.txt
