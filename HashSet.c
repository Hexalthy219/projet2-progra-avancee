#include <stdlib.h>
#include <string.h>
#include <math.h>

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

/* ------------------------------------------------------------------------- *
 * La fonction est une fonction de hachage donc renvoit une certaine valeur 
 * suivant la formule de hachage choisie porpre à la chaine de 
 * caractère elem en fonction de la taille m.
 *
 * PARAMETERS
 * elem         pointeur valide vers une chaine de caractères
 * m            un valeur entière correspond à la taille du tableau de la table de hash
 * 
 * RETURN
 * size_t      un entier correspondant à la valeur pour elem suivant la formule de hachage
 *
 * ------------------------------------------------------------------------- */
static size_t fonction_h(const char *elem, size_t m);

/* ------------------------------------------------------------------------- *
 * La fonction libère la mémoire de la liste donnée en argument et toutes 
 * les autres liste suivante chainée à celle-ci
 *
 * PARAMETERS
 * liste        pointeur valide vers une struct Liste       
 *
 * ------------------------------------------------------------------------- */
static void freelist(Liste *liste);

/* ------------------------------------------------------------------------- *
 * La fonction double la taille de la table contenue dans set et replace 
 * les éléments s'y trouvant déjà à leur nouvelle position
 *
 * PARAMETERS
 * set          pointeur valide vers une struct Set        
 *
 * RETURN
 * -1     en cas d'erreur
 * 0      sinon   
 * 
 * ------------------------------------------------------------------------- */
static int rehachage(Set *set);

static int rehachage(Set *set){
    Liste **tableau = calloc(set->taille*2, sizeof(Liste*));
    if(!tableau)
        return -1;
    Liste *intermediaire, *tampon, *intermediaire_precedent;
    size_t pos;
    
    for(size_t i = 0; i<set->taille; i++){
        intermediaire = set->tableau[i];
        while(intermediaire){
            pos = fonction_h(intermediaire->cle, set->taille*2);
            tampon = tableau[pos];
            while(tampon && tampon->suivant)
                tampon = tampon->suivant;

            if(!tampon)
                tampon = intermediaire;
            else
                tampon->suivant = intermediaire;
            intermediaire_precedent = intermediaire;
            intermediaire = intermediaire->suivant;
            intermediaire_precedent->suivant = NULL;
        }
        free(set->tableau[i]);
    }
    free(set->tableau);
    set->tableau = tableau;
    set->taille *= 2;
    return 0;
}

static size_t fonction_h(const char *elem, size_t m){
    size_t pos = 0;
    unsigned int i= 0;
    while(elem[i]){
        pos += (unsigned int)(elem[i]*pow(727, i))%m;
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
    dict->taille = 64;
    
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

    //rehachage dans le cas où le nombre d'éléménet atteint la taille du set
    if(set->nbr_elem == set->taille)
        rehachage(set);
    
    if(!set->tableau || !set->taille)
        set->tableau = calloc(set->taille, sizeof(Liste*));
    if(!set->tableau)
        return ALLOC_ERROR;
    
    size_t pos = fonction_h(element, set->taille);
    
    //Recherche de l'emplacement où ajouter le nouvel élément et retourne OLD si l'élément existe déjà
    Liste *tampon = set->tableau[pos];
    while(tampon && tampon->suivant){
        if(!strcmp(element, tampon->cle))
            return OLD;
        tampon = tampon->suivant;
    }

    //Création et ajout de la nouvelle céllule au tableau
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
    size_t i = 0;
    while(tampon){
        if(!strcmp(element, tampon->cle)){
            return true;
        }
        tampon = tampon->suivant;
        i++;
    }
    return false;
}