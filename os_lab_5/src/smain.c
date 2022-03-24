#include <stdio.h>
#include <stdlib.h>

// Перевод числа x из десятичной системы счисления в другую
extern char* translation(long x);

// Отсортировать целочисленный массив
extern int * Sort(int * array);

int main()
{
    printf("1: translation\n");
    printf("2: Sort\n");
    int cmd = 0;
    while (scanf("%d", &cmd) != EOF)
    {
        switch(cmd)
        {
        case 1:
            printf("translation\n");
            long a;
            scanf("%ld", &a);
            char* str = translation(a);
            printf("%s\n", str);
            free(str);
            break;
        case 2:
            printf("Sort\n");
            int array[256];
            int len = 0;
            char c = '1';
            int b;
            while (c != '\n')
            {
                scanf("%d%c", &b, &c);
                len++;
                array[len] = b;
            }
            c = '1';
            array[0] = len;
            
            Sort(array);
            for (int i = 1; i <= len; i++)
            {
                printf("%d ", array[i]);
            }
            printf("\n");
            break;
        }
    }
    return 0;
}


