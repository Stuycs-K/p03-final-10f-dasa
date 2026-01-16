#ifndef SERVER_H
#define SERVER_H

static void sighandler(int signo);
void subserver_logic(int client_socket, struct song_node **library);

#endif
