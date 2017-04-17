
CC = gcc
CFLAG = -g -Wall

INC += -I./
INC += -I../build_linux/include/
LIB += -L../build_linux/lib -lsqlite3

db_test: db_driver.o db_test.o 
	$(CC) db_driver.o db_test.o $(CFLAG) $(INC) $(LIB) -o db_test

db_driver.o: db_driver.c db_driver.h
	$(CC) db_driver.c $(CFLAG) $(INC) -c -o db_driver.o

db_test.o: db_test.c
	$(CC) db_test.c $(CFLAG) $(INC) -c -o db_test.o

clean:
	rm db_test

distclean:
	rm *.o db_test
	
