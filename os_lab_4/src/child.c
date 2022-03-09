#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define MAX_BUF 65534

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        perror("Wrong number of arguments\n");
        return 1;
    }
    char* tmp_name = argv[0];
    int tmp = open(tmp_name, O_RDWR|O_APPEND, S_IRWXU);
    if (tmp < 0)
    {
        perror("Problems with file\n");
        return 1;
    }
    struct stat buff;
    stat(tmp_name, &buff);
    char* buf = mmap(0, buff.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, tmp, 0);
    close(tmp);
    int i = 0;
    while (i < buff.st_size)
    {
        char* str = malloc(MAX_BUF);
        int ind = 0;
        while (buf[i] != '\n')
        {
            str[ind] = buf[i];
            ind++;
            i++;
        }
        str[ind] = '\n';
        i++;
        int l = strlen(str); 
        str[l] = '\0';
        for (int j = 0; j <= (l - 2) / 2; j++)
        {
            char k = str[l - 2 - j];
            str[l - 2 - j] = str[j];
            str[j] = k;
        }
        write(1, str, sizeof(char) * strlen(str));
    }
    close(tmp);
    remove(tmp_name);
    return 0;
}
