#include "array.h"

void doTests() 
{
#ifdef DO_TESTS
    arrayTest();
#endif
}

int main(void)
{
    doTests();
}
