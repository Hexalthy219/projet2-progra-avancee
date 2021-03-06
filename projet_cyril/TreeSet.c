#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Set.h"

typedef struct noeud_t Noeud;

struct noeud_t{
    Noeud *parent, *fils_gauche, *fils_droit;
    char *cle;
    size_t h;
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

/* ------------------------------------------------------------------------- *
 *  La fonction réquilibre l'arbre afin de garder une hauteur max de 1.44*log(n+2)
 *
 * PARAMETERS
 * noeud           pointeur valide vers une struct Noeud
 *
 * ------------------------------------------------------------------------- */
static void equilibrague_arbre(Noeud *noeud);

/* ------------------------------------------------------------------------- *
 * Applique une rotation droite de réiquilibrage sur "noeud"
 *
 * PARAMETERS
 * noeud           pointeur valide vers une struct Noeud
 *
 * ------------------------------------------------------------------------- */
static Noeud *rotation_droite(Noeud *noeud);

/* ------------------------------------------------------------------------- *
 * Applique une rotation gauche de réiquilibrage sur "noeud"
 *
 * PARAMETERS
 * noeud           pointeur valide vers une struct Noeud
 *
 * ------------------------------------------------------------------------- */
static Noeud *rotation_gauche(Noeud *noeud);

/* ------------------------------------------------------------------------- *
 * reajuste les hauteurs des noeuds parents de noeud
 *
 * PARAMETERS
 * noeud           pointeur valide vers une struct Noeud
 *
 * ------------------------------------------------------------------------- */
static void ajustement_hauteur(Noeud *noeud);

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

static Noeud *rotation_droite(Noeud *noeud){

    Noeud *tampon = noeud->fils_gauche;
    noeud->fils_gauche = tampon->fils_droit;
    tampon->fils_droit = noeud;

    tampon->parent = noeud->parent;
    noeud->parent = tampon;
    if(tampon->parent){
        if(strcmp(tampon->parent->cle, tampon->cle)>0)
            tampon->parent->fils_gauche = tampon;
        else 
            tampon->parent->fils_droit = tampon;
    }
    if (noeud->fils_gauche)
        noeud->fils_gauche->parent = noeud;
    if(tampon->fils_gauche)
        tampon->fils_gauche->parent = tampon;
    
    ajustement_hauteur(noeud);
    
    return tampon;
}

static Noeud *rotation_gauche(Noeud *noeud){

    Noeud *tampon = noeud->fils_droit;
    noeud->fils_droit = tampon->fils_gauche;
    tampon->fils_gauche = noeud;
    tampon->parent = noeud->parent;
    noeud->parent = tampon;
    if(tampon->parent){
        if(strcmp(tampon->parent->cle, tampon->cle)>0)
            tampon->parent->fils_gauche = tampon;
        else 
            tampon->parent->fils_droit = tampon;
    }
    if(noeud->fils_droit)
        noeud->fils_droit->parent = noeud;
    if (tampon->fils_droit)
        tampon->fils_droit->parent = tampon;

    ajustement_hauteur(noeud);

    return tampon;
}

static void equilibrague_arbre(Noeud *noeud){
    int balance;
    size_t h_droite, h_gauche;

    while(noeud){
        balance = 0, h_droite = 0, h_gauche = 0;
        if(noeud->fils_droit)
            balance += noeud->fils_droit->h + 1;
        if(noeud->fils_gauche)
            balance -= noeud->fils_gauche->h + 1;
        if(balance>1){
            if(noeud->fils_droit->fils_droit)
                h_droite = noeud->fils_droit->fils_droit->h + 1;
            if(noeud->fils_droit->fils_gauche)
                h_gauche = noeud->fils_droit->fils_gauche->h + 1;
            if(h_gauche>h_droite){
                rotation_droite(noeud->fils_droit);}
            noeud = rotation_gauche(noeud);
        }
        else if(balance<-1){
            if(noeud->fils_gauche->fils_droit)
                h_droite = noeud->fils_gauche->fils_droit->h + 1;
            if(noeud->fils_gauche->fils_gauche)
                h_gauche = noeud->fils_gauche->fils_gauche->h + 1;
            if(h_gauche<h_droite)
                rotation_gauche(noeud->fils_gauche);
            noeud = rotation_droite(noeud);
        }
        noeud = noeud->parent;
    }
}

static void ajustement_hauteur(Noeud *noeud){
    size_t h_droite = 0, h_gauche = 0;
    while(noeud){
        h_droite = 0, h_gauche = 0;
        if(noeud->fils_droit)
            h_droite = noeud->fils_droit->h + 1;
        if(noeud->fils_gauche)
            h_gauche = noeud->fils_gauche->h + 1;
        
        /*
        *max(h_gauche, h_droite) est la nouvelle hauteur du noeud.
        *Si h_gauche==h_droite, alors les hauteurs des noeuds parents 
        *sont également inchangés et donc on sort de la boucle
        */
        if(h_gauche>h_droite)
            noeud->h = h_gauche;
        else if(h_gauche<=h_droite)
            noeud->h = h_droite;
        noeud = noeud->parent;
    }
}   

insert_t insertInSet(Set* set, char* element){
    Noeud *actuel = set->racine;
    int comparaison;
    int fils;

    //recherche du noeud devant recevoir un nouveau fils
    while(actuel){
        comparaison = strcmp(element, actuel->cle);
        if(comparaison>0){
            if(actuel->fils_droit==NULL){
               fils = 1;
               break;  
            }
            actuel = actuel->fils_droit;  
        }
        else if (comparaison<0){
            if(actuel->fils_gauche==NULL){
               fils = -1;
               break;  
            }
            actuel = actuel->fils_gauche;
        }
        else
            return OLD;
        
    }
    //création du nouveau noeud
    Noeud *new = malloc(sizeof(Noeud));
    if(!new)
        return ALLOC_ERROR;

    new->cle = element;
    new->parent = actuel;
    new->fils_droit = new->fils_gauche = NULL;
    new->h = 0;
    if(!actuel){
        set->racine=new;
        set->taille++;
        return NEW;
    }

    //ajout du nouveau noeud au fils de droite ou de gauche de noeud "actuel"
    if(fils==1)
        actuel->fils_droit = new;
    else
        actuel->fils_gauche = new;
    set->taille++;
    ajustement_hauteur(actuel);
    equilibrague_arbre(actuel);

    while(set->racine->parent)
        set->racine = set->racine->parent;


    return NEW;
}

bool contains(const Set* set, const char* element){
    Noeud *actuel = set->racine;
    int comparaison;
    while(actuel){
        comparaison = strcmp(element, actuel->cle);
        if(!comparaison)
            return true;
        else if(comparaison>0)
            actuel = actuel->fils_droit;
        else
            actuel = actuel->fils_gauche;
    }
    
    return false;
}