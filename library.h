#ifndef NODE_H
#define NODE_H
struct song_node ** init();
void add(struct song_node** library, char* artist, char* title);
struct song_node * search_song(struct song_node ** library, char* artist, char* title);
struct song_node * search_artist(struct song_node**library, char*artist, char*title);
void print_list(struct song_node * list);
void print_letter(struct song_node ** library, char letter);
void print_artist(struct song_node ** library, char* artist);
void print_library(struct song_node** library);
void shuffle(struct song_node **library, int n);
int delete_song(struct song_node ** library, char* artist, char* title);
void reset(struct song_node ** library);
#endif