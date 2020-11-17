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
    size_t taille;
};

/* ------------------------------------------------------------------------- *
 * La fonction supprime récursivement les fils du noeud qui lui est donné 
 * en paramètre.
 *
 * PARAMETERS
 * noeud           pointeur valide vers une struct Noeud
 *
 * ------------------------------------------------------------------------- */
static void suppresion_recursive(Noeud *noeud);


Set* createEmptySet(void){
    Set *dict = malloc(sizeof(Set));
    if(!dict)
        return NULL;

    dict->racine = NULL;
    dict->taille = 0;

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
    return set->taille;
}
/**
 * 
 * 
 * FAUT FAIRE LA FONCTION BALANCE LES FDP(HORS DES FRONTIERES), EH OUAIS JE SAIS QUE ÇA VOUS SOULE MAIS FAUT ARRÊTER DE SE BRANLER
 * 
 * 
 * 
 */
insert_t insertInSet(Set* set, char* element){
    Noeud *actuel = set->racine;
    int comparaison;
    int fils;
    while(actuel){
        comparaison = strcmp(element, actuel->cle);
        if(!comparaison)
            return OLD;
        if(comparaison>0){
            if(actuel->fils_droit==NULL){
               fils = 1;
               break;  
            }
            actuel = actuel->fils_droit;  
        }
        else{
            if(actuel->fils_gauche==NULL){
               fils = -1;
               break;  
            }
            actuel = actuel->fils_gauche;  
        }
    }
    Noeud *new = malloc(sizeof(Noeud));
    if(!new)
        return ALLOC_ERROR;

    new->cle = element;
    new->parent = actuel;
    new->fils_droit = new->fils_gauche = NULL;
    if(!actuel){
        actuel=new;
        return NEW;
    }

    if(fils==1)
        actuel->fils_droit = new;
    else
        actuel->fils_gauche = new;
    set->taille++;

    return NEW;
}

bool contains(const Set* set, const char* element){
    
}

StringArray* setIntersection(const Set* set1, const Set* set2){
    
}