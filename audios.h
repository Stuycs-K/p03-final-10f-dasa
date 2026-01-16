#ifndef AUDIOS.H
#define AUDIOS.H


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "node.h"
#include "library.h"
#include "server.h"
#include "client.h"
#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>


int play_song_pipev(int *write_fd);
void stream_song(int server_socket);
void send_client(int player_pid, int write_fd, char command);
void deleting_song(char * song_path);
void stop_song(int pid);

#endif