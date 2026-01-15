#makefile

.PHONY: clean compile

server: server.o networking.o node.o library.o
	@gcc -o server server.o networking.o node.o library.o -lm
	@./server
client: client.o networking.o node.o library.o audios.o
	@gcc -o client client.o networking.o node.o library.o audios.o -lm
	@./client
compile: client.o server.o networking.o node.o library.o audios.o
	@gcc -o client client.o networking.o node.o library.o audios.o -lm
	@gcc -o server server.o networking.o node.o library.o -lm
server.o: server.c server.h networking.h node.h library.h
	@gcc -c server.c
client.o: client.c client.h networking.h node.h library.h
	@gcc -c client.c
networking.o: networking.c networking.h
	@gcc -c networking.c
library.o: library.c library.h node.h
	@gcc -c library.c
node.o: node.c node.h
	@gcc -c node.c
audios.o: audios.c audios.h
	@gcc -c audios.c
clean:
	@rm -f *.o
