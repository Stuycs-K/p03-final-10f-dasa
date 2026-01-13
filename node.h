#ifndef NODE_H
#define NODE_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct song_node{char artist[100];char title[100]; struct song_node *next;};
int print_song_node(struct song_node m);
struct song_node * createnode(char artist[100], char title[100], struct song_node *next);
struct song_node * insert_front(struct song_node * list,char * artist, char * title);
int compare(struct song_node * a,struct song_node * b);
struct song_node * front_abc(struct song_node * list, char * artist, char * title);
void print_song_list(struct song_node * list);
struct song_node * find_song(char * artist, char * title, struct song_node * list);
struct song_node * find_first(char * artist, struct song_node * list);
struct song_node * random_song(struct song_node * list);
struct song_node * remove_song(struct song_node * list, char * artist, char * title);
void freeing(struct song_node * list);
#endif
