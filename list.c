#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

//helper funcs
int get_min ( int a, int b ) {
    return a < b ? a : b;
}

typedef struct TNode {
    struct TNode * next;
    char digit;
} TNODE;

TNODE * create_node ( char digit, TNODE * next ) {
    TNODE * node = ( TNODE* ) malloc ( sizeof( TNODE ) );
    node -> next = next;
    node -> digit = digit;
    return node;
}

/**
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
    return l;
}

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
    return l;
}

int list_size ( TNODE * l ) {
    int size = 0;
    while ( l ) {
        size++;
        l = l -> next;
    }
    return size;
}

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

TNODE * list_sum ( TNODE * a, TNODE * b ) {
    TNODE * l = NULL, *curr = NULL;
    int n = get_min ( list_size ( a ), list_size ( b ) );
    int res = 0, carry = 0;
    while ( n-- ) {
        res = a -> digit - '0' + b -> digit - '0';
        if ( res >= 10 ) {
            carry = 1;
            res %= 10;
        }
        if ( !l )
            curr = l = create_node ( res + '0', NULL );
        else{
            curr -> next = create_node ( res + '0', NULL );
            curr = curr -> next;
        }
    }
    return l;
}

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
        printf("ERR: LIST IS EMPTY");
        return;
    }
    print_num_rec ( l, 1 );
    printf("\n");
}

void free_list ( TNODE * l ) {
    TNODE * tmp = NULL;
    while ( l ) {
        tmp = l -> next;
        free ( l );
        l = tmp;
    }
}

//TEST PROCEDURES
void are_equal ( TNODE * ref, TNODE * actual ) {
    if ( !compare_lists ( ref, actual ) )
        printf( "ERR: VALUE MISMATCH\n" );
    else printf( "EQUAL: OK\n" );
}

int main ( void ) {
    TNODE * a = parse_list ( "1234" );
    TNODE * b = parse_list ( "12      3    4" );
    TNODE * c = list_sum ( a, b );
    print_list ( a );
    print_list ( b );
    print_num ( a );
    print_num ( b );

    free_list ( a );
    free_list ( b );
    free_list ( c );
    return 0;
}