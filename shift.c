#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

//QUICK DIRTY SETUP. THIS IS DUMB. VERY DUMB. DONT DO THIS!
int g_OK = 0;
int g_FAIL = 0;

                                /*HELPER FUNCTIONS*/
//----------------------------------------------------------------------------------//

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
void print_binary ( int * arr, int len );

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

                                /*PARSING FUNCTIONS*/
//----------------------------------------------------------------------------------//

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
    return l;
}
                                /*LIST OPERATIONS*/
//----------------------------------------------------------------------------------//

void to_bin ( int * arr, int size, int digit, int pos ) {
    int pow = 8;
    int start = size - pos * 4;
    for ( int i = start; pow; i++ ) {
        arr[i] = digit / pow;
        digit %= pow;
        pow /= 2;
    }
}

void shift_bin ( int * arr, int size, unsigned shift ) {
    for ( int i = 0; i < size; i++ ) {
        if ( arr[i] ) {
            arr[i - shift] = 1;
            arr[i] = 0;
        }
    }
}

TNODE * bin_to_list ( int * arr, int size, int num_len ) {
    TNODE * head = NULL;
    int in_zeroes = 1;
    int digit = 0, pow = 8, block = 0;
    for ( int i = 0; i < size; i++ ) {
        if ( arr[i] )
            in_zeroes = 0;
        digit += pow * arr[i];
        pow /= 2;
        if ( block++ == 3 ) {
            if ( !in_zeroes )
                head = create_node ( to_hex ( digit ), head );
            block = digit = 0;
            pow = 8;
        }
    }
    return head;
}

TNODE * list_lshift ( TNODE * n, unsigned int shift ) {
    int num_len = list_size ( n );
    int size = ( num_len + shift ) * 4;
    int * arr = ( int* ) calloc ( size , sizeof( *arr ) );
    int i = 0;
    while ( n ) {
        to_bin ( arr, size, from_hex ( n -> digit ), i + 1 );
        n = n -> next;
        i++;
    }
    shift_bin ( arr, size, shift );
    TNODE * head = bin_to_list ( arr, size, num_len );
    free ( arr );    
    return head;
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

void print_binary ( int * bin, int len ) {
    for ( int i = 0; i < len; i++ ) 
        printf( "%d", bin[i] );
    printf("\n");
}

                            /*TESTING METHODS*/
//----------------------------------------------------------------------------------//

void print_results ( void ) {
    printf("RESULTS:\nOK:%d\nFAIL:%d\n", g_OK, g_FAIL);
    g_OK = 0; g_FAIL = 0;
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
    } else g_OK++;

    del_list ( a );
    del_list ( b );
    del_list ( ref );
}

//----------------------------------------------------------------------------------//

int main ( void ) {

    test_shift ( "59cdeeb3", 4, "59cdeeb30" );
    test_shift ( "4bc36037", 30, "12f0d80dc0000000" );
    test_shift ( "19d5b7c9", 7, "ceadbe480" );
    test_shift ( "530463a2", 1, "a608c744" );
    test_shift ( "c5adbac", 18, "316b6eb00000" );
    test_shift ( "3459d3eb", 7, "1a2ce9f580" );
    test_shift ( "58ecc225", 32, "58ecc22500000000" );
    test_shift ( "4cef1d39", 26, "133bc74e4000000" );
    test_shift ( "59680555", 14, "165a01554000" );
    test_shift ( "8920f9b", 9, "11241f3600" );
    test_shift ( "50e2cbbb", 7, "287165dd80" );
    test_shift ( "217c8b99", 19, "10be45cc80000" );
    test_shift ( "7db66ddc", 7, "3edb36ee00" );
    test_shift ( "6b1cd1d7", 20, "6b1cd1d700000" );
    test_shift ( "4879179c", 24, "4879179c000000" );
    test_shift ( "3abc8db5", 16, "3abc8db50000" );
    test_shift ( "4e653dfb", 13, "9cca7bf6000" );
    test_shift ( "11c643ad", 5, "238c875a0" );
    test_shift ( "20ebb8b3", 16, "20ebb8b30000" );
    test_shift ( "6a6d3564", 32, "6a6d356400000000" );
    test_shift ( "2cb82be0", 16, "2cb82be00000" );
    test_shift ( "dd72e26", 6, "375cb8980" );
    test_shift ( "21628b8e", 32, "21628b8e00000000" );
    test_shift ( "39b39ede", 4, "39b39ede0" );
    test_shift ( "79f88e8c", 27, "3cfc47460000000" );
    test_shift ( "3669306c", 8, "3669306c00" );
    test_shift ( "56e2af60", 6, "15b8abd800" );
    test_shift ( "46a16c1a", 29, "8d42d8340000000" );
    test_shift ( "553914ab", 18, "154e452ac0000" );
    test_shift ( "2ee91d6f", 8, "2ee91d6f00" );
    test_shift ( "43720b68", 5, "86e416d00" );
    test_shift ( "3ff93297", 4, "3ff932970" );
    test_shift ( "70259911", 5, "e04b32220" );
    test_shift ( "7a123f27", 5, "f4247e4e0" );
    test_shift ( "368a1d93", 13, "6d143b26000" );
    test_shift ( "3e57e623", 21, "7cafcc4600000" );
    test_shift ( "3467ddb", 10, "d19f76c00" );
    test_shift ( "68e73779", 11, "34739bbc800" );
    test_shift ( "26f55fa8", 25, "4deabf50000000" );
    test_shift ( "1f739a4d", 22, "7dce693400000" );
    test_shift ( "64e084d2", 26, "193821348000000" );
    test_shift ( "6bc9487c", 19, "35e4a43e00000" );
    test_shift ( "2dc286be", 23, "16e1435f000000" );
    test_shift ( "105c516e", 10, "417145b800" );
    test_shift ( "6428f287", 30, "190a3ca1c0000000" );
    test_shift ( "20adc7b1", 17, "415b8f620000" );
    test_shift ( "28eb1a01", 9, "51d6340200" );
    test_shift ( "39724753", 19, "1cb923a980000" );
    test_shift ( "3266708c", 28, "3266708c0000000" );
    test_shift ( "2a2204d6", 32, "2a2204d600000000" );
    test_shift ( "1803fb07", 27, "c01fd838000000" );
    test_shift ( "4c32d793", 3, "26196bc98" );
    test_shift ( "34a8ff43", 14, "d2a3fd0c000" );
    test_shift ( "3ff3824c", 13, "7fe70498000" );
    test_shift ( "57b4e565", 27, "2bda72b28000000" );
    test_shift ( "7c8c4d21", 24, "7c8c4d21000000" );
    test_shift ( "3543e753", 30, "d50f9d4c0000000" );
    test_shift ( "78589e69", 19, "3c2c4f3480000" );
    test_shift ( "1ac04a34", 24, "1ac04a34000000" );
    test_shift ( "6602097b", 28, "6602097b0000000" );
    test_shift ( "47fa5eb4", 13, "8ff4bd68000" );
    test_shift ( "26843aab", 22, "9a10eaac00000" );
    test_shift ( "6756cb74", 32, "6756cb7400000000" );
    test_shift ( "9f62bc8", 1, "13ec5790" );
    test_shift ( "137b087a", 31, "9bd843d00000000" );
    test_shift ( "2adca20", 2, "ab72880" );
    test_shift ( "1c921b98", 20, "1c921b9800000" );
    test_shift ( "46c97c63", 29, "8d92f8c60000000" );
    test_shift ( "3fab0c0", 17, "7f561800000" );
    test_shift ( "2cd979c8", 6, "b365e7200" );
    test_shift ( "66241b4a", 10, "198906d2800" );
    test_shift ( "1845005c", 30, "611401700000000" );
    test_shift ( "11bb5c86", 6, "46ed72180" );
    test_shift ( "7e6e70af", 27, "3f3738578000000" );
    test_shift ( "19528e9c", 12, "19528e9c000" );
    test_shift ( "2b64e376", 17, "56c9c6ec0000" );
    test_shift ( "6b204b57", 2, "1ac812d5c" );
    test_shift ( "52326606", 13, "a464cc0c000" );
    test_shift ( "31dfa020", 15, "18efd0100000" );
    test_shift ( "6c3452cc", 27, "361a29660000000" );
    test_shift ( "1040c26c", 13, "208184d8000" );
    test_shift ( "70d9ac1b", 6, "1c366b06c0" );
    test_shift ( "3dc2bea0", 32, "3dc2bea000000000" );
    test_shift ( "7e0df8e1", 1, "fc1bf1c2" );
    test_shift ( "3e60272f", 10, "f9809cbc00" );
    test_shift ( "1d6d0586", 26, "75b41618000000" );
    test_shift ( "544d3992", 3, "2a269cc90" );
    test_shift ( "3fec4516", 25, "7fd88a2c000000" );
    test_shift ( "464fa4a8", 7, "2327d25400" );
    test_shift ( "12843cd3", 5, "250879a60" );
    test_shift ( "22cb2ff1", 10, "8b2cbfc400" );
    test_shift ( "40ef2095", 9, "81de412a00" );
    test_shift ( "fd23e6a", 28, "fd23e6a0000000" );
    test_shift ( "60629214", 12, "60629214000" );
    test_shift ( "6f4a1869", 2, "1bd2861a4" );
    test_shift ( "70c8725", 22, "1c321c9400000" );
    test_shift ( "3b7901cd", 2, "ede40734" );
    test_shift ( "2c5db8da", 14, "b176e368000" );
    test_shift ( "2f895bc0", 28, "2f895bc00000000" );
    test_shift ( "3af92fce", 16, "3af92fce0000" );
    test_shift ( "55532464", 21, "aaa648c800000" );
    test_shift ( "126dc568", 23, "936e2b4000000" );
    test_shift ( "e183b36", 31, "70c1d9b00000000" );
    test_shift ( "fa8f40f", 31, "7d47a0780000000" );
    test_shift ( "10b4c044", 3, "85a60220" );
    test_shift ( "340a1323", 22, "d0284c8c00000" );
    test_shift ( "6016512b", 25, "c02ca256000000" );
    test_shift ( "416b6b7e", 22, "105adadf800000" );
    test_shift ( "81aba53", 19, "40d5d2980000" );
    test_shift ( "319a6e20", 29, "6334dc400000000" );
    test_shift ( "6e64e273", 7, "3732713980" );
    test_shift ( "76efad91", 2, "1dbbeb644" );
    test_shift ( "308d2887", 13, "611a510e000" );
    test_shift ( "2319a4ae", 8, "2319a4ae00" );
    test_shift ( "4db91827", 29, "9b72304e0000000" );
    test_shift ( "4e000736", 13, "9c000e6c000" );
    test_shift ( "367909d0", 31, "1b3c84e800000000" );
    test_shift ( "4cac9a82", 8, "4cac9a8200" );
    test_shift ( "32c7d71c", 18, "cb1f5c700000" );
    test_shift ( "18f924e5", 2, "63e49394" );
    test_shift ( "7e82bf73", 9, "fd057ee600" );
    test_shift ( "76fc8796", 31, "3b7e43cb00000000" );
    test_shift ( "41fc6bc0", 21, "83f8d78000000" );
    test_shift ( "4e81a034", 20, "4e81a03400000" );
    test_shift ( "5a35f0a7", 21, "b46be14e00000" );
    test_shift ( "417bf8cf", 27, "20bdfc678000000" );
    test_shift ( "38c3039a", 2, "e30c0e68" );
    test_shift ( "7278b75b", 2, "1c9e2dd6c" );
    test_shift ( "5bb90ccd", 11, "2ddc8666800" );
    test_shift ( "4966b068", 21, "92cd60d000000" );
    test_shift ( "73a53086", 31, "39d2984300000000" );
    test_shift ( "4c7e6180", 24, "4c7e6180000000" );
    test_shift ( "77d4843c", 4, "77d4843c0" );
    test_shift ( "6eaf7f5e", 26, "1babdfd78000000" );
    test_shift ( "75808a94", 4, "75808a940" );
    test_shift ( "6e18f2ba", 9, "dc31e57400" );
    test_shift ( "54abdc4c", 17, "a957b8980000" );
    test_shift ( "529c5aa6", 13, "a538b54c000" );
    test_shift ( "1d7d6d65", 27, "ebeb6b28000000" );
    test_shift ( "60bf6e20", 13, "c17edc40000" );
    test_shift ( "213a092f", 16, "213a092f0000" );
    test_shift ( "404e3126", 10, "10138c49800" );
    test_shift ( "5aa70d50", 3, "2d5386a80" );
    test_shift ( "434d38eb", 30, "10d34e3ac0000000" );
    test_shift ( "4859448c", 20, "4859448c00000" );
    test_shift ( "5fd23f12", 19, "2fe91f8900000" );
    test_shift ( "241aa0f2", 19, "120d507900000" );
    test_shift ( "661caf69", 15, "330e57b48000" );
    test_shift ( "457b9c95", 8, "457b9c9500" );
    test_shift ( "468b8087", 11, "2345c043800" );
    test_shift ( "5c74d30b", 2, "171d34c2c" );
    test_shift ( "2eff7132", 24, "2eff7132000000" );
    test_shift ( "19b9ed92", 25, "3373db24000000" );
    test_shift ( "47c8f763", 24, "47c8f763000000" );
    test_shift ( "22b9c6b3", 4, "22b9c6b30" );
    test_shift ( "2eeab903", 3, "17755c818" );
    test_shift ( "4ac3cc73", 10, "12b0f31cc00" );
    test_shift ( "1df0dcab", 4, "1df0dcab0" );
    test_shift ( "21ffd2ab", 23, "10ffe955800000" );
    test_shift ( "5bcaf3e1", 24, "5bcaf3e1000000" );
    test_shift ( "6df1feea", 20, "6df1feea00000" );
    test_shift ( "26578c4a", 29, "4caf18940000000" );
    test_shift ( "67edd385", 20, "67edd38500000" );
    test_shift ( "2dfbc50a", 28, "2dfbc50a0000000" );
    test_shift ( "61406a7b", 19, "30a0353d80000" );
    test_shift ( "68659745", 7, "3432cba280" );
    test_shift ( "292bb8d3", 24, "292bb8d3000000" );
    test_shift ( "6ed5ecdd", 6, "1bb57b3740" );
    test_shift ( "1900d44f", 1, "3201a89e" );
    test_shift ( "7a1d015c", 3, "3d0e80ae0" );
    test_shift ( "5f2749c4", 32, "5f2749c400000000" );
    test_shift ( "7fae6ae4", 24, "7fae6ae4000000" );
    test_shift ( "1840a489", 17, "308149120000" );
    test_shift ( "4f55effa", 21, "9eabdff400000" );
    test_shift ( "7edd5d26", 28, "7edd5d260000000" );
    test_shift ( "24998e6c", 17, "49331cd80000" );
    test_shift ( "66be16ce", 23, "335f0b67000000" );
    test_shift ( "7edbfbec", 21, "fdb7f7d800000" );
    test_shift ( "57655669", 24, "57655669000000" );
    test_shift ( "7c155a6f", 5, "f82ab4de0" );
    test_shift ( "215f0689", 21, "42be0d1200000" );
    test_shift ( "765afe6a", 29, "ecb5fcd40000000" );
    test_shift ( "7bdffa2b", 8, "7bdffa2b00" );
    test_shift ( "d7065c2", 27, "6b832e10000000" );
    test_shift ( "1bcfcf88", 31, "de7e7c400000000" );
    test_shift ( "509b697d", 13, "a136d2fa000" );
    test_shift ( "3095217e", 2, "c25485f8" );
    test_shift ( "24e22f83", 8, "24e22f8300" );
    test_shift ( "6de91bf1", 30, "1b7a46fc40000000" );
    test_shift ( "3163d3b", 25, "62c7a76000000" );
    test_shift ( "1f590359", 8, "1f59035900" );
    test_shift ( "5995d528", 8, "5995d52800" );
    test_shift ( "72a0e65d", 22, "1ca83997400000" );
    test_shift ( "54c3047b", 8, "54c3047b00" );
    test_shift ( "549920c", 11, "2a4c906000" );
    test_shift ( "2abfdab", 22, "aaff6ac00000" );
    test_shift ( "3553509e", 23, "1aa9a84f000000" );
    test_shift ( "713357f1", 10, "1c4cd5fc400" );
    test_shift ( "5e37e75d", 20, "5e37e75d00000" );
    test_shift ( "46141944", 6, "1185065100" );
    test_shift ( "63124d2", 2, "18c49348" );
    test_shift ( "74fed031", 17, "e9fda0620000" );
    test_shift ( "66f95722", 21, "cdf2ae4400000" );
    test_shift ( "7f9c0c57", 21, "ff3818ae00000" );
    test_shift ( "e191f32", 19, "70c8f9900000" );
    test_shift ( "41a8ec2c", 12, "41a8ec2c000" );
    test_shift ( "2a62153a", 21, "54c42a7400000" );
    test_shift ( "33893cb2", 24, "33893cb2000000" );
    test_shift ( "73b09269", 15, "39d849348000" );
    test_shift ( "6709385e", 22, "19c24e17800000" );
    test_shift ( "5924a018", 11, "2c92500c000" );
    test_shift ( "1fa2bd0a", 24, "1fa2bd0a000000" );
    test_shift ( "62bc3220", 29, "c57864400000000" );
    test_shift ( "3fab3b80", 30, "feacee000000000" );
    test_shift ( "f79d2af", 5, "1ef3a55e0" );
    test_shift ( "3afbd063", 2, "ebef418c" );
    test_shift ( "1c6ed785", 21, "38ddaf0a00000" );
    test_shift ( "4c713dd1", 9, "98e27ba200" );
    test_shift ( "49dba049", 9, "93b7409200" );
    test_shift ( "584aa1bc", 28, "584aa1bc0000000" );
    test_shift ( "4ebf93bb", 9, "9d7f277600" );
    test_shift ( "566e206", 22, "159b881800000" );
    test_shift ( "35324f3c", 25, "6a649e78000000" );
    test_shift ( "1624a48c", 7, "b12524600" );
    test_shift ( "413c5fe6", 12, "413c5fe6000" );
    test_shift ( "21dd061b", 32, "21dd061b00000000" );
    test_shift ( "66e312f5", 7, "3371897a80" );
    test_shift ( "28d8f0b6", 22, "a363c2d800000" );
    test_shift ( "5255d822", 23, "292aec11000000" );
    test_shift ( "a935e92", 18, "2a4d7a480000" );
    test_shift ( "6e4380fb", 22, "1b90e03ec00000" );
    test_shift ( "777aa253", 1, "eef544a6" );
    test_shift ( "7589cf8a", 5, "eb139f140" );
    test_shift ( "e1a8728", 20, "e1a872800000" );
    test_shift ( "ff92a4d", 5, "1ff2549a0" );
    test_shift ( "175a2f4e", 9, "2eb45e9c00" );
    test_shift ( "58b6cc", 21, "b16d9800000" );
    test_shift ( "57da66fd", 10, "15f699bf400" );
    test_shift ( "55b1300c", 10, "156c4c03000" );
    test_shift ( "5e6de7af", 20, "5e6de7af00000" );
    test_shift ( "6b2ce874", 11, "3596743a000" );
    test_shift ( "314e8ff2", 10, "c53a3fc800" );
    test_shift ( "41cab0f0", 9, "839561e000" );
    test_shift ( "1baf407e", 19, "dd7a03f00000" );
    test_shift ( "42abacde", 18, "10aaeb3780000" );
    test_shift ( "75f033e4", 26, "1d7c0cf90000000" );
    test_shift ( "6bd1ce06", 24, "6bd1ce06000000" );
    test_shift ( "3ba1865a", 17, "77430cb40000" );
    test_shift ( "3156b65b", 3, "18ab5b2d8" );
    test_shift ( "20c10d63", 9, "41821ac600" );
    test_shift ( "30213667", 18, "c084d99c0000" );
    test_shift ( "20089eb0", 20, "20089eb000000" );
    test_shift ( "54dc4e05", 14, "153713814000" );
    test_shift ( "6604f33c", 19, "3302799e00000" );
    test_shift ( "65e21137", 12, "65e21137000" );
    test_shift ( "417b0e05", 12, "417b0e05000" );
    test_shift ( "44bdc8b6", 24, "44bdc8b6000000" );
    test_shift ( "231ef515", 7, "118f7a8a80" );
    test_shift ( "4cf11e9f", 20, "4cf11e9f00000" );
    test_shift ( "1aa902b9", 30, "6aa40ae40000000" );
    test_shift ( "6510d4a4", 30, "1944352900000000" );
    test_shift ( "408bf957", 12, "408bf957000" );
    test_shift ( "7e040cd4", 18, "1f81033500000" );
    test_shift ( "323e403b", 16, "323e403b0000" );
    test_shift ( "45e98d33", 32, "45e98d3300000000" );
    test_shift ( "70aaa3d8", 27, "385551ec0000000" );
    test_shift ( "b1a8a50", 9, "163514a000" );
    test_shift ( "2684b0ce", 23, "13425867000000" );
    test_shift ( "8964836", 11, "44b241b000" );
    test_shift ( "a845668", 14, "2a1159a0000" );
    test_shift ( "50fc7ef6", 14, "143f1fbd8000" );
    test_shift ( "3f875318", 13, "7f0ea630000" );
    test_shift ( "3ec90264", 14, "fb240990000" );
    test_shift ( "1c42c152", 4, "1c42c1520" );
    test_shift ( "21747dc1", 12, "21747dc1000" );
    test_shift ( "1b56ec80", 6, "6d5bb2000" );
    test_shift ( "4784faa8", 24, "4784faa8000000" );
    test_shift ( "5767f510", 29, "aecfea200000000" );
    test_shift ( "58106588", 13, "b020cb10000" );
    test_shift ( "74e3caac", 29, "e9c795580000000" );
    test_shift ( "5ca582eb", 5, "b94b05d60" );
    test_shift ( "1404b656", 28, "1404b6560000000" );
    test_shift ( "7641b10c", 5, "ec8362180" );
    test_shift ( "47b6e591", 3, "23db72c88" );
    test_shift ( "47130b2f", 26, "11c4c2cbc000000" );
    test_shift ( "6d5052af", 6, "1b5414abc0" );
    test_shift ( "1e3aa066", 9, "3c7540cc00" );
    test_shift ( "2dc9d1d1", 11, "16e4e8e8800" );
    test_shift ( "f7dedf5", 4, "f7dedf50" );
    test_shift ( "68bdc3cd", 23, "345ee1e6800000" );
    test_shift ( "f8572f", 15, "7c2b978000" );
    test_shift ( "3777be1c", 24, "3777be1c000000" );
    test_shift ( "5ff79625", 13, "bfef2c4a000" );
    test_shift ( "e065954", 15, "7032caa0000" );
    test_shift ( "1885e878", 1, "310bd0f0" );
    test_shift ( "5601ee6a", 4, "5601ee6a0" );
    test_shift ( "68789284", 1, "d0f12508" );
    test_shift ( "5ceb789f", 17, "b9d6f13e0000" );
    test_shift ( "24900be5", 17, "492017ca0000" );
    test_shift ( "5d923cd3", 21, "bb2479a600000" );
    test_shift ( "76dd9a2a", 3, "3b6ecd150" );
    test_shift ( "3b2a139", 17, "76542720000" );
    test_shift ( "77ba354a", 11, "3bdd1aa5000" );
    test_shift ( "721bdd5b", 1, "e437bab6" );
    test_shift ( "7b89062d", 9, "f7120c5a00" );
    test_shift ( "382a8ef6", 29, "70551dec0000000" );
    test_shift ( "5eee5176", 19, "2f7728bb00000" );
    test_shift ( "44feaf34", 29, "89fd5e680000000" );
    test_shift ( "7e82003f", 9, "fd04007e00" );
    test_shift ( "76ede34a", 24, "76ede34a000000" );
    test_shift ( "55ef2c88", 21, "abde591000000" );
    test_shift ( "c33541b", 13, "1866a836000" );
    test_shift ( "36f67674", 27, "1b7b3b3a0000000" );
    test_shift ( "1d22029c", 26, "74880a70000000" );
    test_shift ( "dc12aea", 16, "dc12aea0000" );
    test_shift ( "4729996d", 21, "8e5332da00000" );
    test_shift ( "4596cef2", 7, "22cb677900" );
    test_shift ( "19b6ffa5", 29, "336dff4a0000000" );
    test_shift ( "7c58adb0", 1, "f8b15b60" );
    test_shift ( "4489277c", 31, "224493be00000000" );
    test_shift ( "66ac7e28", 20, "66ac7e2800000" );
    test_shift ( "7463113a", 32, "7463113a00000000" );
    test_shift ( "6c560385", 15, "362b01c28000" );
    test_shift ( "480b73b", 5, "9016e760" );
    test_shift ( "c66c8f6", 6, "319b23d80" );
    test_shift ( "1dfec7c", 31, "eff63e00000000" );
    test_shift ( "485e6c39", 24, "485e6c39000000" );
    test_shift ( "20bdb48a", 14, "82f6d228000" );
    test_shift ( "77320d51", 8, "77320d5100" );
    test_shift ( "5adb6507", 28, "5adb65070000000" );
    test_shift ( "3893f696", 21, "7127ed2c00000" );
    test_shift ( "991fd50", 9, "1323faa000" );
    test_shift ( "6ce1391b", 22, "1b384e46c00000" );
    test_shift ( "3b7bc9c5", 12, "3b7bc9c5000" );
    test_shift ( "2f1bd9f5", 2, "bc6f67d4" );
    test_shift ( "611b9aa9", 30, "1846e6aa40000000" );
    test_shift ( "7cb8a7b4", 5, "f9714f680" );
    test_shift ( "5b6327bc", 27, "2db193de0000000" );
    test_shift ( "ee06c52", 24, "ee06c52000000" );
    test_shift ( "53e6aefe", 10, "14f9abbf800" );
    test_shift ( "5b52797c", 27, "2da93cbe0000000" );
    test_shift ( "7d9d2ac7", 22, "1f674ab1c00000" );
    test_shift ( "63d9dc11", 19, "31ecee0880000" );
    test_shift ( "2305c863", 3, "1182e4318" );
    test_shift ( "5c3a9679", 11, "2e1d4b3c800" );
    test_shift ( "5fff219e", 16, "5fff219e0000" );
    test_shift ( "1fe62bde", 15, "ff315ef0000" );
    test_shift ( "12f95298", 26, "4be54a60000000" );
    test_shift ( "cda1be3", 30, "33686f8c0000000" );
    test_shift ( "76014bc5", 25, "ec02978a000000" );
    test_shift ( "4e7a0d9e", 15, "273d06cf0000" );
    test_shift ( "51be4df5", 20, "51be4df500000" );
    test_shift ( "2c9b9252", 19, "164dc92900000" );
    test_shift ( "3441608d", 6, "d10582340" );
    test_shift ( "d0554a9", 12, "d0554a9000" );
    test_shift ( "56c333ee", 7, "2b6199f700" );
    test_shift ( "5deeab06", 22, "177baac1800000" );
    test_shift ( "c08b3db", 24, "c08b3db000000" );
    test_shift ( "72bb3e07", 31, "395d9f0380000000" );
    test_shift ( "1cd4707a", 1, "39a8e0f4" );
    test_shift ( "2cefa9a8", 25, "59df5350000000" );
    test_shift ( "63c46190", 8, "63c4619000" );
    test_shift ( "6664b106", 9, "ccc9620c00" );
    test_shift ( "599d3a80", 10, "16674ea0000" );
    test_shift ( "4532d085", 6, "114cb42140" );
    test_shift ( "2f34c2c1", 4, "2f34c2c10" );
    test_shift ( "26bb6cb4", 23, "135db65a000000" );
    test_shift ( "5edf9376", 7, "2f6fc9bb00" );
    test_shift ( "2e148668", 4, "2e1486680" );
    test_shift ( "ed2fdab", 19, "7697ed580000" );
    test_shift ( "1b49038f", 26, "6d240e3c000000" );
    test_shift ( "2371a938", 22, "8dc6a4e000000" );
    test_shift ( "39f6904f", 20, "39f6904f00000" );
    test_shift ( "3b0035ac", 23, "1d801ad6000000" );
    test_shift ( "5e88d952", 7, "2f446ca900" );
    test_shift ( "7ba7a2d7", 27, "3dd3d16b8000000" );
    test_shift ( "54a8383e", 8, "54a8383e00" );
    test_shift ( "584e5881", 5, "b09cb1020" );
    test_shift ( "5629b88f", 3, "2b14dc478" );
    test_shift ( "2e4bb62d", 21, "5c976c5a00000" );
    test_shift ( "18a1947", 15, "c50ca38000" );
    test_shift ( "2f096737", 28, "2f0967370000000" );
    test_shift ( "5e7389a5", 15, "2f39c4d28000" );
    test_shift ( "7b9c8502", 14, "1ee721408000" );
    test_shift ( "2109eeb1", 14, "8427bac4000" );
    test_shift ( "47a1eb1f", 1, "8f43d63e" );
    test_shift ( "7005b447", 24, "7005b447000000" );
    test_shift ( "358aa0d5", 23, "1ac5506a800000" );
    test_shift ( "1a8df16b", 3, "d46f8b58" );
    test_shift ( "56ae12ec", 30, "15ab84bb00000000" );
    test_shift ( "485f7ca8", 4, "485f7ca80" );
    test_shift ( "48f4db7", 8, "48f4db700" );
    test_shift ( "31c1ba2a", 26, "c706e8a8000000" );
    test_shift ( "58149e2b", 26, "1605278ac000000" );
    test_shift ( "ec9393b", 26, "3b24e4ec000000" );
    test_shift ( "2347fbcd", 3, "11a3fde68" );
    test_shift ( "63e0cd47", 6, "18f83351c0" );
    test_shift ( "3898d87e", 13, "7131b0fc000" );
    test_shift ( "603a5db3", 1, "c074bb66" );
    test_shift ( "4edc66fa", 5, "9db8cdf40" );
    test_shift ( "3ea4e02d", 26, "fa9380b4000000" );
    test_shift ( "3d973ea5", 21, "7b2e7d4a00000" );
    test_shift ( "191e671", 27, "c8f3388000000" );
    test_shift ( "58a6d90a", 29, "b14db2140000000" );
    test_shift ( "63acb5fc", 24, "63acb5fc000000" );
    test_shift ( "1536a299", 6, "54da8a640" );
    test_shift ( "1aaa1ba", 17, "35543740000" );
    test_shift ( "4913e78c", 6, "1244f9e300" );
    test_shift ( "76a39689", 24, "76a39689000000" );
    test_shift ( "3b57ce9e", 5, "76af9d3c0" );
    test_shift ( "2788da10", 13, "4f11b420000" );
    test_shift ( "15c02247", 25, "2b80448e000000" );
    test_shift ( "30f12d71", 6, "c3c4b5c40" );
    test_shift ( "4dbdfe44", 5, "9b7bfc880" );
    test_shift ( "28e5845", 31, "1472c2280000000" );
    test_shift ( "126fd788", 19, "937ebc400000" );
    test_shift ( "3318b758", 14, "cc62dd60000" );
    test_shift ( "6fddcce7", 10, "1bf77339c00" );
    test_shift ( "4328f5a8", 18, "10ca3d6a00000" );
    test_shift ( "50ca75a5", 5, "a194eb4a0" );
    test_shift ( "77d991e8", 31, "3becc8f400000000" );
    test_shift ( "52e1de49", 4, "52e1de490" );
    test_shift ( "7fc7088e", 22, "1ff1c223800000" );
    test_shift ( "2cf08f88", 25, "59e11f10000000" );
    test_shift ( "3d1e4b8d", 7, "1e8f25c680" );
    test_shift ( "7bd76edc", 30, "1ef5dbb700000000" );
    test_shift ( "46e82892", 4, "46e828920" );
    test_shift ( "7010ccf5", 4, "7010ccf50" );
    test_shift ( "5ff08be8", 27, "2ff845f40000000" );
    test_shift ( "904e127", 14, "2413849c000" );
    test_shift ( "5a693078", 17, "b4d260f00000" );
    test_shift ( "23b21ca0", 17, "476439400000" );
    test_shift ( "6b7bcedd", 8, "6b7bcedd00" );
    test_shift ( "422c8599", 6, "108b216640" );
    test_shift ( "5c148f78", 31, "2e0a47bc00000000" );
    test_shift ( "557a702a", 2, "155e9c0a8" );
    test_shift ( "78f8137c", 20, "78f8137c00000" );
    test_shift ( "4d725504", 12, "4d725504000" );
    test_shift ( "44521449", 13, "88a42892000" );
    test_shift ( "6f29bb23", 23, "3794dd91800000" );
    test_shift ( "62ab42b2", 32, "62ab42b200000000" );
    test_shift ( "66178573", 6, "1985e15cc0" );
    test_shift ( "7c98bb23", 10, "1f262ec8c00" );
    test_shift ( "216cc148", 12, "216cc148000" );
    test_shift ( "13f71da3", 17, "27ee3b460000" );
    test_shift ( "3f082b39", 28, "3f082b390000000" );
    test_shift ( "45e65b20", 26, "117996c80000000" );
    test_shift ( "7be235ec", 30, "1ef88d7b00000000" );
    test_shift ( "764a3160", 6, "1d928c5800" );
    test_shift ( "6006ee83", 25, "c00ddd06000000" );
    test_shift ( "5105b6c4", 14, "14416db10000" );
    test_shift ( "264ce239", 1, "4c99c472" );
    test_shift ( "5dddad20", 30, "17776b4800000000" );
    test_shift ( "42bce64b", 10, "10af3992c00" );
    test_shift ( "6e221bc9", 15, "37110de48000" );
    test_shift ( "23a0213f", 29, "4740427e0000000" );
    test_shift ( "1ce7cb6e", 20, "1ce7cb6e00000" );
    test_shift ( "7a60c9c1", 18, "1e98327040000" );
    test_shift ( "5fdff91c", 10, "17f7fe47000" );
    test_shift ( "7609cd9c", 32, "7609cd9c00000000" );
    test_shift ( "463f2d79", 22, "118fcb5e400000" );
    test_shift ( "623764da", 26, "188dd9368000000" );
    test_shift ( "17cc40ae", 7, "be6205700" );
    test_shift ( "3d87b297", 15, "1ec3d94b8000" );
    test_shift ( "1c28564c", 27, "e142b260000000" );
    test_shift ( "607532e7", 17, "c0ea65ce0000" );
    test_shift ( "530fffc7", 1, "a61fff8e" );
    test_shift ( "372bd750", 8, "372bd75000" );
    test_shift ( "7a814c5e", 29, "f50298bc0000000" );
    test_shift ( "531d6e51", 8, "531d6e5100" );
    test_shift ( "2bcf5f0a", 17, "579ebe140000" );
    test_shift ( "3970c6a3", 25, "72e18d46000000" );
    test_shift ( "753643", 5, "ea6c860" );
    test_shift ( "6237b109", 32, "6237b10900000000" );
    test_shift ( "4f9f1b6e", 7, "27cf8db700" );
    test_shift ( "542c461e", 8, "542c461e00" );
    test_shift ( "d53777b", 26, "354dddec000000" );
    test_shift ( "2203d181", 11, "1101e8c0800" );
    test_shift ( "147d45bf", 25, "28fa8b7e000000" );
    test_shift ( "33362a38", 12, "33362a38000" );
    test_shift ( "7d1a2532", 32, "7d1a253200000000" );
    test_shift ( "1dd3a91b", 26, "774ea46c000000" );
    test_shift ( "1a6d7240", 13, "34dae480000" );
    test_shift ( "117d1e0", 31, "8be8f000000000" );
    test_shift ( "4ee83e08", 18, "13ba0f8200000" );
    test_shift ( "7d9226c5", 19, "3ec9136280000" );
    test_shift ( "4af2cfc2", 10, "12bcb3f0800" );
    test_shift ( "436ec78b", 6, "10dbb1e2c0" );
    test_shift ( "6ad47dcd", 21, "d5a8fb9a00000" );
    test_shift ( "2bbd3565", 28, "2bbd35650000000" );
    test_shift ( "7de7f73a", 4, "7de7f73a0" );
    test_shift ( "5051e223", 23, "2828f111800000" );
    test_shift ( "364d267c", 5, "6c9a4cf80" );
    test_shift ( "305b26e0", 29, "60b64dc00000000" );
    test_shift ( "51e137bc", 25, "a3c26f78000000" );
    test_shift ( "7b700847", 15, "3db804238000" );
    test_shift ( "773cae38", 4, "773cae380" );
    test_shift ( "1f2581e7", 25, "3e4b03ce000000" );
    test_shift ( "6e4331cf", 8, "6e4331cf00" );
    test_shift ( "2698df16", 24, "2698df16000000" );
    test_shift ( "747293f9", 28, "747293f90000000" );
    test_shift ( "37e30ce9", 28, "37e30ce90000000" );
    test_shift ( "5b2df344", 21, "b65be68800000" );
    test_shift ( "acd69c0", 19, "566b4e000000" );
    test_shift ( "20f84d09", 6, "83e134240" );
    test_shift ( "760a4d", 4, "760a4d0" );
    test_shift ( "36741aa9", 17, "6ce835520000" );
    test_shift ( "2a1bb2f9", 6, "a86ecbe40" );
    test_shift ( "431da014", 26, "10c768050000000" );
    test_shift ( "378bad61", 17, "6f175ac20000" );
    test_shift ( "3e082af2", 10, "f820abc800" );
    test_shift ( "63fa34be", 11, "31fd1a5f000" );
    test_shift ( "4c3f670c", 6, "130fd9c300" );
    test_shift ( "46eef9a2", 28, "46eef9a20000000" );
    test_shift ( "235d0a6d", 25, "46ba14da000000" );
    test_shift ( "77ae08b2", 7, "3bd7045900" );
    test_shift ( "11b2de93", 28, "11b2de930000000" );
    test_shift ( "57350221", 25, "ae6a0442000000" );
    test_shift ( "2ae2ea10", 2, "ab8ba840" );
    test_shift ( "32e342ea", 26, "cb8d0ba8000000" );
    test_shift ( "188d0b07", 24, "188d0b07000000" );
    test_shift ( "6abb7b5c", 17, "d576f6b80000" );
    test_shift ( "42139a8", 23, "2109cd4000000" );
    test_shift ( "3bc266d5", 29, "7784cdaa0000000" );
    test_shift ( "6f4e082f", 24, "6f4e082f000000" );
    test_shift ( "5c3db18d", 2, "170f6c634" );
    test_shift ( "2649c9e0", 12, "2649c9e0000" );
    test_shift ( "629b0c4b", 13, "c5361896000" );
    test_shift ( "43579cf1", 14, "10d5e73c4000" );
    test_shift ( "2d0bc9c7", 31, "1685e4e380000000" );
    test_shift ( "1711dea5", 26, "5c477a94000000" );
    test_shift ( "7e8445c4", 26, "1fa111710000000" );
    test_shift ( "544ae814", 6, "1512ba0500" );
    test_shift ( "15a58f11", 5, "2b4b1e220" );
    test_shift ( "37bbb3c6", 28, "37bbb3c60000000" );
    test_shift ( "4b09093d", 14, "12c2424f4000" );
    test_shift ( "7be21f32", 27, "3df10f990000000" );
    test_shift ( "1f49e47d", 27, "fa4f23e8000000" );
    test_shift ( "4a9bb2f0", 20, "4a9bb2f000000" );
    test_shift ( "47423297", 32, "4742329700000000" );
    test_shift ( "4e5a5f8a", 5, "9cb4bf140" );
    test_shift ( "673fee41", 11, "339ff720800" );
    test_shift ( "63b7ca6f", 13, "c76f94de000" );
    test_shift ( "672d5a56", 1, "ce5ab4ac" );
    test_shift ( "7365007a", 30, "1cd9401e80000000" );
    test_shift ( "dc40ebe", 32, "dc40ebe00000000" );
    test_shift ( "38f2f696", 3, "1c797b4b0" );
    test_shift ( "333b9c58", 11, "199dce2c000" );
    test_shift ( "62019c67", 10, "18806719c00" );
    test_shift ( "c6bb0cf", 15, "635d8678000" );
    test_shift ( "1169fd64", 13, "22d3fac8000" );
    test_shift ( "6a060efb", 24, "6a060efb000000" );
    test_shift ( "d393ea6", 26, "34e4fa98000000" );
    test_shift ( "d4f7571", 23, "6a7bab8800000" );
    test_shift ( "645c3ecc", 9, "c8b87d9800" );
    test_shift ( "11beacb6", 23, "8df565b000000" );
    test_shift ( "78118c2c", 24, "78118c2c000000" );
    test_shift ( "275ac7c0", 29, "4eb58f800000000" );
    test_shift ( "42d99583", 23, "216ccac1800000" );
    test_shift ( "2d8bdfc", 30, "b62f7f00000000" );
    test_shift ( "22c14633", 28, "22c146330000000" );
    test_shift ( "40b5751d", 10, "102d5d47400" );
    test_shift ( "1ce5213d", 22, "739484f400000" );
    test_shift ( "68f21b73", 6, "1a3c86dcc0" );
    test_shift ( "3cd23cdf", 3, "1e691e6f8" );
    test_shift ( "18a40dd3", 4, "18a40dd30" );
    test_shift ( "4cd2864f", 15, "266943278000" );
    test_shift ( "5b8856da", 22, "16e215b6800000" );
    test_shift ( "bfa1047", 13, "17f4208e000" );
    test_shift ( "31e0b68c", 20, "31e0b68c00000" );
    test_shift ( "3d697454", 3, "1eb4ba2a0" );
    test_shift ( "230ced69", 2, "8c33b5a4" );
    test_shift ( "3c9dfe39", 10, "f277f8e400" );
    test_shift ( "1144571d", 29, "2288ae3a0000000" );
    test_shift ( "58efd73f", 26, "163bf5cfc000000" );
    test_shift ( "35b629ba", 19, "1adb14dd00000" );
    test_shift ( "24b9e1d4", 24, "24b9e1d4000000" );
    test_shift ( "57655a3b", 19, "2bb2ad1d80000" );
    test_shift ( "6a5cb04c", 16, "6a5cb04c0000" );
    test_shift ( "4085c0b7", 12, "4085c0b7000" );
    test_shift ( "35b541f1", 11, "1adaa0f8800" );
    test_shift ( "756b276f", 1, "ead64ede" );
    test_shift ( "5bd3eb58", 10, "16f4fad6000" );
    test_shift ( "5c938d36", 1, "b9271a6c" );
    test_shift ( "39cb4a95", 3, "1ce5a54a8" );
    test_shift ( "58244ab3", 11, "2c122559800" );
    test_shift ( "5213a704", 30, "1484e9c100000000" );
    test_shift ( "2cafbf6b", 30, "b2befdac0000000" );
    test_shift ( "4598ed46", 9, "8b31da8c00" );
    test_shift ( "e2938f9", 7, "7149c7c80" );
    test_shift ( "52112ba1", 20, "52112ba100000" );
    test_shift ( "1a39e1f8", 23, "d1cf0fc000000" );
    test_shift ( "3a4b018a", 21, "7496031400000" );
    test_shift ( "386a1088", 24, "386a1088000000" );
    test_shift ( "31f6b1e3", 32, "31f6b1e300000000" );
    test_shift ( "4bd69f02", 21, "97ad3e0400000" );
    test_shift ( "52199fc9", 18, "148667f240000" );
    test_shift ( "6a33bc15", 2, "1a8cef054" );
    test_shift ( "123544bb", 12, "123544bb000" );
    test_shift ( "15bb86c1", 17, "2b770d820000" );
    test_shift ( "553b8d0d", 22, "154ee343400000" );
    test_shift ( "43354e3a", 18, "10cd538e80000" );
    test_shift ( "69110992", 6, "1a44426480" );
    test_shift ( "3600d94e", 25, "6c01b29c000000" );
    test_shift ( "2dd9242d", 8, "2dd9242d00" );
    test_shift ( "4d32bb5e", 16, "4d32bb5e0000" );
    test_shift ( "80974fb", 24, "80974fb000000" );
    test_shift ( "76b55d45", 6, "1dad575140" );
    test_shift ( "590bd98b", 14, "1642f662c000" );
    test_shift ( "66fc285c", 15, "337e142e0000" );
    test_shift ( "280f3f0c", 32, "280f3f0c00000000" );
    test_shift ( "72ae7f42", 22, "1cab9fd0800000" );
    test_shift ( "74148dd0", 24, "74148dd0000000" );
    test_shift ( "281669f6", 12, "281669f6000" );
    test_shift ( "23a984a2", 25, "47530944000000" );
    test_shift ( "524a61dc", 16, "524a61dc0000" );
    test_shift ( "2bb1c22d", 23, "15d8e116800000" );
    test_shift ( "203445c0", 32, "203445c000000000" );
    test_shift ( "564bdbc", 15, "2b25ede0000" );
    test_shift ( "436cc297", 10, "10db30a5c00" );
    test_shift ( "1a5f3156", 23, "d2f98ab000000" );
    test_shift ( "332831b8", 18, "cca0c6e00000" );
    test_shift ( "780c1b4d", 30, "1e0306d340000000" );
    test_shift ( "64bd1cd6", 25, "c97a39ac000000" );
    test_shift ( "58fcfcca", 20, "58fcfcca00000" );
    test_shift ( "5c1a8046", 23, "2e0d4023000000" );
    test_shift ( "7e8c0c92", 9, "fd18192400" );
    test_shift ( "7b351aab", 3, "3d9a8d558" );
    test_shift ( "2bab3ae0", 3, "15d59d700" );
    test_shift ( "78ea6cee", 3, "3c7536770" );
    test_shift ( "611d755a", 11, "308ebaad000" );
    test_shift ( "4839d132", 8, "4839d13200" );
    test_shift ( "60b47ee0", 19, "305a3f7000000" );
    test_shift ( "21920346", 29, "4324068c0000000" );
    test_shift ( "3ea33601", 30, "fa8cd8040000000" );
    test_shift ( "19571e86", 24, "19571e86000000" );
    test_shift ( "759e43d3", 31, "3acf21e980000000" );
    test_shift ( "7b6b0da8", 1, "f6d61b50" );
    test_shift ( "765cdf3c", 31, "3b2e6f9e00000000" );
    test_shift ( "3ec253f8", 7, "1f6129fc00" );
    test_shift ( "2be16fb1", 31, "15f0b7d880000000" );
    test_shift ( "506617dd", 4, "506617dd0" );
    test_shift ( "69a5d3c7", 9, "d34ba78e00" );
    test_shift ( "1d0e16a6", 8, "1d0e16a600" );
    test_shift ( "6ee6b72a", 21, "ddcd6e5400000" );
    test_shift ( "64a5ce29", 5, "c94b9c520" );
    test_shift ( "612d525e", 28, "612d525e0000000" );
    test_shift ( "5cd3640b", 31, "2e69b20580000000" );
    test_shift ( "154db64e", 18, "5536d9380000" );
    test_shift ( "27fb0ddb", 16, "27fb0ddb0000" );
    test_shift ( "63642d2f", 2, "18d90b4bc" );
    test_shift ( "2cf353a6", 3, "1679a9d30" );
    test_shift ( "dd17c9f", 15, "6e8be4f8000" );
    test_shift ( "46acd023", 28, "46acd0230000000" );
    test_shift ( "8868bcc", 28, "8868bcc0000000" );
    test_shift ( "538837e2", 31, "29c41bf100000000" );
    test_shift ( "204bf85a", 32, "204bf85a00000000" );
    test_shift ( "5ed577c1", 2, "17b55df04" );
    test_shift ( "6f898247", 8, "6f89824700" );
    test_shift ( "1f42dac8", 19, "fa16d6400000" );
    test_shift ( "11dc11fb", 18, "477047ec0000" );
    test_shift ( "2e7465f6", 26, "b9d197d8000000" );
    test_shift ( "30c8484d", 3, "186424268" );
    test_shift ( "34eb3598", 28, "34eb35980000000" );
    test_shift ( "9ad3153", 20, "9ad315300000" );
    test_shift ( "1a06eaea", 3, "d0375750" );
    test_shift ( "1e386fd4", 17, "3c70dfa80000" );
    test_shift ( "4613cf85", 20, "4613cf8500000" );
    test_shift ( "6f589fde", 9, "deb13fbc00" );
    test_shift ( "3038484f", 11, "181c2427800" );
    test_shift ( "122fc3c3", 18, "48bf0f0c0000" );
    test_shift ( "2c472728", 30, "b11c9ca00000000" );
    test_shift ( "27aecff0", 11, "13d767f8000" );
    test_shift ( "3c6d883e", 24, "3c6d883e000000" );
    test_shift ( "7002d51", 7, "38016a880" );
    test_shift ( "75a88ba9", 14, "1d6a22ea4000" );
    test_shift ( "5f990bf8", 1, "bf3217f0" );
    test_shift ( "be5a3a6", 6, "2f968e980" );
    test_shift ( "2f64bba2", 31, "17b25dd100000000" );
    test_shift ( "567752e0", 22, "159dd4b8000000" );
    test_shift ( "1db71cb1", 11, "edb8e58800" );
    test_shift ( "26234b78", 6, "988d2de00" );
    test_shift ( "37787c5a", 30, "dde1f1680000000" );
    test_shift ( "67f978f9", 25, "cff2f1f2000000" );
    test_shift ( "78f7baa5", 9, "f1ef754a00" );
    test_shift ( "1eb047e2", 9, "3d608fc400" );
    test_shift ( "1bf24179", 12, "1bf24179000" );
    test_shift ( "3da33a86", 10, "f68cea1800" );
    test_shift ( "56140df5", 5, "ac281bea0" );
    test_shift ( "5ad963a0", 7, "2d6cb1d000" );
    test_shift ( "55c125cb", 11, "2ae092e5800" );
    test_shift ( "75f07a93", 4, "75f07a930" );
    test_shift ( "749ee0ea", 27, "3a4f70750000000" );
    test_shift ( "45bb8608", 13, "8b770c10000" );
    test_shift ( "42a6f778", 9, "854deef000" );
    test_shift ( "5d158981", 11, "2e8ac4c0800" );
    test_shift ( "cb116b2", 26, "32c45ac8000000" );
    test_shift ( "1c4da0af", 13, "389b415e000" );
    test_shift ( "6f6feff6", 9, "dedfdfec00" );
    test_shift ( "6afaaf44", 28, "6afaaf440000000" );
    test_shift ( "1c78daf0", 7, "e3c6d7800" );
    test_shift ( "738f2904", 10, "1ce3ca41000" );
    test_shift ( "54a26631", 11, "2a513318800" );
    test_shift ( "7c0c2dd2", 7, "3e0616e900" );
    test_shift ( "2b43264e", 20, "2b43264e00000" );
    test_shift ( "7caaf6d", 26, "1f2abdb4000000" );
    test_shift ( "276780bd", 1, "4ecf017a" );
    test_shift ( "365e7ddc", 8, "365e7ddc00" );
    test_shift ( "7f91483a", 5, "ff2290740" );
    test_shift ( "4009fe33", 20, "4009fe3300000" );
    test_shift ( "184cdccc", 21, "3099b99800000" );
    test_shift ( "229653dd", 31, "114b29ee80000000" );
    test_shift ( "20585cae", 13, "40b0b95c000" );
    test_shift ( "6927868a", 5, "d24f0d140" );
    test_shift ( "432b0e35", 15, "2195871a8000" );
    test_shift ( "782ff740", 6, "1e0bfdd000" );
    test_shift ( "5dcd2cf5", 5, "bb9a59ea0" );
    test_shift ( "180f058f", 7, "c0782c780" );
    test_shift ( "1cf183ce", 2, "73c60f38" );
    test_shift ( "5d26074d", 29, "ba4c0e9a0000000" );
    test_shift ( "6b00c4d4", 27, "3580626a0000000" );
    test_shift ( "4938f636", 18, "124e3d8d80000" );
    test_shift ( "62abe0ba", 19, "3155f05d00000" );
    test_shift ( "2e6ea738", 22, "b9ba9ce000000" );
    test_shift ( "7bb177f7", 12, "7bb177f7000" );
    test_shift ( "247568a8", 4, "247568a80" );
    test_shift ( "b982d20", 6, "2e60b4800" );
    test_shift ( "38fc4842", 15, "1c7e24210000" );
    test_shift ( "5efb111", 13, "bdf6222000" );
    test_shift ( "3bb8d672", 7, "1ddc6b3900" );
    test_shift ( "7646049b", 19, "3b23024d80000" );
    test_shift ( "28bea86c", 17, "517d50d80000" );
    test_shift ( "1fa7edf6", 28, "1fa7edf60000000" );
    test_shift ( "682c4b6", 5, "d05896c0" );
    test_shift ( "54e8e15c", 4, "54e8e15c0" );
    test_shift ( "4ce1be1", 18, "13386f840000" );
    test_shift ( "489982bd", 24, "489982bd000000" );
    test_shift ( "5251ebc2", 25, "a4a3d784000000" );
    test_shift ( "4d9e8629", 28, "4d9e86290000000" );
    test_shift ( "d828c6d", 1, "1b0518da" );
    test_shift ( "6f393866", 22, "1bce4e19800000" );
    test_shift ( "5d4e52e4", 7, "2ea7297200" );
    test_shift ( "7903b19a", 7, "3c81d8cd00" );
    test_shift ( "26c1ef54", 12, "26c1ef54000" );
    test_shift ( "386e69f2", 8, "386e69f200" );
    test_shift ( "480e21f2", 14, "1203887c8000" );
    test_shift ( "16639379", 31, "b31c9bc80000000" );
    test_shift ( "2c7a01d", 17, "58f403a0000" );
    test_shift ( "319a7859", 21, "6334f0b200000" );
    test_shift ( "72a4f334", 22, "1ca93ccd000000" );
    test_shift ( "6cf330d7", 22, "1b3ccc35c00000" );
    test_shift ( "466cffe6", 22, "119b3ff9800000" );
    test_shift ( "457a212c", 10, "115e884b000" );
    test_shift ( "60d2170d", 23, "30690b86800000" );
    test_shift ( "197f7ea4", 27, "cbfbf520000000" );
    test_shift ( "5c68a576", 11, "2e3452bb000" );
    test_shift ( "204c988f", 27, "10264c478000000" );
    test_shift ( "38a1c91", 10, "e28724400" );
    test_shift ( "50019380", 6, "140064e000" );
    test_shift ( "1843acd4", 20, "1843acd400000" );
    test_shift ( "cc6d1ac", 7, "66368d600" );
    test_shift ( "37246c00", 7, "1b92360000" );
    test_shift ( "511e9924", 31, "288f4c9200000000" );
    test_shift ( "5935e696", 30, "164d79a580000000" );
    test_shift ( "433670f2", 11, "219b3879000" );
    test_shift ( "93c6b33", 10, "24f1accc00" );
    test_shift ( "434de8e0", 26, "10d37a380000000" );
    test_shift ( "65b6555e", 13, "cb6caabc000" );
    test_shift ( "686856c2", 12, "686856c2000" );
    test_shift ( "1be0b162", 7, "df058b100" );
    test_shift ( "34dd0fe5", 26, "d3743f94000000" );
    test_shift ( "aa08c71", 21, "154118e200000" );
    test_shift ( "32004f33", 3, "190027998" );
    test_shift ( "6e79f29d", 21, "dcf3e53a00000" );
    test_shift ( "3876b4e7", 19, "1c3b5a7380000" );
    test_shift ( "a71e027", 21, "14e3c04e00000" );
    test_shift ( "670f6e38", 8, "670f6e3800" );
    test_shift ( "6867ebba", 30, "1a19faee80000000" );
    test_shift ( "7b825845", 17, "f704b08a0000" );
    test_shift ( "3ae718da", 24, "3ae718da000000" );
    test_shift ( "d78ac1a", 14, "35e2b068000" );
    test_shift ( "6ee26b01", 27, "377135808000000" );
    test_shift ( "13ccef27", 32, "13ccef2700000000" );
    test_shift ( "598e9f07", 10, "1663a7c1c00" );
    test_shift ( "1b212ccb", 10, "6c84b32c00" );
    test_shift ( "7e1d1b50", 17, "fc3a36a00000" );
    test_shift ( "6db8a742", 2, "1b6e29d08" );
    test_shift ( "2527e525", 23, "1293f292800000" );
    test_shift ( "16e850c3", 3, "b7428618" );
    test_shift ( "21bad92a", 11, "10dd6c95000" );
    test_shift ( "1a5f7734", 18, "697ddcd00000" );
    test_shift ( "149c8c3e", 14, "527230f8000" );
    test_shift ( "6dc30578", 25, "db860af0000000" );
    test_shift ( "399cecca", 31, "1cce766500000000" );
    test_shift ( "3ea24a48", 5, "7d4494900" );
    test_shift ( "27fe26f5", 4, "27fe26f50" );
    test_shift ( "38a789b2", 23, "1c53c4d9000000" );
    test_shift ( "1ce18b5d", 26, "73862d74000000" );
    test_shift ( "6b795256", 5, "d6f2a4ac0" );
    test_shift ( "48a9bec2", 2, "122a6fb08" );
    test_shift ( "6bdf7ace", 19, "35efbd6700000" );
    test_shift ( "5698bbd1", 17, "ad3177a20000" );
    test_shift ( "4f8481d3", 23, "27c240e9800000" );
    test_shift ( "79511886", 23, "3ca88c43000000" );
    test_shift ( "f6278b9", 17, "1ec4f1720000" );
    test_shift ( "35cbd841", 14, "d72f6104000" );
    test_shift ( "4738ab01", 32, "4738ab0100000000" );
    test_shift ( "2b30d55a", 27, "15986aad0000000" );
    test_shift ( "476cdc78", 5, "8ed9b8f00" );
    test_shift ( "1e410e38", 1, "3c821c70" );
    test_shift ( "5951c7c9", 14, "165471f24000" );
    test_shift ( "522a1d23", 28, "522a1d230000000" );
    test_shift ( "5d1fc724", 2, "1747f1c90" );
    test_shift ( "5e6dca54", 27, "2f36e52a0000000" );
    test_shift ( "657bd2e5", 23, "32bde972800000" );
    test_shift ( "4f33989b", 20, "4f33989b00000" );
    test_shift ( "6dde6329", 13, "dbbcc652000" );
    test_shift ( "2af36fc4", 29, "55e6df880000000" );
    test_shift ( "218cf563", 11, "10c67ab1800" );
    test_shift ( "23cfb793", 29, "479f6f260000000" );
    test_shift ( "3f5079fb", 21, "7ea0f3f600000" );
    test_shift ( "5ab15e09", 29, "b562bc120000000" );
    test_shift ( "2403f453", 5, "4807e8a60" );
    test_shift ( "3b84d576", 12, "3b84d576000" );
    test_shift ( "6aaff588", 15, "3557fac40000" );
    test_shift ( "717ff78c", 18, "1c5ffde300000" );
    test_shift ( "200518dc", 16, "200518dc0000" );
    test_shift ( "55fb0ecc", 1, "abf61d98" );
    test_shift ( "32b5bd30", 1, "656b7a60" );
    test_shift ( "45bdf97a", 23, "22defcbd000000" );
    test_shift ( "5b806237", 22, "16e0188dc00000" );
    test_shift ( "698cddc9", 1, "d319bb92" );
    test_shift ( "3abde681", 14, "eaf79a04000" );
    test_shift ( "6c1aa5c", 5, "d8354b80" );
    test_shift ( "38c4d5d8", 16, "38c4d5d80000" );
    test_shift ( "d3a4a00", 20, "d3a4a0000000" );
    test_shift ( "42cf1c3", 11, "21678e1800" );
    test_shift ( "7e9e74cf", 24, "7e9e74cf000000" );
    test_shift ( "6dcddb6e", 7, "36e6edb700" );
    test_shift ( "13a1b6e2", 23, "9d0db71000000" );
    test_shift ( "13e92df4", 15, "9f496fa0000" );
    test_shift ( "1c7f8e48", 17, "38ff1c900000" );
    test_shift ( "48cbc31e", 21, "9197863c00000" );
    test_shift ( "311326d0", 15, "188993680000" );
    test_shift ( "26e37635", 11, "1371bb1a800" );
    test_shift ( "13b31064", 13, "276620c8000" );
    test_shift ( "bc2b25f", 15, "5e1592f8000" );
    test_shift ( "4bc29dcc", 2, "12f0a7730" );
    test_shift ( "11c03bbb", 9, "2380777600" );
    test_shift ( "22cb74c5", 20, "22cb74c500000" );
    test_shift ( "7d15aa18", 7, "3e8ad50c00" );
    test_shift ( "429a6166", 28, "429a61660000000" );
    test_shift ( "17f166d0", 23, "bf8b368000000" );
    test_shift ( "297381f2", 31, "14b9c0f900000000" );
    test_shift ( "7b5c207c", 22, "1ed7081f000000" );
    test_shift ( "5e3d1334", 17, "bc7a26680000" );
    test_shift ( "4236e743", 29, "846dce860000000" );
    test_shift ( "43339541", 2, "10cce5504" );
    test_shift ( "6d373e91", 18, "1b4dcfa440000" );
    test_shift ( "6842ab0", 7, "342155800" );
    test_shift ( "6b17dc5c", 21, "d62fb8b800000" );
    test_shift ( "167e8d32", 28, "167e8d320000000" );
    test_shift ( "17772942", 31, "bbb94a100000000" );
    test_shift ( "3d5b279c", 31, "1ead93ce00000000" );
    test_shift ( "34c57326", 3, "1a62b9930" );
    test_shift ( "73bc7691", 31, "39de3b4880000000" );
    test_shift ( "102c5b28", 25, "2058b650000000" );
    test_shift ( "331db91a", 25, "663b7234000000" );
    test_shift ( "778fae2e", 13, "ef1f5c5c000" );
    test_shift ( "2ddd0436", 11, "16ee821b000" );
    test_shift ( "19a673e1", 11, "cd339f0800" );
    test_shift ( "2311d1a", 6, "8c474680" );
    test_shift ( "6d6b8e7", 28, "6d6b8e70000000" );
    test_shift ( "66e00586", 25, "cdc00b0c000000" );
    test_shift ( "39ab6e6d", 23, "1cd5b736800000" );
    test_shift ( "828ac3e", 10, "20a2b0f800" );
    test_shift ( "79b6b4b", 17, "f36d6960000" );
    test_shift ( "1b9dd984", 14, "6e776610000" );
    test_shift ( "e8646e", 2, "3a191b8" );
    test_shift ( "4849f98b", 21, "9093f31600000" );
    test_shift ( "391f9d83", 30, "e47e760c0000000" );
    test_shift ( "6788f4d3", 12, "6788f4d3000" );
    test_shift ( "725d4815", 14, "1c9752054000" );
    test_shift ( "7169baa3", 4, "7169baa30" );
    test_shift ( "7737e8f9", 26, "1dcdfa3e4000000" );
    test_shift ( "5cd8c4ed", 28, "5cd8c4ed0000000" );
    test_shift ( "2b60d643", 8, "2b60d64300" );
    test_shift ( "6cbbb800", 11, "365ddc00000" );
    test_shift ( "246e9463", 7, "12374a3180" );
    test_shift ( "264586a2", 17, "4c8b0d440000" );
    test_shift ( "40ffedbd", 1, "81ffdb7a" );
    test_shift ( "2dd4d99", 9, "5ba9b3200" );
    test_shift ( "4d156c50", 30, "13455b1400000000" );
    test_shift ( "67aded95", 31, "33d6f6ca80000000" );
    test_shift ( "7774283e", 2, "1ddd0a0f8" );
    test_shift ( "3aba853", 2, "eaea14c" );
    test_shift ( "6bfe573e", 7, "35ff2b9f00" );
    test_shift ( "79dfbe6c", 20, "79dfbe6c00000" );
    test_shift ( "5db4b13", 16, "5db4b130000" );
    test_shift ( "48489596", 13, "90912b2c000" );
    test_shift ( "a9037e8", 4, "a9037e80" );
    test_shift ( "df190e7", 13, "1be321ce000" );
    test_shift ( "42b3c8a", 8, "42b3c8a00" );
    test_shift ( "68289d56", 14, "1a0a27558000" );
    test_shift ( "4e49066e", 26, "1392419b8000000" );
    test_shift ( "6b3d3bd", 12, "6b3d3bd000" );
    test_shift ( "3cdc56d9", 23, "1e6e2b6c800000" );
    test_shift ( "57e44d33", 11, "2bf22699800" );
    test_shift ( "280c4874", 9, "501890e800" );
    test_shift ( "57ef93e8", 19, "2bf7c9f400000" );
    test_shift ( "6f8a21e9", 28, "6f8a21e90000000" );
    test_shift ( "50143674", 8, "5014367400" );
    test_shift ( "46cfd961", 1, "8d9fb2c2" );
    test_shift ( "39e4187a", 21, "73c830f400000" );
    test_shift ( "353d6df0", 17, "6a7adbe00000" );
    test_shift ( "49be5881", 25, "937cb102000000" );
    test_shift ( "274e0893", 9, "4e9c112600" );
    test_shift ( "75beb404", 31, "3adf5a0200000000" );
    test_shift ( "525d3250", 28, "525d32500000000" );
    test_shift ( "5413160b", 31, "2a098b0580000000" );
    test_shift ( "6c557554", 10, "1b155d55000" );
    test_shift ( "2fef2ce9", 14, "bfbcb3a4000" );
    test_shift ( "64580b1f", 29, "c8b0163e0000000" );
    test_shift ( "33238f57", 20, "33238f5700000" );
    test_shift ( "4765b4e4", 1, "8ecb69c8" );
    test_shift ( "2be4c446", 15, "15f262230000" );
    test_shift ( "66ae5f7b", 27, "33572fbd8000000" );
    test_shift ( "12784ff5", 30, "49e13fd40000000" );
    test_shift ( "45ecef9a", 17, "8bd9df340000" );
    test_shift ( "7a295d51", 11, "3d14aea8800" );
    test_shift ( "4e891680", 19, "27448b4000000" );
    test_shift ( "3af80363", 21, "75f006c600000" );
    test_shift ( "1b979f3b", 8, "1b979f3b00" );
    test_shift ( "21506432", 12, "21506432000" );
    test_shift ( "e9e08c2", 30, "3a7823080000000" );
    test_shift ( "e9b0a49", 23, "74d8524800000" );
    test_shift ( "502a6406", 19, "2815320300000" );
    test_shift ( "24254a44", 7, "1212a52200" );
    test_shift ( "465db14e", 28, "465db14e0000000" );
    test_shift ( "40e6c2b9", 19, "2073615c80000" );
    test_shift ( "625bfcdb", 32, "625bfcdb00000000" );
    test_shift ( "44b33d00", 24, "44b33d00000000" );
    test_shift ( "68c481b9", 23, "346240dc800000" );
    test_shift ( "76889534", 21, "ed112a6800000" );
    test_shift ( "2387f566", 6, "8e1fd5980" );
    test_shift ( "29dbcede", 7, "14ede76f00" );
    test_shift ( "3499a858", 2, "d266a160" );
    test_shift ( "67e82afa", 20, "67e82afa00000" );
    test_shift ( "158a89a9", 13, "2b151352000" );
    test_shift ( "3e26191e", 12, "3e26191e000" );
    test_shift ( "7e9c096a", 8, "7e9c096a00" );
    test_shift ( "1f1c1082", 17, "3e3821040000" );
    test_shift ( "b4b5a99", 7, "5a5ad4c80" );
    test_shift ( "348dc96", 8, "348dc9600" );
    test_shift ( "1a8a3461", 17, "351468c20000" );
    test_shift ( "5f6c7219", 30, "17db1c8640000000" );
    test_shift ( "30fb264f", 27, "187d93278000000" );
    test_shift ( "45f08d94", 10, "117c2365000" );
    test_shift ( "7b4b3c10", 9, "f696782000" );
    test_shift ( "4871195d", 23, "24388cae800000" );
    test_shift ( "2d2b154d", 28, "2d2b154d0000000" );
    test_shift ( "10e43d5c", 6, "4390f5700" );
    test_shift ( "5720ba7d", 24, "5720ba7d000000" );
    test_shift ( "31f60538", 7, "18fb029c00" );
    test_shift ( "204f783", 23, "1027bc1800000" );
    test_shift ( "10d3d691", 14, "434f5a44000" );
    test_shift ( "3cdd41bd", 20, "3cdd41bd00000" );
    test_shift ( "4f676e5e", 23, "27b3b72f000000" );
    test_shift ( "733141d9", 21, "e66283b200000" );
    test_shift ( "19d1a83d", 28, "19d1a83d0000000" );
    test_shift ( "16dfea44", 24, "16dfea44000000" );
    test_shift ( "aa1a778", 21, "15434ef000000" );
    test_shift ( "1d5dc971", 13, "3abb92e2000" );
    test_shift ( "619ab89d", 2, "1866ae274" );
    test_shift ( "d0b57d4", 27, "685abea0000000" );
    test_shift ( "377c36f7", 2, "ddf0dbdc" );
    test_shift ( "1c58ba35", 20, "1c58ba3500000" );
    test_shift ( "1bfb2ac7", 19, "dfd956380000" );
    test_shift ( "412cdcaa", 32, "412cdcaa00000000" );
    test_shift ( "6024b8d8", 15, "30125c6c0000" );
    test_shift ( "3e0d4e56", 11, "1f06a72b000" );
    test_shift ( "43d2d51b", 20, "43d2d51b00000" );
    test_shift ( "20e8767d", 26, "83a1d9f4000000" );
    test_shift ( "43132c6a", 24, "43132c6a000000" );
    test_shift ( "65ea8e6e", 8, "65ea8e6e00" );
    test_shift ( "21d52e92", 19, "10ea974900000" );
    test_shift ( "5622eefe", 11, "2b11777f000" );
    test_shift ( "44a58946", 16, "44a589460000" );
    test_shift ( "7d090b16", 4, "7d090b160" );
    test_shift ( "c29bdf0", 11, "614def8000" );
    test_shift ( "504c13dd", 8, "504c13dd00" );
    test_shift ( "444ad00b", 20, "444ad00b00000" );
    test_shift ( "c06693b", 19, "603349d80000" );
    test_shift ( "601e42c5", 6, "180790b140" );
    test_shift ( "2e372ad2", 31, "171b956900000000" );
    test_shift ( "10651a13", 9, "20ca342600" );
    test_shift ( "313b8b08", 16, "313b8b080000" );
    test_shift ( "672f093b", 6, "19cbc24ec0" );
    test_shift ( "677232a8", 6, "19dc8caa00" );
    test_shift ( "663db9dc", 23, "331edcee000000" );

    print_results();
    return 0;
}