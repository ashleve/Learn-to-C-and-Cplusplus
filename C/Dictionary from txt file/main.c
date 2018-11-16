#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "dictionary.h"
#define SIZE 50
#define WORD_LENGTH 64
#define DICTIONARY_SIZE 64


int main()
{
    char *ptr=malloc(SIZE*sizeof(char));
    if(!ptr)
    {
        printf("failed to allocate memory");
        return 2;
    }
    printf("Podaj nazwe pliku: ");
    scanf("%49s", ptr);

    FILE *fp=fopen(ptr, "r");
    if(!fp)
    {
        printf("File not found");
        free(ptr);
        return 1;
    }

    struct dictionary_t *dictionary=create_dictionary(DICTIONARY_SIZE);
    if(!dictionary)
    {
        printf("failed to allocate memory");
        free(ptr);
        fclose(fp);
        return 2;
    }
    char *word=malloc(WORD_LENGTH*sizeof(char));
    if(!word)
    {
        printf("failed to allocate memory");
        free(ptr);
        fclose(fp);
        destroy_dictionary(&dictionary);
        return 2;
    }

    char c;
    int i=0, end=0;
    c=fgetc(fp);
    while(!end)
    {
        if(isalpha(c))
        {
            *(word+i)=c;
            i++;
        }
        else if(i>0)
        {
            *(word+i)='\0';
            if(dictionary_add_word(dictionary, word)==2)
            {
                printf("failed to allocate memory");
                free(ptr);
                free(word);
                fclose(fp);
                destroy_dictionary(&dictionary);
                return 2;
            }
            i=0;
        }
        if(c==EOF)end=1;
        else c=fgetc(fp);
    }

    dictionary_display(dictionary);
    free(ptr);
    destroy_dictionary(&dictionary);
    fclose(fp);
    free(word);
    return 0;
}


