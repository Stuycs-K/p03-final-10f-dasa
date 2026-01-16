#ifndef SERVER_H
#define SERVER_H

static void sighandler(int signo);
void send_song(int client_socket, char * filepath);
void subserver_logic(int client_socket, struct song_node **library);


#endif
