#include <stdio.h>
#include <stdlib.h>

struct circular_buffer_t
{
    int *ptr;
    int begin;
    int end;
    int capacity;
    unsigned char full:1;
};

int create(struct circular_buffer_t *cb, int N)
{
    if(cb==NULL || N<=0)
        return 1;

    cb->ptr=(int*)malloc(N*sizeof(int));
    if(cb->ptr==NULL)
        return 2;

    cb->full=0;
    cb->begin=0;
    cb->end=0;
    cb->capacity=N;

    return 0;
}

int create_circular_buffer(struct circular_buffer_t **cb, int N)
{
    if(N<=0 || cb==NULL)
        return 1;

    *cb=(struct circular_buffer_t*)malloc(sizeof(struct circular_buffer_t));
    if(*cb==NULL)
        return 2;

    int err_code=create(*cb, N);
    if(err_code)
    {
        free(*cb);
        return 2;
    }

    return 0;
}

void destroy(struct circular_buffer_t *cb)
{
    if(cb==NULL)
        return;
    free(cb->ptr);
}

void destroy_circular_buffer(struct circular_buffer_t **cb)
{
    if(cb==NULL)
        return;
    destroy(*cb);
    free(*cb);
}

int empty(const struct circular_buffer_t *cb)
{
    if(cb==NULL || cb->begin<0 || cb->end<0 || cb->begin>=cb->capacity || cb->end>=cb->capacity || cb->ptr==NULL)
        return -1;

    if(cb->full==0 && cb->begin==cb->end)
        return 1;

    return 0;
}

int full(const struct circular_buffer_t *cb)
{
    if(cb==NULL || cb->begin<0 || cb->end<0 || cb->begin>=cb->capacity || cb->end>=cb->capacity || cb->ptr==NULL)
        return -1;

    if(cb->full==1 && cb->begin==cb->end)
        return 1;

    return 0;
}

int push_back(struct circular_buffer_t *cb, int value)
{
    if(cb==NULL || cb->begin<0 || cb->end<0 || cb->begin>=cb->capacity || cb->end>=cb->capacity || cb->ptr==NULL)
        return 1;

    *(cb->ptr+cb->end)=value;
    if(cb->full==1)
    {
        cb->begin=(cb->begin+1)%(cb->capacity);
        cb->end=(cb->end+1)%(cb->capacity);
    }
    else
    {
        cb->end=(cb->end+1)%(cb->capacity);
        if(cb->end==cb->begin)
            cb->full=1;
    }

    return 0;
}

int pop_front(struct circular_buffer_t *cb, int *err_code)
{
    if(cb==NULL || cb->begin<0 || cb->end<0 || cb->begin>=cb->capacity || cb->end>=cb->capacity || cb->ptr==NULL)
    {
        if(err_code)
            *err_code=1;
        else
            return 0;
    }

    if(empty(cb))
    {
        if(err_code!=NULL)
            *err_code=2;
        return 0;
    }

    int value=*(cb->ptr+cb->begin);
    cb->begin=(cb->begin+1)%(cb->capacity);
    cb->full=0;

    if(err_code!=NULL)
        *err_code=0;
    return value;
}

int pop_back(struct circular_buffer_t *cb, int *err_code)
{
    if(cb==NULL || cb->begin<0 || cb->end<0 || cb->begin>=cb->capacity || cb->end>=cb->capacity || cb->ptr==NULL)
    {
        if(err_code)
            *err_code=1;
        return 0;
    }

    if(empty(cb))
    {
        if(err_code)
            *err_code=2;
        return 0;
    }

    int value=*(cb->ptr+(cb->capacity+cb->end-1)%(cb->capacity));
    cb->end=(cb->capacity+cb->end-1)%(cb->capacity);
    cb->full=0;

    if(err_code)
        *err_code=0;
    return value;
}

void display(const struct circular_buffer_t *cb)
{
    if(cb==NULL || cb->begin<0 || cb->end<0 || cb->begin>=cb->capacity || cb->end>=cb->capacity || cb->ptr==NULL)
        return;

    if(cb->full==0)
        for(int i=cb->begin; (i%cb->capacity)!=cb->end; i++)
            printf("%d ", *(cb->ptr+(i%cb->capacity)));
    else
        for(int i=0, j=cb->begin; i<cb->capacity; i++,j++)
            printf("%d ", *(cb->ptr+(j%cb->capacity)));
}

int main()
{
    int size;
    printf("podaj wielkosc bufora: ");
    scanf("%d", &size);

    struct circular_buffer_t *cb;

    int err_code=create_circular_buffer(&cb, size);
    if(err_code==1)
    {
        printf("Error");
        return 1;
    }
    if(err_code==2)
    {
        printf("Failed to allocate memory");
        return 2;
    }

    int c, n, var;
    printf("0-wyjdz \n1-dodaj \n2-daj ostatni \n3-daj pierwszy \n4-wyswietl \n5-zapytaj czy pusty \n6-zapytaj czy pelny \n");

    while(1)
    {
        printf("wybierz mondrze: ");
        scanf("%d", &c);

        switch(c)
        {
        case 0:
            destroy_circular_buffer(&cb);
            return 0;
        case 1:
            printf("podaj nowy element: ");
            scanf("%d", &n);
            push_back(cb, n);
            break;
        case 2:
            var=pop_back(cb, &err_code);
            if(err_code==2)
                printf("Buffer is empty\n");
            else
                printf("%d\n", var);
            break;
        case 3:
            var=pop_front(cb, &err_code);
            if(err_code==2)
                printf("Buffer is empty\n");
            else
                printf("%d\n", var);
            break;
        case 4:
            if(empty(cb))
            {
                printf("Buffer is empty\n");
                break;
            }
            display(cb);
            printf("\n");
            break;
        case 5:
            printf("%d\n", empty(cb));
            break;
        case 6:
            printf("%d\n", full(cb));
            break;
        default:
            printf("Incorrect input\n");
            break;
        }
    }

    destroy_circular_buffer(&cb);
    return 0;
}
