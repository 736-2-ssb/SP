#include <stdio.h>
#include <stdint.h>

int16_t sumP (int16_t arr [10])
{
    int16_t sum=0;
    for (int8_t i=0;i<10;i++)
        {
            if ( arr[i]>=0)
            sum = sum + arr [i];
        }
    return sum;
}
 
int16_t sumN (int16_t arr [10])
{
    int16_t sum=0;
    for (int8_t i=0;i<10;i++)
        {
            if ( arr[i]<0)
            sum = sum + arr [i];
        }
    return sum;
}
void checkbit5 (int16_t a)
{
    if ((a&16) == 0) 
        printf ("0");
    else
        printf ("1");
}

int main ()
{
    int16_t arr[10] = {1,19,16,18,-15,17,14,13,-12,11};
    int16_t sump, sumn;
    sump = sumP (arr);
    sumn = sumN(arr);
    printf ("%d \n",sump);
    printf ("%d \n",sumn);
    checkbit5 (sumn);
    printf ("\n");
    return 0;

}