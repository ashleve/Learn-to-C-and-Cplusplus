#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "dictionary.h"

struct dictionary_t* create_dictionary(int N)
{
    if(N<=0)
        return NULL;
    struct dictionary_t *d=malloc(sizeof(struct dictionary_t));
    if(!d)
        return NULL;

    d->wc=malloc(N*sizeof(struct word_count_t));
    if(d->wc==NULL)
    {
        free(d);
        return NULL;
    }
    d->capacity=N;
    d->size=0;

    return d;
}
void destroy_dictionary(struct dictionary_t** d)
{
    if(!d || !*d)
        return;

    for(int i=0; i<(*d)->size; i++)
    {
        if((*d)->wc+i!=NULL)
        {
            if(((*d)->wc+i)->word!=NULL)
                free(((*d)->wc+i)->word);
        }
    }
    free((*d)->wc);
    free(*d);
}
struct word_count_t* dictionary_find_word(const struct dictionary_t *d, const char *word)
{
    if(!d || !word || !d->wc || d->size<0 || d->capacity<0)
        return NULL;

    for(int i=0; i<d->size; i++)
    {
        if(d->wc+i)
            if(!strcmp(word, (d->wc+i)->word))
                return (d->wc+i);
    }
    return NULL;
}
void dictionary_display(const struct dictionary_t *d)
{
    if(!d || !d->wc || d->capacity<0 || d->size>d->capacity)
        return;

    for(int i=0; i<d->size; i++)
        printf("%s %d\n", (d->wc+i)->word, (d->wc+i)->counter);
}
int dictionary_add_word(struct dictionary_t *d, const char *word)
{
    if(!d || !word || !d->wc || d->size<0 || d->capacity<0)
        return 1;

    struct word_count_t *ptr=dictionary_find_word(d, word);
    if(ptr)
        ptr->counter++;
    else
    {
        struct word_count_t *d_temp;
        if(d->size+1>d->capacity)
        {
            d_temp=realloc(d->wc, d->capacity*2*sizeof(struct word_count_t));
            if(!d_temp)
                return 2;
            d->capacity*=2;
            d->wc=d_temp;
        }

        char *rtp=malloc((strlen(word)+1)*sizeof(char));
        if(!rtp)
        {
            if(!d_temp)
                d->capacity/=2;
            return 2;
        }
        (d->wc+d->size)->word=rtp;

        strcpy((d->wc+d->size)->word, word);
        (d->wc+d->size)->counter=1;
        d->size++;
    }

    return 0;
}

int determine(char *a, char *b)
{

    if(islower(*a) && islower(*b))
    {
        if(*a<*b)
            return 1;
        if(*a==*b)
        {
            if(determine(a+1, b+1))
                return 1;
        }
    }
    else if(!islower(*a) && !islower(*b))
    {
        if(*a<*b)
            return 1;
        if(*a==*b)
        {
            if(determine(a+1, b+1))
                return 1;
        }
    }
    else if(islower(*a) && !islower(*b))
    {
//        if(*a<*b+32)  //odblokowac jesli ma sortowac zgodznie z trescia zadania
//            return 1;
//        if(*a==*b+32)
        return 0;
    }
    else if(!islower(*a) && islower(*b))
    {
//        if(*a+32<*b)
        return 1;
//        if(*a+32==*b)
//            return 0;
    }

    return 0;
}
int dictionary_sort_alphabetically(struct dictionary_t *d)
{
    if(!d || d->size<=0 || d->capacity<=0 || d->size>d->capacity || !d->wc)
        return 1;

    for(int i=0; i<d->size; i++)
    {
        for(int j=i; j>0 && determine((d->wc+j)->word, (d->wc+j-1)->word); j--)
        {
            void *ptr=(d->wc+j)->word;
            (d->wc+j)->word=(d->wc+j-1)->word;
            (d->wc+j-1)->word=ptr;

            int tmp=(d->wc+j)->counter;
            (d->wc+j)->counter=(d->wc+j-1)->counter;
            (d->wc+j-1)->counter=tmp;
        }
    }

    return 0;
}
int dictionary_sort_occurence(struct dictionary_t *d)
{
    if(!d || d->size<=0 || d->capacity<=0 || d->size>d->capacity || !d->wc)
        return 1;

    for(int i=0; i<d->size; i++)
    {
        for(int j=i; j>0; j--)
        {
            if((d->wc+j)->counter>(d->wc+j-1)->counter || ((d->wc+j)->counter==(d->wc+j-1)->counter && determine((d->wc+j)->word, (d->wc+j-1)->word)))
            {
                void *ptr=(d->wc+j)->word;
                (d->wc+j)->word=(d->wc+j-1)->word;
                (d->wc+j-1)->word=ptr;

                int tmp=(d->wc+j)->counter;
                (d->wc+j)->counter=(d->wc+j-1)->counter;
                (d->wc+j-1)->counter=tmp;
            }
        }
    }

    return 0;
}
int save_dictionary_b(const struct dictionary_t *d, const char *filename)
{
    if(!d || !filename || d->size<=0 || d->capacity<=0 || d->size>d->capacity || !d->wc)
        return 1;

    FILE *fp=fopen(filename, "wb");
    if(!fp)
        return 2;

    fwrite(&(d->size), sizeof(int), 1, fp);

    for(int i=0; i<d->size; i++)
    {
        int length=strlen((d->wc+i)->word);
        fwrite(&length, sizeof(int), 1, fp);
        fwrite((d->wc+i)->word, strlen((d->wc+i)->word)*sizeof(char), 1, fp);
        fwrite(&((d->wc+i)->counter), sizeof(int), 1, fp);
    }
    fclose(fp);

    return 0;
}
struct dictionary_t* load_dictionary_b(const char *filename, int *err_code)
{
    if(!filename)
    {
        if(err_code)
            *err_code=1;
        return NULL;
    }

    FILE *fp=fopen(filename, "rb");
    if(!fp)
    {
        if(err_code)
            *err_code=2;
        return NULL;
    }

    int size=0;
    if(!fread(&size, sizeof(int), 1, fp) || size<=0)
    {
        if(err_code)
            *err_code=3;
        fclose(fp);
        return NULL;
    }

    struct dictionary_t *d=create_dictionary(size);
    if(!d)
    {
        fclose(fp);
        if(err_code)
            *err_code=4;
        return NULL;
    }

    char *word=malloc(64*sizeof(char));
    if(!word)
    {
        fclose(fp);
        destroy_dictionary(&d);
        if(err_code)
            *err_code=4;
        return NULL;
    }

    int count=0;
    while(!feof(fp))
    {
        int word_length=0;
        if(!fread(&word_length, sizeof(int), 1, fp))
            break;

        if(fread(word, sizeof(char), word_length, fp))
        {
            *(word+word_length)='\0';
            if(dictionary_add_word(d, word)==2)
            {
                fclose(fp);
                free(word);
                destroy_dictionary(&d);
                if(err_code)
                    *err_code=4;
                return NULL;
            }

            if(!fread(&(d->wc+d->size-1)->counter, sizeof(int), 1, fp))
                break;
        }
        count++;
    }
    fclose(fp);
    free(word);
    if(size!=count)
    {
        if(err_code)
            *err_code=3;
        destroy_dictionary(&d);
        return NULL;
    }

    if(err_code)
        *err_code=0;
    return d;
}

