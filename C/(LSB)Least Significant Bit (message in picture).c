#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define SIZE 32
#define SIZE_OF_TEXT 1010

struct bits
{
    unsigned char bit1: 1;
    unsigned char bit2: 1;
    unsigned char bit3: 1;
    unsigned char bit4: 1;
    unsigned char bit5: 1;
    unsigned char bit6: 1;
    unsigned char bit7: 1;
    unsigned char bit8: 1;
};

union bit_set
{
    unsigned int c: 8;
    struct bits bitArray;
};

union txt_bit_set
{
    unsigned int c: 8;
    struct bits bitArray;
};

int encode(const char *input, char *txt, const char *output)
{
    if(input==NULL || txt==NULL || output==NULL)return 4;

    FILE *fp_input, *fp_output;
    fp_input=fopen(input, "r");
    if(fp_input==NULL)return 1;
    fp_output=fopen(output, "w");
    if(fp_output==NULL)
    {
        fclose(fp_input);
        return 2;
    }


    int byte_value, tmp=0, c;
    union bit_set byte;
    union txt_bit_set txt_byte;
    for(unsigned int i=0;; i++)
    {
        if(tmp==1)break;
        if(i<strlen(txt))txt_byte.c=*(txt+i);

        for(int j=0; j<8; j++)
        {
            byte_value=0;
            c=fgetc(fp_input);
            if(c==EOF && i<strlen(txt))
            {
                fclose(fp_input);
                fclose(fp_output);
                return 3;
            }

            if(c=='\n')
                while(c=='\n')
                {
                putc('\n', fp_output);
                c=fgetc(fp_input);

                }
            if(c==EOF)
            {
                tmp=1;
                break;
            }

            while(c!=' ' && c!=EOF && c!='\n')//wczytywanie wartosci bajtu zapisanego w pliku txt do zmiennej byte_value
            {
                byte_value*=10;
                byte_value+=c-48;
                c=fgetc(fp_input);
            }

            if(byte_value>255)
            {
                fclose(fp_input);
                fclose(fp_output);
                return 3;
            }

            byte.c=byte_value;
            //zmiana ostatniego bitu bajtu
            if(i<strlen(txt))
            {
                if(j==0){if(txt_byte.bitArray.bit8!=byte.bitArray.bit1)byte.bitArray.bit1=txt_byte.bitArray.bit8;}
                else if(j==1){if(txt_byte.bitArray.bit7!=byte.bitArray.bit1)byte.bitArray.bit1=txt_byte.bitArray.bit7;}
                else if(j==2){if(txt_byte.bitArray.bit6!=byte.bitArray.bit1)byte.bitArray.bit1=txt_byte.bitArray.bit6;}
                else if(j==3){if(txt_byte.bitArray.bit5!=byte.bitArray.bit1)byte.bitArray.bit1=txt_byte.bitArray.bit5;}
                else if(j==4){if(txt_byte.bitArray.bit4!=byte.bitArray.bit1)byte.bitArray.bit1=txt_byte.bitArray.bit4;}
                else if(j==5){if(txt_byte.bitArray.bit3!=byte.bitArray.bit1)byte.bitArray.bit1=txt_byte.bitArray.bit3;}
                else if(j==6){if(txt_byte.bitArray.bit2!=byte.bitArray.bit1)byte.bitArray.bit1=txt_byte.bitArray.bit2;}
                else if(j==7){if(txt_byte.bitArray.bit1!=byte.bitArray.bit1)byte.bitArray.bit1=txt_byte.bitArray.bit1;}
            }
            else byte.bitArray.bit1=0;


            //wpisanie bajtu do pliku txt
            char str[4];
            sprintf(str, "%u", byte.c); //zamiana int na string
            for(int k=0; *(str+k)!='\0'; k++)putc(*(str+k), fp_output);
            if(c!=EOF)
            {
                if(c!='\n')putc(' ', fp_output);
                else putc('\n', fp_output);
            }

            if(c==EOF && j==7 && i<strlen(txt)-1)
            {
                fclose(fp_input);
                fclose(fp_output);
                return 3;
            }
            if(c==EOF)
            {
                tmp=1;
                break;
            }
        }

    }

    fclose(fp_input);
    fclose(fp_output);

    return 0;
}

int decode(const char * filename, char *txt, int size)
{
    if(filename==NULL || txt==NULL || size<=0)return 2;

    FILE *fp=fopen(filename, "r");
    if(fp==NULL)return 1;

    int c=0, byte_value, i;
    union txt_bit_set byte;
    union txt_bit_set txt_byte;
    for(i=0; i<size-1 && c!=EOF; i++)
    {

        for(int j=0; j<8; j++)
        {
            byte_value=0;
            c=fgetc(fp);
            if(c==EOF)
            {
                fclose(fp);
                return 3;
            }


            while(c!=' ' && c!=EOF)//wczytywanie wartosci bajtu zapisanego w pliku txt do zmiennej byte_value
            {
                byte_value*=10;
                byte_value+=c-48;
                c=fgetc(fp);
            }

            if(byte_value>255)
            {
                fclose(fp);
                return 3;
            }

            byte.c=byte_value;

           //zapisanie ostatniego bitu bajtu do unii
           if(j==0)txt_byte.bitArray.bit8=byte.bitArray.bit1;
           else if(j==1)txt_byte.bitArray.bit7=byte.bitArray.bit1;
           else if(j==2)txt_byte.bitArray.bit6=byte.bitArray.bit1;
           else if(j==3)txt_byte.bitArray.bit5=byte.bitArray.bit1;
           else if(j==4)txt_byte.bitArray.bit4=byte.bitArray.bit1;
           else if(j==5)txt_byte.bitArray.bit3=byte.bitArray.bit1;
           else if(j==6)txt_byte.bitArray.bit2=byte.bitArray.bit1;
           else if(j==7)txt_byte.bitArray.bit1=byte.bitArray.bit1;


            if(c==EOF && j!=7)
            {
                fclose(fp);
                return 3;
            }
        }

        *(txt+i)=txt_byte.c;

    }
    *(txt+i)='\0';

    fclose(fp);

    return 0;
}


int main()
{
    char c[3];
    printf("e - encode, d - decode: ");

    fgets(c, 3, stdin);

    if((*c=='e' || *c=='E') && *(c+1)=='\n')
    {
        char input[SIZE], output[SIZE], txt[1010];

        printf("podaj tekst do zakodowania: ");
        fgets(txt, 1010, stdin);
        *(txt+strlen(txt)-1)='\0';
        printf("podaj plik obrazu w formie txt: ");
        fgets(input, SIZE-1, stdin);
        *(input+strlen(input)-1)='\0';
        printf("podaj plik wyjsciowy: ");
        fgets(output, SIZE-1, stdin);
        *(output+strlen(output)-1)='\0';

        int err_code=encode(input, txt, output);
        if(err_code==1)
        {
            printf("File not found");
            return 1;
        }
        if(err_code==2)
        {
            printf("Couldn't create file");
            return 2;
        }
        if(err_code==3)
        {
            printf("File corrupted");
            return 3;
        }
        if(err_code==0)
        {
            printf("File saved");
            return 0;
        }
    }
    else if((*c=='d' || *c=='D') && *(c+1)=='\n')
    {
        char filename[SIZE], txt[SIZE_OF_TEXT];

        printf("podaj nazwe pliku z obrazem: ");
        fgets(filename, SIZE-1, stdin);
        *(filename+strlen(filename)-1)='\0';

        int err_code=decode(filename, txt, SIZE_OF_TEXT);
        if(err_code==1)
        {
            printf("File not found");
            return 1;
        }
        if(err_code==3)
        {
            printf("File corrupted");
            return 3;
        }
        if(err_code==0)
        {
            printf("%s", txt);
            return 0;
        }
    }
    else
    {
        printf("Incorrect input");
        return 0;
    }
}
