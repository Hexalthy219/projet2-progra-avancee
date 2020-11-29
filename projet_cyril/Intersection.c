#include <stdio.h>
#include <time.h>

#include "Set.h"
#include "StringArray.h"



StringArray *getIntersection(const StringArray *array1, const StringArray *array2)
{
    if (!array1 || !array2)
        return NULL;

    StringArray *intersection = createEmptyArray();
    if (!intersection)
        return NULL;
    Set *set = createEmptySet();
    if (!set)
    {
        freeArray(intersection, 0);
        return NULL;
    }
    size_t i, taille_set, taille_tableau;
    clock_t start;
    double moyenne = 0;

    if (arrayLength(array1) > arrayLength(array2))
    {
        taille_set = arrayLength(array2);
        taille_tableau = arrayLength(array1);
        start = clock();
        for (i = 0; i < arrayLength(array2); i++)
            insertInSet(set, getElementInArray(array2, i));
        printf("temps pour creation set : %lf\n", ((double)(clock()-start))/CLOCKS_PER_SEC);
        
        start = clock();
        for (i = 0; i < arrayLength(array1); i++)
        {
            if (contains(set, getElementInArray(array1, i)))
                insertInArray(intersection, getElementInArray(array1, i));
        }
        printf("temps pour intersection : %lf\n", ((double)(clock() - start)) / CLOCKS_PER_SEC);
    }
    else
    {
        taille_set = arrayLength(array1);
        taille_tableau = arrayLength(array2);
        start = clock();
        for (i = 0; i < taille_set; i++)
            insertInSet(set, getElementInArray(array1, i));
        printf("temps pour creation set : %lf\n", ((double)(clock() - start)) / CLOCKS_PER_SEC);

        start = clock();
        for (i = 0; i < taille_tableau; i++){
            if (contains(set, getElementInArray(array2, i)))
                insertInArray(intersection, getElementInArray(array2, i));
            // moyenne += ((double)(clock() - start)) / CLOCKS_PER_SEC;
        }
        // moyenne /= i;
        printf("%lf\n", moyenne);
        printf("temps pour intersection : %lf\n", ((double)(clock() - start)) / CLOCKS_PER_SEC);
    }

    freeSet(set);

    return intersection;
}