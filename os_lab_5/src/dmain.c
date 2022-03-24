#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main()
{
    char* (*translation) (long x) = NULL;
    int* (*Sort) (int* array) = NULL;
    
    void *handle1 = dlopen("./libd1.so", RTLD_LAZY);
    if (!handle1)
    {
        printf("%s\n", dlerror());
        exit(1);
    }
    void *handle2 = dlopen("./libd2.so", RTLD_LAZY);
    if (!handle2)
    {
        printf("%s\n", dlerror());
        exit(2);
    }
    
    int lib = 1;
    int cmd = 0;
    char *error;
    while (scanf("%d", &cmd) != EOF)
    {
        switch(cmd)
        {
        case 0:
            lib = 3 - lib;
            printf("lib was changed to %d\n", lib);
            break;
        case 1:
            if (lib == 1)
            {
                translation = dlsym(handle1, "translation");
            }
            else
            {
                translation = dlsym(handle2, "translation");
            }

            if ((error = dlerror()) != NULL)
            {
                printf("dlsym error\n");
                exit(3);
            }

            printf("translation\n");
            long a;
            scanf("%ld", &a);
            char* str = translation(a);
            printf("%s\n", str);
            free(str);
            break;
        case 2:
            if (lib == 1)
            {
                Sort = dlsym(handle1, "Sort");
            }
            else
            {
                Sort = dlsym(handle2, "Sort");
            }

            if ((error = dlerror()) != NULL)
            {
                printf("dlsym error\n");
                exit(4);
            }
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

    if (dlclose(handle1) != 0)
    {
        perror("dlclose error");
        exit(5);
    }
    if (dlclose(handle2) != 0)
    {
        perror("dlclose error");
        exit(6);
    }

    return 0;
}
