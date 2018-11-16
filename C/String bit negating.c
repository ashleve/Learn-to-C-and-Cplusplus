#include <stdio.h>
#define SIZE 102

void show(const void* ptr, size_t count)
{
    if(ptr==NULL || count==0)return;

    for(unsigned int i=0; i<count; i++)
        printf("%02x ",*((unsigned char*)ptr+i));
    printf("\n");

}
void inverse_bits(void* ptr, size_t offset, size_t count)
{
    if(ptr==NULL || count==0)return;

    for(unsigned int i=offset; i<offset+count; i++)
    {
        *((unsigned char*)ptr+i)=~(*((unsigned char*)ptr+i));
    }
}

int main()
{
    char text[SIZE];

    printf("Podaj tekst:\n");
    fgets(text, 100, stdin);
    int i;
    for(i=0; *(text+i)!='\0'; i++);
    *(text+i-1)='\0';

    size_t size=i, offset=0;
    show(text, size);
    inverse_bits(text, offset, size);
    show(text, size);

    getchar();
    return 0;
}
