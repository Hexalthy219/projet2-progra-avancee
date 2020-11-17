#include <stdlib.h>
#include <string.h>

#include "Set.h"

typedef struct noeud_t Noeud;

struct noeud_t{
    Noeud *parent, *fils_gauche, *fils_droit;
    char *cle;
};


struct set_t{
    Noeud *racine;
    // size_t hauteur;
};

static void suppresion_recursive(Noeud *noeud);


Set* createEmptySet(void){
    Set *dict = malloc(sizeof(Set));
    if(!dict)
        return NULL;

    dict->racine = NULL;

    return dict;
}

static void suppresion_recursive(Noeud *noeud){
    if(noeud->fils_droit!=NULL)
        suppresion_recursive(noeud->fils_droit);
    if(noeud->fils_gauche!=NULL)
        suppresion_recursive(noeud->fils_gauche);
    free(noeud);
}


void freeSet(Set* set){
    if(!set)
        return;

    if(set->racine!=NULL)
        suppresion_recursive(set->racine);

    free(set);
}



size_t sizeOfSet(const Set* set){
    
}

insert_t insertInSet(Set* set, char* element){
    
}

bool contains(const Set* set, const char* element){
    
}

StringArray* setIntersection(const Set* set1, const Set* set2){
    
}