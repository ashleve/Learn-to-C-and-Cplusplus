#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "char_operations.h"
#define SIZE 1010

int main()
{
    char *text = malloc(SIZE*sizeof(char));
    if(!text)
    {
        printf("Failed to allocate memory");
        return 2;
    }

    printf("Podaj tekst: ");
    fgets(text, SIZE, stdin);
    *(text+strlen(text))='\0';

    func_ptr funcs[4];
    *(funcs+0)=lower_to_upper;
    *(funcs+1)=upper_to_lower;
    *(funcs+2)=space_to_dash;
    *(funcs+3)=reverse_letter;

    int c;
    printf("Podaj rodzaj operacji: ");
    scanf("%d", &c);
    if(c<0 || c>3)
    {
        printf("Incorrect input");
        free(text);
        return 1;
    }

    char *res = letter_modifier(text, *(funcs+c));
    free(text);
    if(!res)
    {
        printf("Failed to allocate memory");
        return 2;
    }

    printf("%s", res);

    free(res);
    return 0;
}
