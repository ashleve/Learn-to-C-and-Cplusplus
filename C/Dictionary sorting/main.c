#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "dictionary.h"
#define SIZE 40
#define WORD_LENGTH 64
#define DICTIONARY_SIZE 64

int main()
{
    char *filename=malloc(SIZE*sizeof(char));
    if(!filename)
    {
        printf("failed to allocate memory");
        return 4;
    }
    printf("Podaj nazwe pliku: ");
    scanf("%39s", filename);

    int w;
    printf("wybierz mondrze: ");
    scanf("%d", &w);

    if(w!=1 && w!=0)
    {
        printf("Incorrect input");
        free(filename);
        return 1;
    }
    else if(w==0)
    {
        int err_code;
        struct dictionary_t *d=load_dictionary_b(filename, &err_code);
        if(err_code==4)
        {
            printf("Failed to allocate memory");
            free(filename);
            return 4;
        }
        if(err_code==2)
        {
            printf("File not found");
            free(filename);
            return 2;
        }
        if(err_code==3)
        {
            printf("File corrupted");
            free(filename);
            return 3;
        }

        char *filename1=malloc((SIZE+10)*sizeof(char));
        char *filename2=malloc((SIZE+10)*sizeof(char));
        if(!filename1 || !filename2)
        {
            printf("Failed to allocate memory");
            free(filename);
            if(filename1)free(filename1);
            if(filename2)free(filename2);
            destroy_dictionary(&d);
            return 4;
        }
        strcpy(filename1, filename);
        strcpy(filename2, filename);
        *(filename1+strlen(filename1)-4)='\0';
        *(filename2+strlen(filename2)-4)='\0';
        strcat(filename1, "_alpha.bin");
        strcat(filename2, "_occ.bin");

        dictionary_sort_alphabetically(d);
        save_dictionary_b(d, filename1) ? printf("Couldn't create file\n") : printf("File saved\n");
        dictionary_sort_occurence(d);
        save_dictionary_b(d, filename2) ? printf("Couldn't create file\n") : printf("File saved\n");

        free(filename);
        free(filename1);
        free(filename2);
        destroy_dictionary(&d);
        return 0;
    }
    else if(w==1)
    {

        FILE *fp=fopen(filename, "r");
        if(!fp)
        {
            printf("File not found");
            free(filename);
            return 2;
        }
        struct dictionary_t *dictionary=create_dictionary(DICTIONARY_SIZE);
        if(!dictionary)
        {
            printf("Failed to allocate memory");
            free(filename);
            fclose(fp);
            return 4;
        }
        char *word=malloc(WORD_LENGTH*sizeof(char));
        if(!word)
        {
            printf("Failed to allocate memory");
            free(filename);
            fclose(fp);
            destroy_dictionary(&dictionary);
            return 4;
        }

        int i=0, end=0, c;
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
                    printf("Failed to allocate memory");
                    free(filename);
                    free(word);
                    fclose(fp);
                    destroy_dictionary(&dictionary);
                    return 4;
                }
                i=0;
            }
            if(c==EOF)
                end=1;
            else
                c=fgetc(fp);
        }

        char *filename1=malloc((SIZE+10)*sizeof(char));
        char *filename2=malloc((SIZE+10)*sizeof(char));
        if(!filename1 || !filename2)
        {
            printf("Failed to allocate memory");
            free(filename);
            if(filename1)free(filename1);
            if(filename2)free(filename2);
            destroy_dictionary(&dictionary);
            fclose(fp);
            free(word);
            return 4;
        }
        strcpy(filename1, filename);
        strcpy(filename2, filename);
        *(filename1+strlen(filename1)-4)='\0';
        *(filename2+strlen(filename2)-4)='\0';
        strcat(filename1, "_alpha.txt");
        strcat(filename2, "_occ.txt");

        dictionary_sort_alphabetically(dictionary);
        save_dictionary_b(dictionary, filename1) ? printf("Couldn't create file\n") : printf("File saved\n");
        dictionary_sort_occurence(dictionary);
        save_dictionary_b(dictionary, filename2) ? printf("Couldn't create file\n") : printf("File saved\n");

        free(filename);
        free(filename1);
        free(filename2);
        destroy_dictionary(&dictionary);
        fclose(fp);
        free(word);

        return 0;
    }
    free(filename);
}
