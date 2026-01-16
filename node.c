#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct song_node{char artist[100];char title[100]; char filepath[256]; struct song_node *next;};
//ok below is needed
int print_song_node(struct song_node m){
	printf("%s, %s \n", m.artist, m.title);
	return 0;
}

char * string_song_node(struct song_node m){
    char * returnable = malloc(strlen(m.artist) + strlen(m.title) + 4);
    sprintf(returnable, "%s - %s", m.artist, m.title);
    return returnable;
}
//below is needed just might need some modifications depending on how i want each thing to be....seems to be especially true if i want to do mpg files
struct song_node * createnode(char artist[100], char title[100], char filepath[256], struct song_node *next){
    struct song_node *p = (struct song_node*) malloc(sizeof(struct song_node));

    strncpy(p->artist, artist, 99);
    p->artist[99] = '\0';

    strncpy(p->title, title, 99);
    p->title[99] = '\0';

    strncpy(p-> filepath, filepath, 255);
    p-> filepath[255] = '\0';

    p->next = next;
    return p;
}

//ok needed since its linked list
struct song_node * insert_front(struct song_node * list,char * artist, char * title, char * filepath){
    struct song_node * newSongNode = createnode(artist,title,filepath, list);
    return newSongNode;
}


//ok maybe maybe not. prob not needed lowk
int compare(struct song_node * a,struct song_node * b){

    int artistCmp = strcasecmp(a->artist,b->artist);// non case sensitive string compare
    if(artistCmp != 0){
        return artistCmp;//if artists are different compare
    }
    return strcasecmp(a->title,b->title);//if artists are the same compare titles
}

//um not really sure how this is supposed to work/be needed but alr will see in a bit (come back later)
struct song_node * front_abc(struct song_node * list, char * artist, char * title, char * filepath){
    struct song_node * newSongNode = createnode(artist,title, filepath, NULL);//null because it doesnt have a next node yet

    if(list == NULL){
        return newSongNode;//if the list is null set it equal to the new node that is created
    }

    if(compare(newSongNode, list)< 0){
        newSongNode->next = list; //sets it to the front of the list if it is alphabeticaly in front
        return newSongNode;
    }

    struct song_node * current = list;
    while(current->next != NULL && compare(current->next, newSongNode) < 0){//keeps going down the list until it needs to be in front of another node
        current = current -> next;
    }

    newSongNode->next = current->next;//places the new song node in between current and the next node after current
    current->next = newSongNode;

    return list;
}

//yeah prob needed

char * print_song_list(struct song_node * list){
    struct song_node * current = list;
    while(current != NULL){
        printf("[%s,%s] | ",current->artist,current->title);//iterates through the list until therer is nothing left printing the artist followed by the title
        current = current->next;
    }
}


//most definitely needed. it needs to be especailly modified to look for things like strings

struct song_node * find_song(char * artist, char * title, struct song_node * list){
    struct song_node * current = list;
    while(current != NULL ){
      if(strcasecmp(current->artist,artist) == 0 && strcasecmp(current->title,title)== 0){//makes sure both the artist and title are correct
        return current;
      }
      else{
          current = current->next;
      }
    }
    return NULL;
}


//ok i think i need to modify this to print whole list of front initial
struct song_node * find_first(char * artist, struct song_node * list){//just makes sure artist is correct, then prints first song
    struct song_node * current = list;
    while(current != NULL){
        if(strcasecmp(current->artist,artist) == 0){
            return current;
        }
        else{
            current = current->next;
        }
    }
    return NULL;
}


//um yeah might delete this. not a big fan.
struct song_node * random_song(struct song_node * list){
    struct song_node * current = list;
    int count = 0;
    int random = 0;

    if(current == NULL){
        return NULL;
    }

    while(current != NULL){
        count ++;
        current = current->next;
    }

   random += (rand() % count);//random number with how many elemts are in the list

    for(int i = 0; i < random; i ++){
        list = list->next;
    }
    return list;
}

// yes needed. :( or might try to reallocate storage to allow for everyone to be there
struct song_node * remove_song(struct song_node * list, char * artist, char * title){
    struct song_node * current = list;
    struct song_node * before = NULL;

    if(list == NULL){
        return NULL;
    }

    while(current != NULL){
        if(strcasecmp(current->artist, artist) == 0 && strcasecmp(current->title, title) == 0){

            if(before == NULL){
                struct song_node * front = current->next;
                free(current);
                return front;
            }

            before->next = current->next;
            free(current);
            return list;
        }

        before = current;
        current = current->next;
    }

    return list;
}

//yeah needed
void freeing(struct song_node * list){
    struct song_node * current = list;
    struct song_node * tempNext;

    while(current != NULL){
        tempNext = current->next;
        free(current);
        current = tempNext;
    }
}
