#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

                                /*HELPER FUNCTIONS*/
//----------------------------------------------------------------------------------//
int get_min ( int a, int b ) {
    return a < b ? a : b;
}

/**
 * Sums the two numbers and checks if the result is bigger than 10.
 * If it is, it assigns value to the output parameter representing the "carry"
 * @param c output parameter representing the carry
 * @return The sum of a and b
*/
int sum_digits ( int a, int b, int * c ) {
    int r = a + b + *c;
    *c = 0;
    if ( r >= 10 ) {
        *c = 1;
        r %= 10;
    }
    return r;
}

typedef struct TNode {
    struct TNode * next;
    char digit;
} TNODE;

void print_list ( TNODE * l );

TNODE * create_node ( char digit, TNODE * next ) {
    TNODE * node = ( TNODE* ) malloc ( sizeof( TNODE ) );
    node -> next = next;
    node -> digit = digit;
    return node;
}

void del_list ( TNODE * l ) {
    TNODE * tmp = NULL;
    while ( l ) {
        tmp = l -> next;
        free ( l );
        l = tmp;
    }
}

int list_size ( TNODE * l ) {
    int size = 0;
    while ( l ) {
        size++;
        l = l -> next;
    }
    return size;
}

int is_valid ( TNODE * l ) {
    int isZero = 0;
    int f = 1;
    while ( l ) {
        if ( l -> digit == '0' && !f )
            isZero = 1;
        else isZero = 0;
        l = l -> next;
        f = 0;
    }
    return !isZero;
}

                                /*PARSING FUNCTIONS*/
//----------------------------------------------------------------------------------//

/**
 * Function takes the input string and turns each digit into linked list, keeping the same order of the digits. 
 * "1234" will be parsed into 1 -> 2 -> 3 -> 4 -> NULL
 * @param str input string with digits for the linked list
 * @return pointer to the head of the parsed linked list 
*/
TNODE * parse_list ( const char * str ) {
    TNODE * l = NULL, *curr = NULL;
    while ( *str ) {
        if ( isdigit ( *str ) ) {
            if ( !l ) 
                curr = l = create_node ( *str, NULL );
            else {
                curr -> next = create_node ( *str, NULL );
                curr = curr -> next;
            }
        }
        str++;
    }
    if ( !is_valid ( l ) ) {
        del_list ( l );
        return NULL;
    }
    return l;
}

/**
 * Function takes the input string and turns each digit into linked list, reversing the order of digits. 
 * "1234" will be parsed into 4 -> 3 -> 2 -> 1 -> NULL
 * @param str input string with digits for the linked list
 * @return pointer to the head of the parsed linked list 
*/
TNODE * parse_num ( const char * str ) {
    TNODE * l = NULL;   
    while ( *str ) {
        if ( isdigit ( *str ) ) {
            if ( !l ) 
                l = create_node ( *str, NULL );
            else l = create_node ( *str, l );    
        }
        str++;
    }
    if ( !is_valid ( l ) ) {
        del_list ( l );
        return NULL;
    }
    return l;
}
                                /*LIST OPERATIONS*/
//----------------------------------------------------------------------------------//

int compare_lists ( TNODE * a, TNODE * b ) {
    if ( list_size ( a ) != list_size ( b ) )
        return 0;
    while ( a ) {
        if ( a -> digit != b -> digit ) 
            return 0;
        a = a -> next;
        b = b -> next;
    }
    return 1;
}

/**
 * Functions sums the linked list provided in params.
 * @param a number represented as a linked list 
 * @param b number represented as a linked list
 * @return Pointer to the head of the linked list representing the sum.
*/
TNODE * list_sum ( TNODE * a, TNODE * b ) {
    
    TNODE * l = NULL, *curr = NULL;
    int n = get_min ( list_size ( a ), list_size ( b ) );
    int res = 0, carry = 0;
    while ( n-- ) {
        res = sum_digits ( a -> digit - '0', b -> digit - '0', &carry );
        if ( !l )
            curr = l = create_node ( res + '0', NULL );
        else{
            curr -> next = create_node ( res + '0', NULL );
            curr = curr -> next;
        }
        a = a -> next;
        b = b -> next;
    }
    
    TNODE * tmp = a ? a : b;
    while ( tmp ) {
        res = sum_digits ( tmp -> digit - '0', 0, &carry );
        curr -> next = create_node ( res + '0', NULL );
        curr = curr -> next;
        tmp = tmp -> next;
    }
    if ( carry ) 
        curr -> next = create_node ( carry + '0', NULL );
    return l;
}

TNODE * list_shift ( TNODE * n, unsigned int shift ) {
    TNODE * l = NULL;

    return l;
}

                                /*PRINTING FUNCTIONS*/
//----------------------------------------------------------------------------------//

void print_list ( TNODE * l ) {
    if ( !l ) printf("ERR: LIST IS EMPTY");
    while ( l ) {
        printf("%c -> ", l -> digit);
        l = l -> next;
    }
    printf("\n");
}


void print_num_rec ( TNODE * l, int first ) {
    if ( l -> next )
        print_num_rec ( l -> next, 0 ); 
    printf("%c", l -> digit);
}

void print_num ( TNODE * l ) {
    if ( !l ) {
        printf("ERR: LIST IS EMPTY\n");
        return;
    }
    print_num_rec ( l, 1 );
    printf("\n");
}

                            /*TESTING METHODS*/
//----------------------------------------------------------------------------------//

void test ( const char * n1, const char * n2, const char * r ) {
    TNODE * a, *b, *c, *ref;

    a = parse_num ( n1 );
    b = parse_num ( n2 );
    c = list_sum ( a, b );
    ref = parse_num ( r );
   
    if ( !compare_lists ( ref, c ) ) {
        printf("--------------------\n");
        printf( "ERR: VALUE MISMATCH\n" );
        print_list ( a );
        print_list ( b );  
        printf("GOT: ");
        print_list ( c );
        print_num ( c );
        printf("REF: ");
        print_list ( ref );
        print_num ( ref );
        printf("--------------------\n");
    } else printf( "EQUAL: OK\n" );

    del_list ( a );
    del_list ( b );
    del_list ( c );
    del_list ( ref );
}

//----------------------------------------------------------------------------------//

int main ( void ) {
    
    test ( "0", "0", "0" );
    test ( "1", "1", "2" );
    test ( "0", "1", "1" );
    test ( "111", "9999", "10110" );
    test ( "5432", "29", "5461" );
    test ( "5432", "29", "5461" );
    test ( "2009", "991", "3000" );

    return 0;
}