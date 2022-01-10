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

int compare_lists ( TNODE * ref, TNODE * actual ) {
    if ( list_size ( ref ) != list_size ( actual ) )
        return 0;
    
    return 1;
}

TNODE * list_sum ( TNODE * a, TNODE * b ) {
    TNODE * l = NULL;
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

int main ( void ) {
    TNODE * a = parse_list ( "5234" );
    TNODE *b = parse_num ( "9547" );
    print_list ( a );
    print_list ( b );
    print_num ( a );
    print_num ( b );
    return 0;
}