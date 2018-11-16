#include <stdio.h>

unsigned int swap_bits(unsigned int *pvalue, int b1, int b2)
{
    if(b1>31 || b2>31 || b1<0 || b2<0 || !pvalue)return 1;
    //znajdz bity
    unsigned int bit1 = (*pvalue >> b1) & 1;
    unsigned int bit2 = (*pvalue >> b2) & 1;

    unsigned int z = (bit1 ^ bit2); //XORujemy bity

    z = (z << b1) | (z << b2);  //przypisujemy bity do ich oryginalnyh pozycji

    *pvalue=*pvalue ^ z;    //XORujemy z oryginalna liczba zeby podminic bity

    return 0;
}

int main()
{
    unsigned int a;
    int b1, b2;
    printf("Enter an uint32 number: ");
    scanf("%u", &a);
    printf("Enter 1st bit index: ");
    scanf("%d", &b1);
    printf("Enter 2nd bit index: ");
    scanf("%d", &b2);

    swap_bits(&a, b1, b2);
    printf("Result: %u", a);
//    zastap wartosc bitu na pozycji b1,b2:
//    *pvalue = v1 ? (*pvalue | (1 << b2)) : (*pvalue & ~(1 << b2));
//    *pvalue = v2 ? (*pvalue | (1 << b1)) : (*pvalue & ~(1 << b1));
    return 0;
}


