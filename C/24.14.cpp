#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct student_t
{
    char name[20];
    char surname[40];
    int index;
};

void show(const struct student_t* ptr, int n)
{
    if(ptr==NULL)return;

    for(int i=0; i<n; i++)
    {
        char text1[20],text2[40];
        strcpy(text1, (ptr+i)->name);
        strcpy(text2, (ptr+i)->surname);
        printf("%s %s\n", text1, text2);
    }
}

int load_data(struct student_t* ptr, int n, FILE *fp1, FILE *fp2)
{
    if(ptr==NULL || fp1==NULL || fp2==NULL)return 0;

    char name[100][20], surname[100][40];
    for(int i=0; i<100; i++)
    {
        fgets(*(name+i), 20, fp1);
        fgets(*(surname+i), 40, fp2);

        for(int j=0; *(*(name+i)+j)!='\0'; j++)if(*(*(name+i)+j)=='\n')*(*(name+i)+j)='\0';
        for(int j=0; *(*(surname+i)+j)!='\0'; j++)if(*(*(surname+i)+j)=='\n')*(*(surname+i)+j)='\0';
    }


    srand(time(NULL));
    for(int i=0; i<n; i++)
    {
        strcpy((ptr+i)->name, *(name+rand()%100));
        strcpy((ptr+i)->surname, *(surname+rand()%100));
    }


    return 1;
}

int main()
{
    struct student_t Students[100], *ptr=Students;
    int n=-1;

    FILE *fp1, *fp2;
    fp1=fopen("imiona.txt", "r");
    if(fp1==NULL)
    {
        printf("Error");
        return 1;
    }
    fp2=fopen("nazwiska.txt", "r");
    if(fp2==NULL)
    {
        printf("Error");
        fclose(fp1);
        return 1;
    }

    printf("Podaj liczbe studentow do wylosowania: ");
    scanf("%d",&n);
    if(n<=0)
    {
        printf("Error");
        fclose(fp1);
        fclose(fp2);
        return 1;
    }
    if(n>100)n=100;

    if(!load_data(ptr, n, fp1, fp2))
    {
        printf("Error");
        fclose(fp1);
        fclose(fp2);
        return 1;
    }
    fclose(fp1);
    fclose(fp2);

    show(ptr, n);

    return 0;
}
