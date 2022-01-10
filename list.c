#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

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

int list_size ( TNODE * l ) {
    int size = 0;
    while ( l ) {
        size++;
        l = l -> next;
    }
    return size;
}

int compare_lists ( TNODE * ref, TNODE * actual ) {
    if ( list_size ( ref ) != list_size ( actual ) )
        return 0;
    
    return 1;
}

void print_list ( TNODE * l ) {
    if ( !list_size( l ) ) printf("ERR: LIST IS EMPTY");
    while ( l ) {
        printf("%c -> ", l -> digit);
        l = l -> next;
    }
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

int main ( void ) {
    TNODE * a = parse_list ( "5 -> 2 -> 3 -> 4" );
    TNODE *b = parse_list ( "9 5 4 7" );
    print_list ( a );
    print_list ( b );
    return 0;
}