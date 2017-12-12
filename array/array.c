#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "array.h"

void *arrayCreate(int elementSize)
{
    Array *array = malloc(sizeof(Array));
    if (!array) {
        fprintf(stderr, "Cannot allocate memory\n");
        return NULL;
    }
    array->elementSize = elementSize;
    array->capacity = 0;
    array->length = 0;
    array->data = NULL;

    return array;
}

static bool arrayGrow(Array *array)
{
    int newSize = array->capacity == 0
        ? ARRAY_INITIAL_CAPACITY * array->elementSize
        : array->capacity * 2 * array->elementSize;

    array->data = realloc(array->data, (size_t) newSize);
    if(!array->data) {
        fprintf(stderr, "Cannot reallocate memory\n");
        return false;
    }
    array->capacity = newSize / array->elementSize;
    return true;
}

void *arrayNewElement(Array *array)
{
    void *el;

    if (array->length == array->capacity) {
        if (!arrayGrow(array)) {
            return NULL;
        }
    }
    el = (void *)((char *)array->data + (array->length * array->elementSize));
    array->length++;

    return el;
}

bool arrayDeleteIndex(Array *array, int idx)
{
    char *to, *from;
    int size;

    if (array->capacity ==  0) 
        return false;

    if(idx > array->length)
        return false;

    size = (array->length - 1 - idx) * array->elementSize;
    to = (char *)array->data+(idx * array->elementSize);
    from = to + array->elementSize;

    memmove(to, from, size);
    array->length--;

    return true;
}

bool arrayDeleteElement(Array *array, void *element)
{
    char *p = array->data;
    for (int i = 0; i < array->length; i++, p+=array->elementSize) {
        if ( p == element) {
            arrayDeleteIndex(array, i);
            return true;
        }
    }
    fprintf(stderr, "Cannot found element %p\n", element);

    return false;
}

int arrayLen(Array *array)
{
    return array->length;
}

void arrayDestroy(Array *array)
{
    if (array->data) {
        free(array->data);
    }
    free(array);
}

void arrayReset(Array *array)
{
    memset(array->data, 0, array->capacity * array->elementSize);
    array->length = 0;
}


void arraySort(Array *array, sortFunc func)
{
    qsort(array->data, array->length, array->elementSize, func);
}

#ifdef DO_TESTS


struct Person {
    char name[40];
    int age;
};
typedef struct Person Person;

int sortPersons(const void *a, const void *b)
{
    const Person *p1 = a;
    const Person *p2 = b;

    //return p2->age - p1->age;
    return strcmp(p1->name, p2->name);
}

bool arrayTest()
{
    Person *p;
    Array *arr = arrayCreate(sizeof(*p));

    printf("Testing array\n");

    p = arrayNewElement(arr);
    strncpy(p->name, "Viorel Irimia", sizeof(p->name));
    p->age = 37;

    p = arrayNewElement(arr);
    strncpy(p->name, "John Doe", sizeof(p->name));
    p->age = 44;

    p = arrayNewElement(arr);
    strncpy(p->name, "Evelyn Irimia", sizeof(p->name));
    p->age = 6;

    p = arr->data;

    assert(arr->length == 3);
    assert(arr->capacity == 4);
    assert(strcmp(p[0].name, "Viorel Irimia") == 0);
    assert(p[0].age == 37);
    assert(strcmp(p[1].name, "John Doe") == 0);
    assert(p[1].age == 44);
    assert(strcmp(p[2].name, "Evelyn Irimia") == 0);
    assert(p[2].age == 6);


    arrayDeleteIndex(arr, 1);

    assert(arr->length == 2);
    assert(arr->capacity == 4);
    assert(strcmp(p[0].name, "Viorel Irimia") == 0);
    assert(p[0].age == 37);
    assert(strcmp(p[1].name, "Evelyn Irimia") == 0);
    assert(p[1].age == 6);

    arrayDeleteElement(arr, &p[0]);
    assert(arr->length == 1);
    assert(arr->capacity == 4);
    assert(strcmp(p[0].name, "Evelyn Irimia") == 0);
    assert(p[0].age == 6);

    arrayReset(arr);
    assert(arr->length == 0);
    assert(p[0].age == 0);
   
    p = arrayNewElement(arr);
    strncpy(p->name, "Viorel Irimia", sizeof(p->name));
    p->age = 37;
   
    p = arrayNewElement(arr);
    strncpy(p->name, "Evelyn Andreea", sizeof(p->name));
    p->age = 6;

    p = arrayNewElement(arr);
    strncpy(p->name, "John Doe", sizeof(p->name));
    p->age = 44;

    arraySort(arr, sortPersons);

    p = arr->data;
    for (int i = 0; i < arr->length; i++, p++) {
        printf("%s %d\n", p->name, p->age);
    }
    
    arrayDestroy(arr);

    return true;
}
#endif
