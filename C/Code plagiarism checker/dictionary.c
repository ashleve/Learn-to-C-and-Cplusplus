#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dictionary.h"

struct dictionary_t* create_dictionary(int N)
{
    if(N<=0)return NULL;
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
    if(!d || !*d)return;

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
        {
            if(!strcmp(word, (d->wc+i)->word))
                return (d->wc+i);
        }
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
        char *rtp=malloc((strlen(word)+1)*sizeof(char));
        if(!rtp)return 2;

        struct word_count_t *d_temp=NULL;
        if(d->size+1>d->capacity)
        {
            d_temp=realloc(d->wc, d->capacity*2*sizeof(struct word_count_t));
            if(!d_temp)
            {
                free(rtp);
                return 2;
            }
            d->capacity*=2;
            d->wc=d_temp;
        }

        (d->wc+d->size)->word=rtp;

        strcpy((d->wc+d->size)->word, word);
        (d->wc+d->size)->counter=1;
        d->size++;
    }

    return 0;
}
