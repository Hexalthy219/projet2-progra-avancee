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

    if (arrayLength(array1) > arrayLength(array2))
    {
        taille_set = arrayLength(array2);
        taille_tableau = arrayLength(array1);
        for (i = 0; i < taille_set; i++)
            insertInSet(set, getElementInArray(array2, i));
        
        for (i = 0; i < taille_tableau; i++){
            if (contains(set, getElementInArray(array1, i)))
                insertInArray(intersection, getElementInArray(array1, i));
        }
    }
    else
    {
        taille_set = arrayLength(array1);
        taille_tableau = arrayLength(array2);
        for (i = 0; i < taille_set; i++)
            insertInSet(set, getElementInArray(array1, i));

        for (i = 0; i < taille_tableau; i++){
            if (contains(set, getElementInArray(array2, i)))
                insertInArray(intersection, getElementInArray(array2, i));
        }
    }

    freeSet(set);

    return intersection;
}