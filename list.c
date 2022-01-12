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

char get_digit ( int a ) {
    if ( a < 10 )
        return a + '0';
    return a - 10 + 'a';
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
    int base = 1, val = 0, carry = 0;
    while ( n ) {
        n = n -> next;
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

void test ( const char * n1, const char * n2, const char * r ) {
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
    } 
    del_list ( a );
    del_list ( b );
    del_list ( c );
    del_list ( ref );
}

//----------------------------------------------------------------------------------//

int main ( void ) {
    TNODE * a = parse_num ( "1af" );
    TNODE * r = list_lshift ( a, 1 );
    
    print_num ( a );
    print_list ( a );
    print_num ( r );
    print_list ( r );
    
    del_list ( a );
    del_list ( r );
    //List sum test
  /*test ( "0", "0", "0" );
    test ( "1", "1", "2" );
    test ( "0", "1", "1" );
    test ( "111", "9999", "10110" );
    test ( "5432", "29", "5461" );
    test ( "5432", "29", "5461" );
    test ( "2009", "991", "3000" );
    test ( "1488191320", "326357850", "1814549170" );
    test ( "971428818", "820982117", "1792410935" );
    test ( "991941969", "1804105222", "2796047191" );
    test ( "953703380", "2040018155", "2993721535" );
    test ( "584176765", "1925753716", "2509930481" );
    test ( "1710039910", "868489816", "2578529726" );
    test ( "644639036", "882343256", "1526982292" );
    test ( "1995728193", "128970477", "2124698670" );
    test ( "1009524844", "1044203630", "2053728474" );
    test ( "910916150", "45449341", "956365491" );
    test ( "169359070", "1066189806", "1235548876" );
    test ( "621351808", "1670916428", "2292268236" );
    test ( "428298880", "1040369171", "1468668051" );
    test ( "11702388", "1543763997", "1555466385" );
    test ( "1396775447", "460195851", "1856971298" );
    test ( "351761060", "737483120", "1089244180" );
    test ( "786553701", "1323189878", "2109743579" );
    test ( "1558465237", "1778495670", "3336960907" );
    test ( "979811452", "364684970", "1344496422" );
    test ( "1671030177", "1563988217", "3235018394" );
    test ( "142955038", "1233586440", "1376541478" );
    test ( "284994386", "787594074", "1072588460" );
    test ( "2115929696", "133238931", "2249168627" );
    test ( "916564552", "977970892", "1894535444" );
    test ( "1177442561", "1827480702", "3004923263" );
    test ( "1023420233", "1346801632", "2370221865" );
    test ( "746186860", "1644772041", "2390958901" );
    test ( "870234412", "1174485740", "2044720152" );
    test ( "537657565", "881936801", "1419594366" );
    test ( "570766089", "1934433012", "2505199101" );
    test ( "1342132652", "922527150", "2264659802" );
    test ( "524432484", "2128686353", "2653118837" );
    test ( "98233380", "2082897722", "2181131102" );
    test ( "1759698375", "1078044833", "2837743208" );
    test ( "300099044", "1283244905", "1583343949" );
    test ( "494549402", "443054082", "937603484" );
    test ( "369347697", "779543788", "1148891485" );
    test ( "1230648156", "337793745", "1568441901" );
    test ( "912782720", "2147212708", "3059995428" );
    test ( "1315764638", "2090225281", "3405989919" );
    test ( "1827209762", "191701223", "2018910985" );
    test ( "1289543265", "425912975", "1715456240" );
    test ( "1836473265", "12294030", "1848767295" );
    test ( "1600398715", "226647182", "1827045897" );
    test ( "894230831", "23681157", "917911988" );
    test ( "13596546", "88879835", "102476381" );
    test ( "946208307", "538029031", "1484237338" );
    test ( "70082540", "1044441687", "1114524227" );
    test ( "473443105", "1829780915", "2303224020" );
    test ( "2122486520", "773542149", "2896028669" );
    test ( "965542172", "469552275", "1435094447" );
    test ( "1216596231", "1334889869", "2551486100" );
    test ( "1249096063", "299760739", "1548856802" );
    test ( "1672683615", "14395135", "1687078750" );
    test ( "299489800", "840964605", "1140454405" );
    test ( "2104620417", "2126699562", "4231319979" );
    test ( "1032665828", "1246680034", "2279345862" );
    test ( "405128889", "721655445", "1126784334" );
    test ( "1258974064", "2005527605", "3264501669" );
    test ( "948302627", "5721247", "954023874" );
    test ( "2029208762", "961899174", "2991107936" );
    test ( "94601082", "827933421", "922534503" );
    test ( "1499928205", "164683622", "1664611827" );
    test ( "1872375108", "1973371310", "3845746418" );
    test ( "1994464538", "1847377981", "3841842519" );
    test ( "599429811", "812523062", "1411952873" );
    test ( "169446608", "1816026042", "1985472650" );
    test ( "2147412932", "1418542671", "3565955603" );
    test ( "2115786781", "1672612899", "3788399680" );
    test ( "1432937807", "267792933", "1700730740" );
    test ( "366093856", "1390074576", "1756168432" );
    test ( "247008848", "1398759684", "1645768532" );
    test ( "489270962", "652137737", "1141408699" );
    test ( "2120415130", "1748245027", "3868660157" );
    test ( "510181694", "921234109", "1431415803" );
    test ( "1753966274", "391906808", "2145873082" );
    test ( "1883133283", "1848567357", "3731700640" );
    test ( "1219840229", "1235577840", "2455418069" );
    test ( "2013250979", "944731690", "2957982669" );
    test ( "1061465502", "1860231869", "2921697371" );
    test ( "644626023", "1660895313", "2305521336" );
    test ( "525271284", "814072631", "1339343915" );
    test ( "1329437707", "525200568", "1854638275" );
    test ( "85131654", "1297740841", "1382872495" );
    test ( "50329819", "1518069461", "1568399280" );
    test ( "1565533774", "416423675", "1981957449" );
    test ( "760660389", "1812542622", "2573203011" );
    test ( "1815183359", "1249931352", "3065114711" );
    test ( "317196712", "1788114841", "2105311553" );
    test ( "850692731", "827378406", "1678071137" );
    test ( "561865303", "457175357", "1019040660" );
    test ( "1219285215", "297514938", "1516800153" );
    test ( "158259066", "291641796", "449900862" );
    test ( "1533092779", "24026398", "1557119177" );
    test ( "1236373486", "447074633", "1683448119" );
    test ( "1884258267", "1880999509", "3765257776" );
    test ( "2107969947", "262045903", "2370015850" );
    test ( "547588492", "1289924006", "1837512498" );
    test ( "787246471", "632720147", "1419966618" );
    test ( "440181199", "837576290", "1277757489" );*/


    return 0;
}