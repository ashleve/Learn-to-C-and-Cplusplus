#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 501

int isOK(char c)
{
    if(isdigit(c) || c=='-' || c=='+')return 1;
    return 0;
}

int validate(const char* number)    //sprawdz poprawnosc liczby
{
    if(number==NULL)return 2;
    if(strlen(number)==0)return 1;

    int i=0;
    if(*number=='-'){if(!isdigit(*(number+1)))return 1;}
    if(*number=='-')i++;
    if(*number=='0' && *(number+1)!='\0')return 1;

    while(*(number+i)!='\0')
    {
        if(!isdigit(*(number+i)))return 1;
        i++;
    }

    return 0;
}
int validate_expression(const char *expr)   //sprawdz poprawnosc wyrazenia (troche bez sensu ale dziala)
{
    if(expr==NULL)return 2;
    if(*expr=='\0' || *expr=='+' || *expr=='*')return 1;
    if(!isOK(*expr) && *expr!='*')return 1;

    int i=0;
    if(*expr=='-')
    {
        if(!isdigit(*(expr+1)))return 1;

    }

    for(i=0;*(expr+i+1)!='\0';i++)
    {
        if(*(expr+i+1)=='*' && *(expr+i+2)=='\0')return 1;
        if(!isOK(*(expr+i)) && *(expr+i)!='*')return 1;
        if((*(expr+i)=='+' && *(expr+i+1)=='-'))
        {
            if(!isdigit(*(expr+i+2)))return 1;
        }
        if(*(expr+i)=='*' && (*(expr+i+1)=='+' || *(expr+i+1)=='*'))return 1;
        if((*(expr+i)=='+' || *(expr+i)=='-') && (*(expr+i+1)=='+' || *(expr+i+1)=='*'))return 1;
        if(isdigit(*(expr+i)))
        {
            if(*(expr+i+1)=='-' || *(expr+i+1)=='+')
            {
                int flag=0;
                for(int k=i+1;*(expr+k)!='\0';k++)
                 {

                   if(isdigit(*(expr+k)))
                    {
                       flag=1;
                       break;
                    }
                 }
                    if(flag!=1)return 1;
            }
        }
    }

    return 0;
}

int compare(const char *number1, const char *number2)   //okresl ktora liczba wieksza
{
    if(number1==NULL || number2==NULL)return 2;
    if(validate(number1) || validate(number2))return 3;

    int i=0,flag=0;
    if(*number1=='-' && *number2!='-')return -1;
    else if(*number2=='-' && *number1!='-')return 1;
    if(*number2=='-' && *number1=='-')
    {
        i++;
        flag=1;
    }

    if(strlen(number1)>strlen(number2) && flag)return -1;
    if(strlen(number1)<strlen(number2) && flag)return 1;
    if(strlen(number1)>strlen(number2) && !flag)return 1;
    if(strlen(number1)<strlen(number2) && !flag)return -1;

    while(*(number1+i)==*(number2+i) && *(number1+i)!='\0' && *(number2+i)!='\0')i++;
    if(*(number1+i)=='\0' && *(number2+i)!='\0' && !flag)return -1;
    if(*(number1+i)=='\0' && *(number2+i)!='\0' && flag)return 1;
    if(*(number1+i)!='\0' && *(number2+i)=='\0' && !flag)return 1;
    if(*(number1+i)!='\0' && *(number2+i)=='\0' && flag)return -1;

    if(*(number1+i)>*(number2+i) && !flag)return 1;
    if(*(number1+i)<*(number2+i) && !flag)return -1;
    if(*(number1+i)>*(number2+i) && flag)return -1;
    if(*(number1+i)<*(number2+i) && flag)return 1;

    return 0;
}

int compare_without_sign(const char *number1, const char *number2)  //okresl ktora wieksza bez znakow
{
    int i=0,j=0;
    int size1=strlen(number1),size2=strlen(number2);
    if(*number1=='-'){i++;size1--;}
    if(*number2=='-'){j++;size2--;}
    if(size1>size2)return 1;
    if(size1<size2)return 0;

    while(*(number1+i)==*(number2+j) && *(number1+i)!='\0' && *(number2+j)!='\0')
    {
        i++;
        j++;
    }

    if(*(number1+i)=='\0' && *(number2+j)!='\0')return 0;
    if(*(number1+i)!='\0' && *(number2+j)=='\0')return 1;

    if(*(number1+i)>*(number2+j))return 1;
    if(*(number1+i)<*(number2+j))return 0;

    if(*number1=='-' && *number2!='-')return -1;
    if(*number1!='-' && *number2=='-')return -1;

    return 1;
}

void deleteZeros(char *ptr)
{
    int i=0;
    while(*(ptr+i)=='0')i++;    //policz ile zer na poczatku

    int size=strlen(ptr);

    for(int j=i,k=0; j<=size; j++,k++)  //usun zera
        *(ptr+k)=*(ptr+j);
}
void deleteSign(char *ptr)
{
    int i=0;
    while(*(ptr+i)=='-')i++;    //sprawdz czy minus na poczatku

    int size=strlen(ptr);

    for(int j=i,k=0; j<=size; j++,k++)  //usun minusy
        *(ptr+k)=*(ptr+j);
}

void moveByOne(char *ptr)//przesun ciag o 1 pozycje do przodu
{
    for(int i=strlen(ptr); i>=0; i--)
        *(ptr+i+1)=*(ptr+i);
}

void moveByX(char *ptr, int X)//przesun ciag o X pozycji do przodu
{
    for(int i=strlen(ptr); i>=0; i--)
        *(ptr+i+X)=*(ptr+i);
}

void give_me_subtract_result(const char *number1, const char *number2, char *result)   //zawsze odejmujemy number2 od number1 (abs(number1) musi byc wieksze od abs(number2))
{
    for(int i=strlen(number1);i>=0;i--)*(result+i)='\0';   //wypelnij terminatorami '\0' na wszelki wypadek

    int carry=0,digit;
    for(int i=strlen(number1)-1,j=strlen(number2)-1; i>=0; i--,j--)
    {
        if(*(number1+i)=='-'){*(result+i)='0';continue;}   //przypadek gdy number1 ma znak "-"

        if(j>=0)
        {
            if(*(number2+j)=='-')digit=(int)*(number1+i)-48-carry;     //przypadek gdy number2 ma znak "-"
            else digit=(int)*(number1+i)-(int)*(number2+j)-carry;
        }
        else digit=((int)*(number1+i)-48)-carry;

        if(digit<0)
        {
            digit+=10;
            carry=1;
        }
        else carry=0;

        *(result+i)=(char)(digit+48);
    }

    deleteZeros(result);
}
void give_me_add_result(const char *number1, const char *number2, char *result)  //dziala tylko gdy  abs(number1) wiekszy niz abs(number2)
{
    int carry=0,digit;
    for(int i=strlen(number1)-1,j=strlen(number2)-1; i>=0; i--,j--)
    {
        if(*(number1+i)=='-')   //przypadek gdy number1 ma znak "-"
        {
            if(carry)*(result+i)='1';
            else *(result+i)='0';
            carry=0;
            continue;
        }

        if(j>=0)
        {
            if(*(number2+j)=='-')digit=(int)*(number1+i)+carry-48;    //przypadek gdy number2 ma znak "-"
            else digit=(int)*(number1+i)-48+(int)*(number2+j)-48+carry;
        }
        else digit=((int)*(number1+i)-48)+carry;

        if(digit>9)
        {
            digit-=10;
            carry=1;
        }
        else carry=0;

        *(result+i)=(char)(digit+48);
    }
    *(result+strlen(number1))='\0';

    if(carry)
    {
        moveByOne(result);
        *result='1';
    }
    deleteZeros(result);
}

void add_number_to_result(char *result, int n, int position_shift)  //funkcja pomocnicza do give_me_multiplication_result()
{
    if(*result=='-')*result='0';    //usun potencjalny minus

    int length=strlen(result)-1;
    if(length<=position_shift)   //zrob miejsce na dodanie n na odpowiednia pozycje (przesuwamy liczbe w prawo i wypelniamy zerami puste miejsca)
    {
        moveByX(result, position_shift-length+1);
        for(int i=0;i<=position_shift-length; i++)
            *(result+i)='0';

    }

    int carry=0,digit;
    length=strlen(result)-1;
    for(int i=length-position_shift; i>=0; i--) //dodanie liczby do result od danej pozycji
    {
        digit=(int)*(result+i)+carry-48+(n%10);
        if(digit>9)
        {
            digit-=10;
            carry=1;
        }
        else carry=0;

        *(result+i)=(char)(digit+48);
        n/=10;
    }
    if(carry)
    {
        moveByOne(result);
        *result='1';
    }
    deleteZeros(result);
}
void give_me_multiplication_result(const char *number1, const char *number2, char *result)
{
    for(unsigned int i=0;i<strlen(number1)+strlen(number2)+1;i++)  //wypelinj zerami zeby result dalo sie dodawac
        *(result+i)='0';
    *(result+1)='\0';

    int position=0;
    for(int j=strlen(number2)-1; j>=0; j--)
    {
        if(*(number2+j)=='-')continue;

        int position_shift=position,tmp;
        for(int i=strlen(number1)-1; i>=0; i--)
        {
            if(*(number1+i)=='-')continue;

            tmp=((int)*(number2+j)-48)*((int)*(number1+i)-48);
            add_number_to_result(result,tmp,position_shift);

            position_shift++;
        }
        position++;
    }
}

int subtract(const char* number1, const char* number2, char** result)
{
    if(number1==NULL || number2==NULL || result==NULL)return 1;
    if(validate(number1) || validate(number2))return 2;

    //alokacja pamieci
    int size;
    if(compare_without_sign(number1,number2))size=strlen(number1);
    else size=strlen(number2);
    *result=(char*)malloc((size+3)*sizeof(char));
    if(*result==NULL)return 3;

    //sprawdzenie znakow
    int num1_sign;
    if(*number1=='-')num1_sign=0;
    else num1_sign=1;
    int num2_sign;
    if(*number2=='-')num2_sign=0;
    else num2_sign=1;

    if(compare(number1,number2)==0)
    {
        **result='0';
        *(*result+1)='\0';
    }
    /////////////////////////
    else if(compare(number1,number2)==1 && num1_sign==1 && num2_sign==1)
    {
        give_me_subtract_result(number1,number2,*result);
    }
    else if(compare(number1,number2)==-1 && num1_sign==1 && num2_sign==1)
    {
        give_me_subtract_result(number2,number1,*result);
        moveByOne(*result);
        **result='-';
    }
    /////////////////////////
    else if(num1_sign==1 && num2_sign==0 && compare_without_sign(number1,number2))
    {
        give_me_add_result(number1,number2,*result);
    }
    else if(num1_sign==1 && num2_sign==0 && !compare_without_sign(number1,number2))
    {
        give_me_add_result(number2,number1,*result);
    }
    /////////////////////////
    else if(num1_sign==0 && num2_sign==1 && compare_without_sign(number1,number2))
    {
        give_me_add_result(number1,number2,*result);
        moveByOne(*result);
        **result='-';
    }
    else if(num1_sign==0 && num2_sign==1 && !compare_without_sign(number1,number2))
    {
        give_me_add_result(number2,number1,*result);
        moveByOne(*result);
        **result='-';
    }
    /////////////////////////
    else if(compare(number1,number2)==-1 && num1_sign==0 && num2_sign==0)
    {
        give_me_subtract_result(number1,number2,*result);
        moveByOne(*result);
        **result='-';
    }
    else if(compare(number1,number2)==1 && num1_sign==0 && num2_sign==0)
    {
        give_me_subtract_result(number2,number1,*result);
    }
    /////////////////////////
    else printf("I have no idea what you want me to do\n");

    return 0;
}
int add(const char* number1, const char* number2, char** result)
{
    if(number1==NULL || number2==NULL || result==NULL)return 1;
    if(validate(number1) || validate(number2))return 2;

    //alokacja pamieci
    int size;
    if(compare_without_sign(number1,number2))size=strlen(number1);
    else size=strlen(number2);
    *result=(char*)malloc((size+2)*sizeof(char));
    if(*result==NULL)return 3;

    //sprawdzenie znakow
    int num1_sign;
    if(*number1=='-')num1_sign=0;
    else num1_sign=1;
    int num2_sign;
    if(*number2=='-')num2_sign=0;
    else num2_sign=1;

    if(*number1=='0' && *number2=='0')
    {
        **result='0';
        *(*result+1)='\0';
    }
    else if(compare_without_sign(number1,number2)==-1)
    {
        **result='0';
        *(*result+1)='\0';
    }
    else if(compare_without_sign(number1,number2) && num1_sign==1 && num2_sign==1)
    {
        give_me_add_result(number1,number2,*result);
    }
    else if(!compare_without_sign(number1,number2) && num1_sign==1 && num2_sign==1)
    {
        give_me_add_result(number2,number1,*result);
    }
    /////////////////////////
    else if(num1_sign==1 && num2_sign==0 && compare_without_sign(number1,number2))
    {
        give_me_subtract_result(number1,number2,*result);
    }
    else if(num1_sign==1 && num2_sign==0 && !compare_without_sign(number1,number2))
    {
        give_me_subtract_result(number2,number1,*result);
        moveByOne(*result);
        **result='-';
    }
    /////////////////////////
    else if(num1_sign==0 && num2_sign==1 && compare_without_sign(number1,number2))
    {
        give_me_subtract_result(number1,number2,*result);
        moveByOne(*result);
        **result='-';
    }
    else if(num1_sign==0 && num2_sign==1 && !compare_without_sign(number1,number2))
    {
        give_me_subtract_result(number2,number1,*result);
    }
    /////////////////////////
    else if(compare(number1,number2)==-1 && num1_sign==0 && num2_sign==0)
    {
        give_me_add_result(number1,number2,*result);
        moveByOne(*result);
        **result='-';
    }
    else if(compare(number1,number2)==1 && num1_sign==0 && num2_sign==0)
    {
        give_me_add_result(number2,number1,*result);
        moveByOne(*result);
        **result='-';
    }
    /////////////////////////
    else printf("I have no idea what you want me to do\n");

    return 0;
}

int multiply(const char* number1, const char* number2, char** result)
{
    if(number1==NULL || number2==NULL || result==NULL)return 1;
    if(validate(number1) || validate(number2))return 2;

    //alokacja pamieci
    unsigned int size;
    size=strlen(number1)+strlen(number2)+3;
    *result=malloc(size*sizeof(char));
    if(*result==NULL)return 3;

    //sprawdzenie znakow
    int num1_sign;
    if(*number1=='-')num1_sign=0;
    else num1_sign=1;
    int num2_sign;
    if(*number2=='-')num2_sign=0;
    else num2_sign=1;


    if(*number1=='0' || *number2=='0')
    {
        **result='0';
        *(*result+1)='\0';
    }
    else if(num1_sign==1 && num2_sign==1 && compare_without_sign(number1,number2) )
    {
        give_me_multiplication_result(number1,number2,*result);
    }
    else if(num1_sign==1 && num2_sign==1 && !compare_without_sign(number1,number2) )
    {
        give_me_multiplication_result(number2,number1,*result);
    }
    /////////////////////////
    else if(num1_sign==1 && num2_sign==0 && compare_without_sign(number1,number2))
    {
        give_me_multiplication_result(number1,number2,*result);
         moveByOne(*result);
        **result='-';
    }
    else if(num1_sign==1 && num2_sign==0 && !compare_without_sign(number1,number2))
    {
        give_me_multiplication_result(number2,number1,*result);
        moveByOne(*result);
        **result='-';
    }
    /////////////////////////
    else if(num1_sign==0 && num2_sign==1 && compare_without_sign(number1,number2))
    {
        give_me_multiplication_result(number1,number2,*result);
        moveByOne(*result);
        **result='-';
    }
    else if(num1_sign==0 && num2_sign==1 && !compare_without_sign(number1,number2))
    {
        give_me_multiplication_result(number2,number1,*result);
        moveByOne(*result);
        **result='-';
    }
    /////////////////////////
    else if(num1_sign==0 && num2_sign==0 && compare_without_sign(number1,number2))
    {
        give_me_multiplication_result(number1,number2,*result);
        deleteSign(*result);
    }
    else if(num1_sign==0 && num2_sign==0 && !compare_without_sign(number1,number2))
    {
        give_me_multiplication_result(number2,number1,*result);
        deleteSign(*result);
    }
    /////////////////////////
    else printf("I have no idea what you want me to do\n");

    return 0;
}

int calculate(const char* expr, char **result)
{
    if(expr==NULL || result==NULL)return 1;
    if(validate_expression(expr)!=0)return 2;

    *result=malloc(2*sizeof(char));
    if(*result==NULL)return 3;
    **result='0';
    *(*result+1)='\0';

    for(int i=0;*(expr+i)!='\0'; i++)
    {
        if(*(expr+i)=='+')
        {
            i++;
            int ind=i,length=0;
            if(*(expr+ind)=='-')    //liczba moze byc ujemna wiec kopiujemy minus
            {
                ind++;
                length++;
            }
            while(isdigit(*(expr+ind))) //zliczanie dlugosci liczby
            {
                ind++;
                length++;
            }

            char *number=malloc((length+1)*sizeof(char));   //alokacja pamieci na nowa liczbe
            if(number==NULL)
            {
                free(*result);
                return 3;
            }

            int j;
            for(j=0; j<length; j++,i++) //kopiowanie liczby z expr do zaalokowanego wskaznika
                *(number+j)=*(expr+i);
            *(number+j)='\0';
            i--;

            char *ptr;
            int err_code=add(*result,number,&ptr);  //znajdz wynik dzialania i przypisz do ptr
            if(err_code==3)
            {
                free(number);
                free(*result);
                return 3;
            }

            *result=realloc(*result, (strlen(ptr)+1)*sizeof(char)); //powieksz pamiec result jesli jest za malo na wynik
            for(unsigned int k=0;k<=strlen(ptr);k++)    //przypisz wynik do result
                *(*result+k)=*(ptr+k);

            free(ptr);
            free(number);
        }
        else if(*(expr+i)=='-')
        {
            i++;
            int ind=i,length=0;
            if(*(expr+ind)=='-')
            {
                ind++;
                length++;
            }
            while(isdigit(*(expr+ind)))
            {
                ind++;
                length++;
            }

            char *number=malloc((length+1)*sizeof(char));
            if(number==NULL)
            {
                free(*result);
                return 3;
            }

            int j;
            for(j=0; j<length; j++,i++)
                *(number+j)=*(expr+i);
            *(number+j)='\0';
            i--;

            char *ptr;
            int err_code=subtract(*result,number,&ptr);
            if(err_code==3)
            {
                free(number);
                free(*result);
                return 3;
            }

            *result=realloc(*result, (strlen(ptr)+1)*sizeof(char));
            for(unsigned int k=0;k<=strlen(ptr);k++)
                *(*result+k)=*(ptr+k);

            free(ptr);
            free(number);
        }
        else if(*(expr+i)=='*')
        {
            i++;
            int ind=i,length=0;
            if(*(expr+ind)=='-')
            {
                ind++;
                length++;
            }
            while(isdigit(*(expr+ind)))
            {
                ind++;
                length++;
            }

            char *number=malloc((length+1)*sizeof(char));
            if(number==NULL)
            {
                free(*result);
                return 3;
            }

            int j;
            for(j=0; j<length; j++,i++)
                *(number+j)=*(expr+i);
            *(number+j)='\0';
            i--;

            char *ptr;
            int err_code=multiply(*result,number,&ptr);
            if(err_code==3)
            {
                free(number);
                free(*result);
                return 3;
            }

            *result=realloc(*result, (strlen(ptr)+1)*sizeof(char));
            for(unsigned int k=0;k<=strlen(ptr);k++)
                *(*result+k)=*(ptr+k);

            free(ptr);
            free(number);
        }
        else
        {
            int ind=i,length=0;
            if(*(expr+ind)=='-')
            {
                ind++;
                length++;
            }
            while(isdigit(*(expr+ind)))
            {
                ind++;
                length++;
            }

            char *number=malloc((length+1)*sizeof(char));
            if(number==NULL)
            {
                free(*result);
                return 3;
            }

            int j;
            for(j=0; j<length; j++,i++)
                *(number+j)=*(expr+i);
            *(number+j)='\0';

            char *ptr;
            int err_code=add(*result,number,&ptr);
            if(err_code==3)
            {
                free(number);
                free(*result);
                return 3;
            }

            *result=realloc(*result, (strlen(ptr)+1)*sizeof(char));
            for(unsigned int k=0;k<=strlen(ptr);k++)
                *(*result+k)=*(ptr+k);

            free(ptr);
            free(number);
            i--;
        }
    }
    return 0;
}

int main()
{
    char *expr;
    expr=malloc(SIZE*sizeof(char));
    if(expr==NULL)
    {
        printf("Failed to allocate memory");
        return 3;
    }
    printf("Podaj wyrazenie:\n");
    scanf("%500s", expr);

    char *result;
    int err_code=calculate(expr, &result);
    if(err_code==3)
    {
        printf("Failed to allocate memory");
        free(expr);
        return 3;
    }
    if(err_code==2)
    {
        printf("Incorrect input");
        free(expr);
        return 2;
    }
    printf("Wynik:\n");
    printf("%s", result);

    free(expr);
    free(result);
    return 0;
}
