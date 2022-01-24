#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

//QUICK DIRTY SETUP. THIS IS DUMB. VERY DUMB. DONT DO THIS!
int g_OK = 0;
int g_FAIL = 0;

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

int from_hex ( char digit ) {
    if ( isdigit( digit ) )
        return digit - '0';
    return digit - 'a' + 10;
}

char to_hex ( int digit ) {
    if ( digit < 10 )
        return digit + '0';
    return digit - 10 + 'a';
}

                                /*LIST FUNCTIONS*/
//----------------------------------------------------------------------------------//

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
        if ( isdigit ( *str ) || ( *str >= 'a' && *str <= 'f' ) ) {
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

/***
 * Treats all numbers as hexadecimal numbers.
 * @param n Pointer to a linked list head
 * @param shift Number of bits to shift
 * @return Pointer to the head of a linked list representing the bit shifted number
 */
TNODE * list_lshift ( TNODE * n, unsigned int shift ) {

    TNODE * l = NULL, *curr = NULL;
    long long int val = 0, carry = 0, remain = 0;
    while ( n ) {
        val = from_hex ( n -> digit );
        val = ( val << shift ) + carry;
        carry = val / 16;
        remain = val % 16;
        if ( !l )
            curr = l = create_node ( to_hex ( remain ), NULL );
        else {
            curr -> next = create_node ( to_hex ( remain ), NULL );
            curr = curr -> next;
        }
        n = n -> next;
    }
    while ( carry ) {
        curr -> next = create_node ( to_hex ( carry % 16 ), NULL );
        curr = curr -> next;
        carry /= 16;
    }
    return l;
}

                                /*PRINTING FUNCTIONS*/
//----------------------------------------------------------------------------------//

void print_list ( TNODE * l ) {
    if ( !l ) printf("<EMPTY LIST>");
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
        printf("<EMPTY LIST>\n");
        return;
    }
    print_num_rec ( l, 1 );
    printf("\n");
}

                            /*TESTING METHODS*/
//----------------------------------------------------------------------------------//

void print_results ( void ) {
    printf("RESULTS:\nOK:%d\nFAIL:%d\n", g_OK, g_FAIL);
    g_OK = 0; g_FAIL = 0;
}

void test_sum ( const char * n1, const char * n2, const char * r ) {
    TNODE * a, *b, *c, *ref;

    a = parse_num ( n1 );
    b = parse_num ( n2 );
    c = list_sum ( a, b );
    ref = parse_num ( r );
   
    if ( !compare_lists ( ref, c ) ) {
        printf("--------------------\n");
        printf( "ERR: VALUE MISMATCH\n" );
        printf("GOT: ");
        print_list ( c );
        printf("REF: ");
        print_list ( ref );
        printf("--------------------\n");
        g_FAIL++;
    } 

    g_OK++;

    del_list ( a );
    del_list ( b );
    del_list ( c );
    del_list ( ref );
}

void test_shift ( const char * n1, unsigned int shift, const char * r ) {
    TNODE * a, *b, *ref;

    a = parse_num ( n1 );
    b = list_lshift ( a, shift );
    ref = parse_num ( r );
   
    if ( !compare_lists ( ref, b ) ) {
        printf("--------------------\n");
        printf( "ERR: VALUE MISMATCH\n" );
        printf("GOT: ");
        print_list ( b );
        printf("REF: ");
        print_list ( ref );
        printf("--------------------\n");
        g_FAIL++;
    } 

    g_OK++;

    del_list ( a );
    del_list ( b );
    del_list ( ref );
}

//----------------------------------------------------------------------------------//

int main ( void ) {
    
    test_shift ( "6b56f85e", 8, "6b56f85e00" );
    test_shift ( "4c65780b", 6, "13195e02c0" );
    test_shift ( "b685b32", 5, "16d0b6640" );
    test_shift ( "5ff7c649", 19, "2ffbe32480000" );
    test_shift ( "7d60f052", 14, "1f583c148000" );
    test_shift ( "1b02f8ef", 18, "6c0be3bc0000" );
    test_shift ( "15218ad4", 13, "2a4315a8000" );
    test_shift ( "125aa27", 2, "496a89c" );
    test_shift ( "44d85eca", 10, "113617b2800" );
    test_shift ( "4e2f1548", 11, "27178aa4000" );
    test_shift ( "4834ee40", 8, "4834ee4000" );
    test_shift ( "2f2254a1", 18, "bc8952840000" );
    test_shift ( "18e68ee8", 18, "639a3ba00000" );
    test_shift ( "60f4cc79", 0, "60f4cc79" );
    test_shift ( "69d303af", 19, "34e981d780000" );
    test_shift ( "65d6a857", 1, "cbad50ae" );
    test_shift ( "2d5fd98", 10, "b57f66000" );
    test_shift ( "576abf40", 6, "15daafd000" );
    test_shift ( "451c7a1f", 13, "8a38f43e000" );
    test_shift ( "6b9c60c9", 6, "1ae7183240" );
    test_shift ( "5fd73dfc", 8, "5fd73dfc00" );
    test_shift ( "5a58b5c", 12, "5a58b5c000" );
    test_shift ( "1c4dd1b1", 4, "1c4dd1b10" );
    test_shift ( "5987962", 12, "5987962000" );
    test_shift ( "79160be6", 14, "1e4582f98000" );

    test_sum ( "1478488643", "909165219", "2387653862" );
    test_sum ( "156825012", "1867485019", "2024310031" );
    test_sum ( "287997204", "1702106722", "1990103926" );
    test_sum ( "1827523588", "1300770670", "3128294258" );
    test_sum ( "56698276", "875354288", "932052564" );
    test_sum ( "722193058", "1828264209", "2550457267" );
    test_sum ( "725264878", "78830385", "804095263" );
    test_sum ( "649615824", "891656784", "1541272608" );
    test_sum ( "1526732041", "1721357656", "3248089697" );
    test_sum ( "1267188419", "1039199399", "2306387818" );
    test_sum ( "741042443", "255870837", "996913280" );
    test_sum ( "541593340", "311177332", "852770672" );
    test_sum ( "1991318608", "925882169", "2917200777" );
    test_sum ( "927139023", "254275513", "1181414536" );
    test_sum ( "361631392", "974402731", "1336034123" );
    test_sum ( "1297575088", "1840120035", "3137695123" );
    test_sum ( "1883567950", "1454400101", "3337968051" );
    test_sum ( "1560121406", "24081506", "1584202912" );
    test_sum ( "1009023175", "1240161346", "2249184521" );
    test_sum ( "1324852176", "1065721451", "2390573627" );
    test_sum ( "2115515635", "2047045234", "4162560869" );
    test_sum ( "746502012", "693296865", "1439798877" );
    test_sum ( "2125875619", "1396117836", "3521993455" );
    test_sum ( "1584953649", "1505124012", "3090077661" );
    test_sum ( "969991844", "704658420", "1674650264" );
    test_sum ( "4568798956579798", "7568784512365445", "12137583468945243" );
    test_sum (  "4549874984651651354654968498498498498451614724376857948448798794598798798798798798798784543216549699", 
                "1437243726759715795179621764276547164574675465764576546754676654594574747564576516712671267216712671", 
                "5987118711411367149834590262775045663026290190141434495203475449193373546363375315511455810433262370" );

    print_results();
    return 0;
}