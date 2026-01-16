#include "audios.h"
#include "networking.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

static void sighandler(int signo) {
  if (signo == SIGINT) {
    printf("Interruption");
    exit(0);
  }
}

void client_song(int server_socket) {
    pid_t child = fork();

    if (child == 0) {
        stream_song(server_socket);
        exit(0);
    }

    while (1) {
        char commands[8];
        printf("Commands: p=pause, u=unpause, q=quit\n");

        if (fgets(commands, sizeof(commands), stdin) == NULL) {
            kill(child, SIGTERM);
            waitpid(child, NULL, 0);
            break;
        }

        char cmd = commands[0];

        if (cmd == 'p') {
            kill(child, SIGSTOP);
        } else if (cmd == 'u') {
            kill(child, SIGCONT);
        } else if (cmd == 'q') {
            char stop_signal = 1;
            send(server_socket, &stop_signal, 1, 0);

            kill(child, SIGTERM);
            waitpid(child, NULL, 0);
            exit(0);
            break;
        }
    }
}

int clientLogic(int server_socket) {
  char current_song[256];
  char buff[256];
  while (1) {

    printf("Please input the first letter of the first name of the artist: ");
    if (fgets(buff, sizeof(buff), stdin) == NULL) {
      printf("Client done.\n");
      close(server_socket);
      return -1;
    }

    if (buff[0] == '\n' || buff[0] == '\0') {
      printf("No input provided.\n");
      close(server_socket);
      return -1;
    }

    char letter = buff[0];
    if (send(server_socket, &letter, 1, 0) <= 0) {
      printf("Failed to send letter.\n");
      close(server_socket);
      return -1;
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
          close(server_socket);
          return -1;
        }
        bytes_received += r;
      }

      printf("%s\n", songs[x]);
    }

    printf("Enter artist: ");
    fgets(buff, sizeof(buff), stdin);
    if (buff[strlen(buff) - 1] == '\n') {
      buff[strlen(buff) - 1] = '\0';
    }
    send(server_socket, buff, 256, 0);

    recv(server_socket, &count, sizeof(int), 0);
    if (count <= 0) {
      printf("Invalid input");
      close(server_socket);
      return -1;
    }
    songs = malloc(count * sizeof(char *));
    for (int x = 0; x < count; x++) {
      songs[x] = malloc(256);
      recv(server_socket, songs[x], 256, 0);
      printf("%d: %s\n", x + 1, songs[x]);
    }

    printf("Choose song: ");
    fgets(buff, sizeof(buff), stdin);
    buff[strcspn(buff, "\n")] = 0;
    send(server_socket, buff, 256, 0);
    client_song(server_socket);

    printf("Playback finished.\n");
  }
return 0;
}

int main(int argc, char *argv[]) {
  char *IP = "127.0.0.1";
  if (argc > 1) {
    IP = argv[1];
  }
  signal(SIGINT, sighandler);
  int server_socket = client_tcp_handshake(IP);
  printf("client connected.\n");
  while (1) {
    int result = clientLogic(server_socket);
    if (result == -1) {
      break;
    }
  }
}
