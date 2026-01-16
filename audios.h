#ifndef AUDIOS.H
#define AUDIOS.H

int play_song_pipev(int *write_fd);
void stream_song(int server_socket);
void send_client(int player_pid, int write_fd, char command);
void deleting_song(char * song_path);
void stop_song(int pid);

#endif