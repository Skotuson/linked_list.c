#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main ( void ) {
    long long a, b, r;
    srand( time(NULL) );    
    for ( int i = 0; i < 100; i++ ) {
        a = rand();
        b = rand();
        r = a + b;
        printf("test ( \"%lld\", \"%lld\", \"%lld\" );\n", a, b, r);
    }
    return 0;
}