#include "audios.h"
#include "networking.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>

static void sighandler(int signo) {
  if (signo == SIGINT) {
    printf("Interruption");
    exit(0);
  }
}

void client_song(int server_socket){
  int filesize;
  recv(server_socket, &filesize, sizeof(filesize), 0);

  int write_fd;
  int pid = play_song_pipev(&write_fd);

  char buffs[4096];
  int total_recieved = 0;
  while(total_recieved < filesize){
    int to_read = sizeof(buffs);
    if(filesize - total_recieved < sizeof(buffs)){
      to_read = filesize - total_recieved;
    }
    int r = recv(server_socket, buffs, to_read, 0);
    if(r <= 0){
      strerror(errno);
      close(write_fd);
      waitpid(pid,NULL,0);
      exit(1);
    }

    write(write_fd,buffs,r);
    total_recieved+=r;
  }
  close(write_fd);
  waitpid(pid,NULL,0);
}


void clientLogic(int server_socket) {
  char current_song[256];
  char buff[256];

  printf("Please input the first letter of the first name of the artist: ");
  if (fgets(buff, sizeof(buff), stdin) == NULL) {
    printf("Client done.\n");
    close(server_socket);
    return;
  }

  if (buff[0] == '\n' || buff[0] == '\0') {
    printf("No input provided.\n");
    close(server_socket);
    return;
  }

  char letter = buff[0];
  if (send(server_socket, &letter, 1, 0) <= 0) {
    printf("Failed to send letter.\n");
    close(server_socket);
    return;
  }

  int count;
  int bytes = recv(server_socket, &count, sizeof(int), 0);
  char **songs = malloc(sizeof(char *) * count);
  printf("Songs for letter %c: \n", letter);
  for (int x = 0; x < count; x++) {
    songs[x] = malloc(256);
    int bytes_received = 0;
    while (bytes_received < 256) {
      int r = recv(server_socket, songs[x] + bytes_received,
                   256 - bytes_received, 0);
      if (r <= 0) {
        printf("Connection closed.\n");
        exit(1);
      }
      bytes_received += r;
    }

    printf("%s\n", songs[x]);
  }

  printf("Enter artist: ");
  fgets(buff, sizeof(buff), stdin);
  if (buff[strlen(buff) - 1] == '\n')
    buff[strlen(buff) - 1] = '\0';
  send(server_socket, buff, 256, 0);

  recv(server_socket, &count, sizeof(int), 0);
  songs = malloc(count * sizeof(char *));
  for (int x = 0; x < count; x++) {
    songs[x] = malloc(256);
    recv(server_socket, songs[x], 256, 0);
    printf("%d: %s\n", x + 1, songs[x]);
  }

  printf("Choose song: ");
  fgets(buff, sizeof(buff), stdin);
  if (buff[strlen(buff) - 1] == '\n') {
    buff[strlen(buff) - 1] = '\0';
  }
  send(server_socket, buff, sizeof(buff), 0);
  strcpy(current_song, buff);

  client_song(server_socket);
  //stream_song(server_socket);
  char commands[2];
  while (1) {
    printf("Enjoy the song. If you would like to pause press p; unpause press "
           "u; exit press q; delete song press d; go to the next song press n. "
           "Command: \n");
    char wants[256];

    if (fgets(commands, sizeof(commands), stdin) == NULL) {
      printf("No command entered. Exiting...\n");
      exit(1);
    }
  }
}

int main(int argc, char *argv[]) {
  char *IP = "127.0.0.1";
  if (argc > 1) {
    IP = argv[1];
  }
  signal(SIGINT, sighandler);
  int server_socket = client_tcp_handshake(IP);
  printf("client connected.\n");
  clientLogic(server_socket);
}
