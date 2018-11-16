#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "dictionary.h"
#define SIZE 50
#define WORD_LENGTH 64
#define DICTIONARY_SIZE 64

struct array_t
{
    float *ptr;
    int size;
    int capacity;
};

int add_words(struct dictionary_t *dict, FILE *fp)
{
    char *word=malloc(WORD_LENGTH*sizeof(char));
    if(!word)
    {
        printf("failed to allocate memory");
        return 4;
    }

    int c;
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
            if(dictionary_add_word(dict, word)==2)
            {
                printf("failed to allocate memory");
                free(word);
                return 4;
            }
            i=0;
        }
        if(c==EOF)end=1;
        else c=fgetc(fp);
    }
    free(word);
    return 0;
}

struct dictionary_t* merge_dictionaries(struct dictionary_t* d1, struct dictionary_t* d2)
{
    struct dictionary_t* dict=create_dictionary(d1->size + d2->size);
    if(!dict)return NULL;

    int count=0;
    for(int i=0; i<d1->size; i++)
    {
        if(dictionary_find_word(dict, (d1->wc+i)->word)!=NULL)continue;

        char *tmp=malloc((strlen((d1->wc+i)->word)+1)*sizeof(char));
        strcpy(tmp, (d1->wc+i)->word);

        (dict->wc+count)->word=tmp;
        (dict->wc+count)->counter=(d1->wc+i)->counter;
        (dict->size)++;
        count++;
    }

    int count2=0;
    for(int j=0; j<d2->size; j++)
    {
        if(dictionary_find_word(dict, (d2->wc+j)->word)!=NULL)continue;

        char *tmp=malloc((strlen((d2->wc+j)->word)+1)*sizeof(char));
        strcpy(tmp, (d2->wc+j)->word);

        (dict->wc+count+count2)->word=tmp;
        (dict->wc+count+count2)->counter=(d2->wc+j)->counter;
        (dict->size)++;
        count2++;
    }

    return dict;
}

void destroy_array(struct array_t **a)
{
    if(!a || !*a || !(*a)->ptr)return;
    free((*a)->ptr);
    free(*a);
}

void destroy(struct array_t *a)
{
    if(!a || !a->ptr)return;
    free(a->ptr);
    a->size=0;
    a->capacity=0;
}


int create(struct array_t *a, int N)
{
    if(!a || N<=0)return 1;

    a->ptr=malloc(N*sizeof(float));
    if(!a->ptr)return 2;
    a->capacity=N;
    a->size=0;

    return 0;
}

int create_array(struct array_t **a, int N)
{
    if(!a || N<=0)return 1;

    *a=malloc(sizeof(struct array_t));
    if(!*a)return 2;
    if(create(*a, N)!=0)
    {
        free(*a);
        return 2;
    }

    return 0;
}

float length(const struct array_t *a)
{
    if(!a || !a->ptr || a->size<=0 || a->capacity<a->size)return -1;

    float res=0;
    for(int i=0; i<a->size; i++)
    {
        res=res+(*(a->ptr+i))*(*(a->ptr+i));
    }
    return (float)sqrt(res);
}

int add(struct array_t *a, float value)
{
    if(!a || !a->ptr || a->size<0)return 1;
    if(a->capacity<a->size+1)return 2;
    *(a->ptr+a->size)=value;
    a->size++;
    return 0;
}

float dot_product(const struct array_t *a, const struct array_t *b)
{
    if(!a || !b || !a->ptr || !b->ptr || a->size!=b->size || a->size<=0 || a->capacity<a->size || b->capacity<b->size)return -1;

    float res=0;
    for(int i=0; i<a->size; i++)
        res+=(*(a->ptr+i))*(*(b->ptr+i));
    return res;
}

int number_of_words(struct dictionary_t *dict)
{
    int sum=0;
    for(int i=0; i<dict->size; i++)
    {
        sum+=(dict->wc+i)->counter;
    }
    return sum;
}
float find_value(struct dictionary_t *dict, char *word)
{
    for(int i=0; i<dict->size; i++)
    {
        if(strcmp((dict->wc+i)->word, word)==0)
        {
            return (float)((dict->wc+i)->counter)/number_of_words(dict);
        }
    }
    return 0;
}

int main()
{
    char *name1=malloc(SIZE*sizeof(char));
    if(!name1)
    {
        printf("failed to allocate memory");
        return 4;
    }
    printf("Give first file: ");
    scanf("%49s", name1);

    FILE *fp1=fopen(name1, "r");
    if(!fp1)
    {
        printf("File not found");
        free(name1);
        return 2;
    }

    struct dictionary_t *d1=create_dictionary(DICTIONARY_SIZE);
    if(!d1)
    {
        printf("failed to allocate memory");
        free(name1);
        fclose(fp1);
        return 4;
    }
    if(add_words(d1, fp1)==4)
    {
        free(name1);
        fclose(fp1);
        destroy_dictionary(&d1);
        return 4;
    }
    fclose(fp1);
    free(name1);

    ///////////////////////////////////////////////////////

    char *name2=malloc(SIZE*sizeof(char));
    if(!name2)
    {
        printf("failed to allocate memory");
        return 4;
    }
    printf("Give second file: ");
    scanf("%49s", name2);

    FILE *fp2=fopen(name2, "r");
    if(!fp2)
    {
        printf("File not found");
        free(name2);
        destroy_dictionary(&d1);
        return 2;
    }

    struct dictionary_t *d2=create_dictionary(DICTIONARY_SIZE);
    if(!d2)
    {
        printf("failed to allocate memory");
        free(name2);
        fclose(fp2);
        destroy_dictionary(&d1);
        return 4;
    }
    if(add_words(d2, fp2)==4)
    {
        free(name2);
        fclose(fp2);
        destroy_dictionary(&d1);
        destroy_dictionary(&d2);
        return 4;
    }
    fclose(fp2);
    free(name2);


    struct dictionary_t *main_dict=merge_dictionaries(d1, d2);

    struct array_t *vec1, *vec2;
    create_array(&vec1, main_dict->size);
    create_array(&vec2, main_dict->size);
    vec1->size=main_dict->size;
    vec2->size=main_dict->size;

    for(int i=0; i<main_dict->size; i++)
    {
        *(vec1->ptr+i)=find_value(d1, (main_dict->wc+i)->word);
        *(vec2->ptr+i)=find_value(d2, (main_dict->wc+i)->word);
    }



    float result=dot_product(vec1, vec2)/(length(vec1)*length(vec2));

    printf("Result: %.5f", result);


    destroy_dictionary(&d1);
    destroy_dictionary(&d2);
    destroy_dictionary(&main_dict);
    destroy_array(&vec1);
    destroy_array(&vec2);
}
