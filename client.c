#include "audios.h"
#include "library.h"
#include "networking.h"
#include "node.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static void sighandler(int signo) {
  if (signo == SIGINT) {
    printf("Interruption");
    exit(0);
  }
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

  int player_pid;
  int player_fd;
  player_pid = play_song_pipev(&player_fd);

  char buffs[1024];
  int bytes_read;
  while ((bytes_read = recv(server_socket, buffs, sizeof(buffs),0)) > 0) {
    int writing = 0;
    while(writing < bytes_read){
      int a = write(player_fd, buffs + writing , bytes_read - writing);
      if(a <= 0){
        strerror(errno);
        exit(1);
      }
      writing += a;
    }
  }
  close(player_fd);
  waitpid(player_pid, NULL, 0);

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

    if (commands[0] == '\n') {
      printf("No command entered. Exiting...\n");
      exit(1);
    }

    char command = commands[0];

    if (command != 'p' && command != 'q' && command != 'd' && command != 'n' &&
        command != 'u') {
      printf("Wrong command. Client is quitting....You ruined it.\n");
      exit(1);
    }

    if (command == 'p') {
      send_client(player_pid, player_fd, 'p');
    }
    if (command == 'u') {
      send_client(player_pid, player_fd, 'p');
    }
    if (command == 'q') {
      send_client(player_pid, player_fd, 'q');
    }
    if (command == 'd') {
      send_client(player_pid, player_fd, 'q');
      deleting_song(current_song);
    }
    if (command == 'n') {
      send_client(player_pid, player_fd, 'q');
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
