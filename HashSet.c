#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Set.h"

typedef struct liste_t Liste;

struct liste_t{
    char *cle;
    Liste suivant;
}

struct set_t{
    Liste **tableau;
    size_t taille;
    size_t nbr_elem;
}

static unsigned int fonction_h(char* elem);

static unsigned int freelist(Liste *liste);

static size_t fonction_h(char *elem, size_t m){
    size_t pos = 0;
    unsigned int i= 0;
    while(elem[i])
        pos+= elem[i]*pow(128, i);

    return pos%m;
}

static void freelist(Liste *liste){
    Liste tampon = liste->suivant;
    free(liste);

    if(tampon)
        freelist(tampon);
}


Set *createEmptySet(void){
    Set *dict = malloc(sizeof(Set));
    if(!dict)
        return NULL;
    dict->tableau = NULL;
    dict->taille = 0;
    
    return dict;
}

void freeSet(Set* set){
    if(!set)
        return;

    size_t i = 0;
    while(set->taille>i){
        if(set->tableau[i])
            freelist(set->tableau[i]);
        i++;
    }
    free(set);
    
}

size_t sizeOfSet(const Set *set){
    return set->nbr_elem;
}

insert_t insertInSet(Set *set, char *element){
    if(set)
        set = createEmptySet();
    if(set)
        return ALLOC_ERROR;
    
    if(set->tableau)
        set->tableau = malloc(sizeof(Liste)* set->taille);
    if(set->tableau)
        return ALLOC_ERROR;
    
    size_t pos = fonction_h(element);
    Liste nouveau = malloc(sizeof(Liste));
    nouveau->cle = element;
    Liste tampon = set->tableau[pos];
    while(tampon->suivant!= NULL && tampon->suivant!=NULL)
        tampon = tampon->suivant;
    tampon->suivant = nouveau;
    set->nbr_elem++;
    return NEW;
}

bool contains(const Set *set, const char *element){
    size_t pos = fonction_h(element, set->taille);
    Liste tampon = set->tableau[pos];
    while(tampon!=NULL){
        if(strcmp(element, tampon->cle))
            return true;
        tampon = tampon->suivant;
    }
    return false;
}

StringArray *setIntersection(const Set *set1, const Set *set2){}