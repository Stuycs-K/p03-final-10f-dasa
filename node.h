#ifndef NODE_H
#define NODE_H
struct song_node {
  char artist[100];
  char title[100];
  struct song_node *next;
};
// ok below is needed
int print_song_node(struct song_node m);
// below is needed just might need some modifications depending on how i want
// each thing to be....seems to be especially true if i want to do mpg files
struct song_node *createnode(char artist[100], char title[100],
                             struct song_node *next);
// ok needed since its linked list
struct song_node *insert_front(struct song_node *list, char *artist,
                               char *title);
// ok maybe maybe not. prob not needed lowk
int compare(struct song_node *a, struct song_node *b);
// um not really sure how this is supposed to work/be needed but alr will see in
// a bit (come back later)
struct song_node *front_abc(struct song_node *list, char *artist, char *title);
// yeah prob needed
void print_song_list(struct song_node *list);
// most definitely needed. it needs to be especailly modified to look for things
// like strings
struct song_node *find_song(char *artist, char *title, struct song_node *list);
// ok i think i need to modify this to print whole list of front initial
struct song_node *find_first(char *artist, struct song_node *list);
// um yeah might delete this. not a big fan.
struct song_node *random_song(struct song_node *list);
// yes needed. :( or might try to reallocate storage to allow for everyone to be
// there
struct song_node *remove_song(struct song_node *list, char *artist,
                              char *title);
// yeah needed
void freeing(struct song_node *list);
#endif
