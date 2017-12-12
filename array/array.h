#ifndef ARRAY_H
#define ARRAY_H

#include <stdbool.h>

// initial capacity
#define ARRAY_INITIAL_CAPACITY 2

typedef struct Array Array;

struct Array {
    void *data;         // pointer to array of elements
    int elementSize;    // size of one array's element in bytes
    int capacity;       // Number of allocated free elements / capacity
    int length;         // number of active elements
};


typedef int (*sortFunc) (const void *a, const void *b);

bool arrayTest();

/**
 * Creates a new Array
 *
 * @return new array
 */
void *arrayCreate(int elementSize);

/**
 * Destrois the array
 */
void arrayDestroy(Array *array);

/**
 * Get the number of elements in the array
 *
 * @param array - the array to check
 * @return number of elements
 */
int arrayLen(Array *array);

/**
 * Creates a new element inside the array
 *
 * @param array - the array
 * @return a pointer to a new allocated element inside the array,
 *      or NULL on error
 */
void *arrayNewElement(Array *array);

/**
 * Deletes the element at index idx and shrink the array
 * shifting the elements after it
 * It also change the lenght of array, so use it careful in loops
 *
 * @param array - the array
 * @param idx - index to delete
 * @return true on success false on error
 */
bool arrayDeleteIndex(Array *array, int idx);

/**
 * Search for element pointed by *element and deletes it
 * shifting the elements after it
 * It also change the lenght of array, so use it careful in loops
 *
 * @param array - the array
 * @param element - pointer to element to delete
 * @return true on success, false on error
 */
bool arrayDeleteElement(Array *array, void *element);

void arrayReset(Array *array);
void arraySort(Array *array, sortFunc func);

#endif // ARRAY_H
