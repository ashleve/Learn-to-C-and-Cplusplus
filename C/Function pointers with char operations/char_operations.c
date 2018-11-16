#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "char_operations.h"

char lower_to_upper(char ch)
{
    ch = toupper(ch);
    return ch;
}
char upper_to_lower(char ch)
{
    ch = tolower(ch);
    return ch;
}
char space_to_dash(char ch)
{
    if(ch == ' ')ch = '_';
    return ch;
}
char reverse_letter(char ch)
{
    if((ch<='z' && ch>='a'))
    {
        ch='z'-(ch-'a');
    }
    else if(ch>='A' && ch<='Z')
    {
        ch='Z'-(ch-'A');
    }
    return ch;
}

char* letter_modifier(const char* text, func_ptr ptr)
{
    if(!text || !ptr || strlen(text) == 0)return NULL;

    char *temp = malloc((strlen(text) + 1)*sizeof(char));
    if(!temp)return NULL;

    for(unsigned int i = 0; i <= strlen(text); i++)
        *(temp+i) = ptr(*(text+i));

    return temp;
}
