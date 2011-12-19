serv:position_1.o position.o index.o server.o order.o position_o.o
	cc  index.o position_1.o position.o server.o order.o position_o.o -LS -o serv -lpthread -lm
order.o:order.c wiLoc.h
	cc -c order.c
position_o.o:position_o.c wiLoc.h
	cc -c position_o.c
position_1.o:position_1.c wiLoc.h
	cc -c position_1.c
position.o:position.c wiLoc.h
	cc -c position.c
index.o:index.c wiLoc.h
	cc -c index.c
server.o:server.c wiLoc.h
	cc -c server.c
clean:
	rm *.o
	rm serv
