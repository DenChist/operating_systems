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
        bin[len] = '0' + (x % 2);
        len++;
        x /= 2;
    }
    
    char* res = (char*) malloc((len + 1) * sizeof(char));
    res[len] = '\0';
    for(int i = len - 1; i >= 0; i--)
    {
        res[i] = bin[len - 1 - i];
    }
    
    return res;
}

int* Sort(int* array)
{
    for (int i = 1; i < array[0] + 1; ++i)
    {
        for (int j = 1; j < array[0]; ++j)
        {
            if (array[j] > array[j + 1])
            {
                int a = array[j];
                array[j] = array[j + 1];
                array[j + 1] = a;
            }
        }
    }
    return array;
}
