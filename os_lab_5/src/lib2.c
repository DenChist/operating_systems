#include <stdlib.h>

char* translation(long x)
{
    char bin[256];
    int len = 0;
    if(x == 0)
    {
        bin[len] = '0';
        len++;
    }
    while (x > 0)
    {
        bin[len] = '0' + (x % 3);
        len++;
        x /= 3;
    }
    
    char* res = (char*) malloc((len + 1) * sizeof(char));
    res[len] = '\0';
    for(int i = len - 1; i >= 0; i--)
    {
        res[i] = bin[len - 1 - i];
    }
    
    return res;
}

static void __sort(int* a, int first, int last)
{
    int i = first, j = last;
    int tmp, x = a[(first + last) / 2];
    do 
    {
        while (a[i] < x)
        {
            i++;
        }
        while (a[j] > x)
        {
            j--;
        }
        if (i <= j) 
        {
            if (i < j)
            {
                tmp = a[i];
                a[i] = a[j];
                a[j] = tmp;
            }
            i++;
            j--;
        }
    } while (i <= j);
    if (i < last)
    {
        __sort(a, i, last);
    }
    if (first < j)
    {
        __sort(a, first, j);
    }
}

int* Sort(int* array)
{
    __sort(array, 1, array[0]);
    return array;
}
