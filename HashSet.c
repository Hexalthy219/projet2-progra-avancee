#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include "Set.h"

typedef struct liste_t Liste;

struct liste_t{
    char *cle;
    Liste *suivant;
};

struct set_t{
    Liste **tableau;
    size_t taille;
    size_t nbr_elem;
};

static size_t fonction_h(const char *elem, size_t m);

static void freelist(Liste *liste);

static size_t fonction_h(const char *elem, size_t m){
    size_t pos = 0;
    unsigned int i= 0;
    while(elem[i]){
        pos += elem[i]*pow(128, i);
        i++;
    }

    return pos%m;
}

static void freelist(Liste *liste){
    if(liste->suivant)
        freelist(liste->suivant);

    free(liste);
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
    if(!set->tableau)
        free(set->tableau);
    free(set); 
}

size_t sizeOfSet(const Set *set){
    return set->nbr_elem;
}

insert_t insertInSet(Set *set, char *element){
    if(!set)
        return ALLOC_ERROR;
    
    if(!set->tableau || !set->taille)
        set->tableau = calloc(set->taille, sizeof(Liste));
    if(!set->tableau)
        return ALLOC_ERROR;
    
    size_t pos = fonction_h(element, set->taille);
    

    Liste *tampon = set->tableau[pos];
    while(tampon && tampon->suivant){
        if(!strcmp(element, tampon->cle))
            return OLD;
        tampon = tampon->suivant;
    }

    Liste *nouveau = malloc(sizeof(Liste));
    if(!nouveau)
        return ALLOC_ERROR;
    nouveau->cle = element;
    nouveau->suivant = NULL;

    if(!tampon){
        tampon = nouveau;
        set->tableau[pos] = tampon;
    }
    else
        tampon->suivant = nouveau;

    set->nbr_elem++;
    return NEW;
}

bool contains(const Set *set, const char *element){
    size_t pos = fonction_h(element, set->taille);
    Liste *tampon = set->tableau[pos];
    while(tampon){
        if(strcmp(element, tampon->cle))
            return true;
        tampon = tampon->suivant;
    }
    return false;
}

StringArray *setIntersection(const Set *set1, const Set *set2){}

void test_affichage(const StringArray *test){
    Set *table_test = createEmptySet();
    if(!table_test)
        return;
    table_test->taille = arrayLength(test);
    size_t i;
    for(i = 0; i<table_test->taille; i++){
        insertInSet(table_test, getElementInArray(test, i));
    }
    
    i=0;
    while(i<table_test->taille){
        Liste *tampon = table_test->tableau[i];
        while(tampon){
            printf("%s\n", tampon->cle);
            printf("%zd\n", i);
            tampon = tampon->suivant;
        }
            
        i++;
    }

}