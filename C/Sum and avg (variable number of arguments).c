#include <stdio.h>
#include <stdarg.h>

int stats(int *sum, float *avg, int,...);

int main()
{
    int sum1, sum2;
    float avg1, avg2;


    if(stats(&sum1, &avg1, 4, 2, 3, 4, 5))goto error;
    if(stats(&sum2, &avg2, 3, 1, 7, 12))goto error;

    printf("%d %g\n", sum1, avg1);
    printf("%d %g\n", sum2, avg2);

    return 0;

error:
    printf("Error");
    return 1;
}

int stats(int *sum, float *avg, int num,...)
{
    if(!sum || !avg || num<=0)return 1;

    *sum = 0;
    *avg = 0;
    va_list ap;
    va_start(ap, num);

    for(int i=0; i<num; i++)
        *sum += va_arg(ap, int);

    va_end(ap);
    *avg = (float)*sum/num;

    return 0;
}
