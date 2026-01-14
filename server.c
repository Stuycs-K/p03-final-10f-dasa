#include "networking.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "node.h"
#include "library.h"



static void sighandler(int signo) {
  if (signo == SIGINT) {
    //remove(WKP);
    printf("Interruption");
    exit(0);
  }
}


void subserver_logic(int client_socket, struct song_node ** library) {
  char letter;
  int sizes = recv(client_socket, &letter, sizeof(letter), 0);
  if(sizes <= 0){
    printf("Socket closed.\n");
    close(client_socket);
    exit(1);
  }
  printf("Client requested letter: %c\n", letter);
  int count;
  char ** songs = by_letter(library, letter, &count);
  send(client_socket, &count, sizeof(int), 0);
  for(int x =0; x < count; x++){
    send(client_socket, songs[x], strlen(songs[x]) + 1, 0);
  }
  free_song_array(songs, count);

  //getting artist name
  char artist[256];
  sizes = recv(client_socket, artist, sizeof(artist), 0);
  if(sizes <= 0){
    close(client_socket);
    exit(1);
  }
  songs = by_artist(library, artist, &count);
  send(client_socket, &count, sizeof(int), 0);
  for(int x = 0; x < count; x++){
    send(client_socket, songs[x], strlen(songs[x]) + 1, 0);
  }
  free_song_array(songs, count);

  char title[256];
  sizes = recv(client_socket,title, sizeof(title),0);
  if(sizes <= 0){
    close(client_socket);
    exit(1);
  }
  struct song_node *song = search_song(library, artist, title);
  char buffer[256];
  sprintf(buffer, "%s - %s", song-> artist, song-> title);
  int len = strlen(buffer) + 1;
  send(client_socket, buffer, len, 0);
}

int main(int argc, char *argv[]) {

  printf("LINKED LIST TESTS\n====================================\n");

  struct song_node * list = NULL;

  list = insert_front(list, "Taylor Swift", "Anti-Hero");
  list = insert_front(list, "Taylor Swift", "Cardigan");
  list = insert_front(list, "Taylor Swift", "You Belong With Me");
  list = insert_front(list, "Taylor Swift", "Shake It Off");
  list = insert_front(list, "Taylor Swift", "Blank Space");
  list = insert_front(list, "Taylor Swift", "Love Story");
  list = insert_front(list, "Katy Perry", "Teenage Dream");
  list = insert_front(list, "Katy Perry", "Firework");
  list = insert_front(list, "Katy Perry", "Roar");
  list = insert_front(list, "Billy Joel", "We Didn't Start the Fire");
  list = insert_front(list, "Billy Joel", "Uptown Girl");
  list = insert_front(list, "Billy Joel", "Piano Man");
  list = insert_front(list, "Frank Sinatra", "New York, New York");
  list = insert_front(list, "Frank Sinatra", "My Way");
  list = insert_front(list, "Frank Sinatra", "Fly Me to the Moon");

  list = insert_front(list, "Adele", "Hello");
  list = insert_front(list, "Bruno Mars", "Uptown Funk");
  list = insert_front(list, "Coldplay", "Clocks");
  list = insert_front(list, "Drake", "God's Plan");
  list = insert_front(list, "Eminem", "Lose Yourself");
  list = insert_front(list, "Foo Fighters", "Everlong");
  list = insert_front(list, "Guns N' Roses", "Sweet Child O' Mine");
  list = insert_front(list, "Hozier", "Take Me to Church");
  list = insert_front(list, "Imagine Dragons", "Believer");
  list = insert_front(list, "Jay-Z", "Empire State of Mind");
  list = insert_front(list, "Kanye West", "Stronger");
  list = insert_front(list, "Linkin Park", "In the End");
  list = insert_front(list, "Muse", "Starlight");
  list = insert_front(list, "Nirvana", "Smells Like Teen Spirit");
  list = insert_front(list, "Outkast", "Hey Ya!");
  list = insert_front(list, "Prince", "Purple Rain");
  list = insert_front(list, "Queen", "Bohemian Rhapsody");
  list = insert_front(list, "Radiohead", "Paranoid Android");
  list = insert_front(list, "Stevie Wonder", "Superstition");
  list = insert_front(list, "The Weeknd", "Blinding Lights");
  list = insert_front(list, "U2", "One");
  list = insert_front(list, "Vance Joy", "Riptide");
  list = insert_front(list, "Weezer", "Buddy Holly");
  list = insert_front(list, "X Japan", "Forever Love");
  list = insert_front(list, "Yeah Yeah Yeahs", "Maps");
  list = insert_front(list, "ZZ Top", "La Grange");
  list = insert_front(list, "#1 Dads", "So Soldier");
  list = insert_front(list, "50 Cent", "In Da Club");

  list = NULL;

  struct song_node ** library = init();

  printf("MUSIC LIBRARY TESTS\n====================================\n");

  printf("init works\n");
  add(library, "Taylor Swift", "Love Story");
  add(library, "Taylor Swift", "Blank Space");
  add(library, "Taylor Swift", "Shake It Off");
  add(library, "Taylor Swift", "You Belong With Me");
  add(library, "Taylor Swift", "Cardigan");
  add(library, "Taylor Swift", "Anti-Hero");
  add(library, "Katy Perry", "Roar");
  add(library, "Katy Perry", "Firework");
  add(library, "Katy Perry", "Teenage Dream");
  add(library, "Billy Joel", "Piano Man");
  add(library, "Billy Joel", "Uptown Girl");
  add(library, "Billy Joel", "We Didn't Start the Fire");
  add(library, "Frank Sinatra", "Fly Me to the Moon");
  add(library, "Frank Sinatra", "My Way");
  add(library, "Frank Sinatra", "New York, New York");

  add(library, "Adele", "Hello");
  add(library, "Bruno Mars", "Uptown Funk");
  add(library, "Coldplay", "Clocks");
  add(library, "Drake", "God's Plan");
  add(library, "Eminem", "Lose Yourself");
  add(library, "Foo Fighters", "Everlong");
  add(library, "Guns N' Roses", "Sweet Child O' Mine");
  add(library, "Hozier", "Take Me to Church");
  add(library, "Imagine Dragons", "Believer");
  add(library, "Jay-Z", "Empire State of Mind");
  add(library, "Kanye West", "Stronger");
  add(library, "Linkin Park", "In the End");
  add(library, "Muse", "Starlight");
  add(library, "Nirvana", "Smells Like Teen Spirit");
  add(library, "Outkast", "Hey Ya!");
  add(library, "Prince", "Purple Rain");
  add(library, "Queen", "Bohemian Rhapsody");
  add(library, "Radiohead", "Paranoid Android");
  add(library, "Stevie Wonder", "Superstition");
  add(library, "The Weeknd", "Blinding Lights");
  add(library, "U2", "One");
  add(library, "Vance Joy", "Riptide");
  add(library, "Weezer", "Buddy Holly");
  add(library, "X Japan", "Forever Love");
  add(library, "Yeah Yeah Yeahs", "Maps");
  add(library, "ZZ Top", "La Grange");
  add(library, "#1 Dads", "So Soldier");
  add(library, "50 Cent", "In Da Club");

  /*
  printf("Testing print_letter\n");
  print_letter(library, 'T');
  printf("\n");

  printf("Testing print_library\n");
  print_library(library);
  printf("\n");

  struct song_node * found = find_song("Taylor Swift", "Shake It Off", library);

  printf("Testing find:\n");
  found = search_song(library, "Taylor Swift", "Shake It Off");
  if(found) printf("\tsong found! [%s, %s]\n", found->artist, found->title);
  else printf("\tsong not found\n");

  found = search_song(library, "Billy Joel", "Uptown Girl");
  if(found) printf("\tsong found! [%s, %s]\n", found->artist, found->title);
  else printf("\tsong not found\n");

  printf("Testing shuffle:\n");
  shuffle(library, 5);
  printf("\n");

  printf("Testing print_artist:\n");
  printf("Printing [Taylor Swift]\n");
  print_artist(library, "Taylor Swift");
  printf("\n");

  printf("Printing [Frank Sinatra]\n");
  print_artist(library, "Frank Sinatra");
  printf("\n");

  printf("Printing [Katy Perry]\n");
  print_artist(library, "Katy Perry");
  printf("\n");
*/
  // the above part uploads songs while the below part is the server client setup aspect


  signal(SIGINT, sighandler);
  printf("bind complete\n");
  int listen_socket = server_setup();
  printf("server listening for connections.\n");
  while (1) {
    int client_socket = server_tcp_handshake(listen_socket);
    pid_t subserver = fork();
    if (subserver == 0) {
      close(listen_socket);
      while (1) {
        subserver_logic(client_socket, library);
        //sleep(1);
      }
      //close(listen_socket);
      close(client_socket);
      exit(0);
    } else {
      // int num;
      // wait(&num);
      //close(listen_socket);
      close(client_socket);
      printf("Waiting for next client...\n");
    }
  }
  return 0;
}
