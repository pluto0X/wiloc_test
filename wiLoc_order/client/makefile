CC=gcc
CFLAG=-g -Wall
.o:.c
	${CC} -c ${CFLAG}  $< -o $@
main:wifi_status.o main.o report.o
	${CC} ${CFLAG} $^ -o main
clean:
	rm -f *.o main
