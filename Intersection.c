#include "Set.h"
#include "StringArray.h"

StringArray* getIntersection(const StringArray* array1, StringArray* array2){
    Set *set1 = createEmptySet();
    Set *set2 = createEmptySet();
    if(!set1 || !set2){
        if(set2)
            freeSet(set2);
        else if(set1)
            freeSet(set1);
        return NULL;
    }

    size_t i = 0;
    StringArray *intersection;
    
    for(i = 0; i<arrayLength(array1); i++)
        insertInSet(set1, getElementInArray(array1, i));

    for(i = 0; i<arrayLength(array2); i++)
        insertInSet(set2, getElementInArray(array2, i));

    intersection = setIntersection(set1, set2);
    

    freeSet(set1);
    freeSet(set2);

    return intersection;
}
