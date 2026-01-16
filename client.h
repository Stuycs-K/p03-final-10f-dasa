#ifndef CLIENT_H
#define CLIENT_H

static void sighandler(int signo);
void client_song(int server_socket);
int clientLogic(int server_socket) ;
#endif