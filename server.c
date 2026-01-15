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
  //reciecing letter
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


  int fd = open(song -> filepath, O_RDONLY);
  if(fd < 0){
    strerror(errno);
    close(client_socket);
    exit(1);
  }
  char buffs[1024];
  int bytes_read;
  bytes_read = read(fd, buffer, sizeof(buffer));
  while(bytes_read > 0){
    send(client_socket, buffs, bytes_read, 0);
    bytes_read = read(fd, buffs, sizeof(buffs));
  }
  close(fd);
  printf("Finished streaming: %s - %s\n", song -> artist, song -> title);
}

int main(int argc, char *argv[]) {


 struct song_node *list = NULL;

list = insert_front(list, "Taylor Swift", "Anti-Hero", "/home/user/music/Taylor_Swift_Anti-Hero.mp3");
list = insert_front(list, "Taylor Swift", "Cardigan", "/home/user/music/Taylor_Swift_Cardigan.mp3");
list = insert_front(list, "Taylor Swift", "You Belong With Me", "/home/user/music/Taylor_Swift_You_Belong_With_Me.mp3");
list = insert_front(list, "Taylor Swift", "Shake It Off", "/home/user/music/Taylor_Swift_Shake_It_Off.mp3");
list = insert_front(list, "Taylor Swift", "Blank Space", "/home/user/music/Taylor_Swift_Blank_Space.mp3");
list = insert_front(list, "Taylor Swift", "Love Story", "/home/user/music/Taylor_Swift_Love_Story.mp3");

list = insert_front(list, "Katy Perry", "Teenage Dream", "/home/user/music/Katy_Perry_Teenage_Dream.mp3");
list = insert_front(list, "Katy Perry", "Firework", "/home/user/music/Katy_Perry_Firework.mp3");
list = insert_front(list, "Katy Perry", "Roar", "/home/user/music/Katy_Perry_Roar.mp3");

list = insert_front(list, "Billy Joel", "We Didn't Start the Fire", "/home/user/music/Billy_Joel_We_Didnt_Start_the_Fire.mp3");
list = insert_front(list, "Billy Joel", "Uptown Girl", "/home/user/music/Billy_Joel_Uptown_Girl.mp3");
list = insert_front(list, "Billy Joel", "Piano Man", "/home/user/music/Billy_Joel_Piano_Man.mp3");

list = insert_front(list, "Frank Sinatra", "New York, New York", "/home/user/music/Frank_Sinatra_New_York_New_York.mp3");
list = insert_front(list, "Frank Sinatra", "My Way", "/home/user/music/Frank_Sinatra_My_Way.mp3");
list = insert_front(list, "Frank Sinatra", "Fly Me to the Moon", "/home/user/music/Frank_Sinatra_Fly_Me_to_the_Moon.mp3");

list = insert_front(list, "Adele", "Hello", "/home/user/music/Adele_Hello.mp3");

list = insert_front(list, "Bruno Mars", "Uptown Funk", "/home/user/music/Bruno_Mars_Uptown_Funk.mp3");

list = insert_front(list, "Coldplay", "Clocks", "/home/user/music/Coldplay_Clocks.mp3");

list = insert_front(list, "Drake", "God's Plan", "/home/user/music/Drake_Gods_Plan.mp3");

list = insert_front(list, "Eminem", "Lose Yourself", "/home/user/music/Eminem_Lose_Yourself.mp3");

list = insert_front(list, "Foo Fighters", "Everlong", "/home/user/music/Foo_Fighters_Everlong.mp3");

list = insert_front(list, "Guns N' Roses", "Sweet Child O' Mine", "/home/user/music/Guns_N_Roses_Sweet_Child_O_Mine.mp3");

list = insert_front(list, "Hozier", "Take Me to Church", "/home/user/music/Hozier_Take_Me_to_Church.mp3");

list = insert_front(list, "Imagine Dragons", "Believer", "/home/user/music/Imagine_Dragons_Believer.mp3");

list = insert_front(list, "Jay-Z", "Empire State of Mind", "/home/user/music/Jay-Z_Empire_State_of_Mind.mp3");

list = insert_front(list, "Kanye West", "Stronger", "/home/user/music/Kanye_West_Stronger.mp3");

list = insert_front(list, "Linkin Park", "In the End", "/home/user/music/Linkin_Park_In_the_End.mp3");

list = insert_front(list, "Muse", "Starlight", "/home/user/music/Muse_Starlight.mp3");

list = insert_front(list, "Nirvana", "Smells Like Teen Spirit", "/home/user/music/Nirvana_Smells_Like_Teen_Spirit.mp3");

list = insert_front(list, "Outkast", "Hey Ya!", "/home/user/music/Outkast_Hey_Ya.mp3");

list = insert_front(list, "Prince", "Purple Rain", "/home/user/music/Prince_Purple_Rain.mp3");

list = insert_front(list, "Queen", "Bohemian Rhapsody", "/home/user/music/Queen_Bohemian_Rhapsody.mp3");

list = insert_front(list, "Radiohead", "Paranoid Android", "/home/user/music/Radiohead_Paranoid_Android.mp3");

list = insert_front(list, "Stevie Wonder", "Superstition", "/home/user/music/Stevie_Wonder_Superstition.mp3");

list = insert_front(list, "The Weeknd", "Blinding Lights", "/home/user/music/The_Weeknd_Blinding_Lights.mp3");

list = insert_front(list, "U2", "One", "/home/user/music/U2_One.mp3");

list = insert_front(list, "Vance Joy", "Riptide", "/home/user/music/Vance_Joy_Riptide.mp3");

list = insert_front(list, "Weezer", "Buddy Holly", "/home/user/music/Weezer_Buddy_Holly.mp3");

list = insert_front(list, "X Japan", "Forever Love", "/home/user/music/X_Japan_Forever_Love.mp3");

list = insert_front(list, "Yeah Yeah Yeahs", "Maps", "/home/user/music/Yeah_Yeah_Yeahs_Maps.mp3");

list = insert_front(list, "ZZ Top", "La Grange", "/home/user/music/ZZ_Top_La_Grange.mp3");

list = insert_front(list, "#1 Dads", "So Soldier", "/home/user/music/#1_Dads_So_Soldier.mp3");

list = insert_front(list, "50 Cent", "In Da Club", "/home/user/music/50_Cent_In_Da_Club.mp3");

  struct song_node ** library = init();
add(library, "Taylor Swift", "Love Story", "/home/user/music/Taylor_Swift_Love_Story.mp3");
add(library, "Taylor Swift", "Blank Space", "/home/user/music/Taylor_Swift_Blank_Space.mp3");
add(library, "Taylor Swift", "Shake It Off", "/home/user/music/Taylor_Swift_Shake_It_Off.mp3");
add(library, "Taylor Swift", "You Belong With Me", "/home/user/music/Taylor_Swift_You_Belong_With_Me.mp3");
add(library, "Taylor Swift", "Cardigan", "/home/user/music/Taylor_Swift_Cardigan.mp3");
add(library, "Taylor Swift", "Anti-Hero", "/home/user/music/Taylor_Swift_Anti-Hero.mp3");


add(library, "Katy Perry", "Roar", "/home/user/music/Katy_Perry_Roar.mp3");
add(library, "Katy Perry", "Firework", "/home/user/music/Katy_Perry_Firework.mp3");
add(library, "Katy Perry", "Teenage Dream", "/home/user/music/Katy_Perry_Teenage_Dream.mp3");


add(library, "Billy Joel", "Piano Man", "/home/user/music/Billy_Joel_Piano_Man.mp3");
add(library, "Billy Joel", "Uptown Girl", "/home/user/music/Billy_Joel_Uptown_Girl.mp3");
add(library, "Billy Joel", "We Didn't Start the Fire", "/home/user/music/Billy_Joel_We_Didnt_Start_the_Fire.mp3");


add(library, "Frank Sinatra", "Fly Me to the Moon", "/home/user/music/Frank_Sinatra_Fly_Me_to_the_Moon.mp3");
add(library, "Frank Sinatra", "My Way", "/home/user/music/Frank_Sinatra_My_Way.mp3");
add(library, "Frank Sinatra", "New York, New York", "/home/user/music/Frank_Sinatra_New_York_New_York.mp3");


add(library, "Adele", "Hello", "/home/user/music/Adele_Hello.mp3");
add(library, "Bruno Mars", "Uptown Funk", "/home/user/music/Bruno_Mars_Uptown_Funk.mp3");
add(library, "Coldplay", "Clocks", "/home/user/music/Coldplay_Clocks.mp3");
add(library, "Drake", "God's Plan", "/home/user/music/Drake_Gods_Plan.mp3");
add(library, "Eminem", "Lose Yourself", "/home/user/music/Eminem_Lose_Yourself.mp3");
add(library, "Foo Fighters", "Everlong", "/home/user/music/Foo_Fighters_Everlong.mp3");
add(library, "Guns N' Roses", "Sweet Child O' Mine", "/home/user/music/Guns_N_Roses_Sweet_Child_O_Mine.mp3");
add(library, "Hozier", "Take Me to Church", "/home/user/music/Hozier_Take_Me_to_Church.mp3");
add(library, "Imagine Dragons", "Believer", "/home/user/music/Imagine_Dragons_Believer.mp3");
add(library, "Jay-Z", "Empire State of Mind", "/home/user/music/Jay-Z_Empire_State_of_Mind.mp3");
add(library, "Kanye West", "Stronger", "/home/user/music/Kanye_West_Stronger.mp3");
add(library, "Linkin Park", "In the End", "/home/user/music/Linkin_Park_In_the_End.mp3");
add(library, "Muse", "Starlight", "/home/user/music/Muse_Starlight.mp3");
add(library, "Nirvana", "Smells Like Teen Spirit", "/home/user/music/Nirvana_Smells_Like_Teen_Spirit.mp3");
add(library, "Outkast", "Hey Ya!", "/home/user/music/Outkast_Hey_Ya.mp3");
add(library, "Prince", "Purple Rain", "/home/user/music/Prince_Purple_Rain.mp3");
add(library, "Queen", "Bohemian Rhapsody", "/home/user/music/Queen_Bohemian_Rhapsody.mp3");
add(library, "Radiohead", "Paranoid Android", "/home/user/music/Radiohead_Paranoid_Android.mp3");
add(library, "Stevie Wonder", "Superstition", "/home/user/music/Stevie_Wonder_Superstition.mp3");
add(library, "The Weeknd", "Blinding Lights", "/home/user/music/The_Weeknd_Blinding_Lights.mp3");
add(library, "U2", "One", "/home/user/music/U2_One.mp3");
add(library, "Vance Joy", "Riptide", "/home/user/music/Vance_Joy_Riptide.mp3");
add(library, "Weezer", "Buddy Holly", "/home/user/music/Weezer_Buddy_Holly.mp3");
add(library, "X Japan", "Forever Love", "/home/user/music/X_Japan_Forever_Love.mp3");
add(library, "Yeah Yeah Yeahs", "Maps", "/home/user/music/Yeah_Yeah_Yeahs_Maps.mp3");
add(library, "ZZ Top", "La Grange", "/home/user/music/ZZ_Top_La_Grange.mp3");
add(library, "#1 Dads", "So Soldier", "/home/user/music/1_Dads_So_Soldier.mp3");
add(library, "50 Cent", "In Da Club", "/home/user/music/50_Cent_In_Da_Club.mp3");

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
