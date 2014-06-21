CC = gcc -Wall
OBJS = main.o getch.o 
EXEC = cwc

$(EXEC): $(OBJS) 
	$(CC) -o $(EXEC) $(OBJS)
.c.o:
	$(CC) -c $<
clean:
	rm $(EXEC) *.o
