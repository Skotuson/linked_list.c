#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void gen_sum ( int cnt ) {
    long long a, b, r;
    for ( int i = 0; i < cnt; i++ ) {
        a = rand();
        b = rand();
        r = a + b;
        printf("test_sum ( \"%lld\", \"%lld\", \"%lld\" );\n", a, b, r);
    }
}

void gen_shift ( int cnt ) {

}

int main ( void ) {
    int c = 0;
    if ( scanf( "%d", & c ) != 1 || c < 1 || c > 2 ) {
        printf("Neplatny vstup.\n");
        return 1;
    } 
    srand( time(NULL) );
    switch ( c ) {
        case 1:
            gen_sum ( 1000 );
            break;
        case 2:
            gen_shift ( 100 );
            break;
    }    
  
    return 0;
}