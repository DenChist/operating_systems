#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#define MAX_BUF 65534

typedef struct
{
    int l;
    int r;
}
data;

char* text;
int len_text;
    
char* str;
int len_str;

void* func(void* arg)
{
    
    int l = ((data*)arg)->l;
    int r = ((data*)arg)->r;

    for(int i = l; i <= r; i++)
    {
        int state = 1;
        for (int j = 0; j < len_str; j++)
        {
            if (text[i + j] != str[j])
            {
                state = 0;
                break;
            }
        }
        if(state)
        {
            printf("i: %d\n", i);
        }
    }

    pthread_exit(0);
}

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        perror("invalid args\n");
        return 1;
    }
    
    int count = atoi(argv[1]);
    if(count <= 0)
    {
        perror("ERROR\n");
        return 1;
    }
    
    text = malloc(MAX_BUF);
    if (read(0, text, MAX_BUF) < 0)
    {
        perror("ERROR\n");
        return 1;
    }
    len_text = strlen(text) - 1;
    text[len_text] = '\0';
    
    str = malloc(MAX_BUF);
    if (read(0, str, MAX_BUF) < 0)
    {
        perror("ERROR\n");
        return 1;
    }
    len_str = strlen(str) - 1;
    str[len_str] = '\0';
    
    int num = len_text - len_str + 1;
    count = count > num ? num : count;
    int celoe = num / count;
    int ostatok = num % count;
    
    
    pthread_t* id = malloc(count * sizeof(pthread_t));
    data* arg = malloc(count * sizeof(data));
    
    struct timeval start, end;
    gettimeofday(&start, NULL);
    
    int l = 0;
    int r = -1;
    for(int i = 0; i < count; i++)
    {
        l = r + 1;
        r += celoe;
        if(ostatok > 0)
        {
            r++;
            ostatok--;
        }
        arg[i].l = l;
        arg[i].r = r;
        if (pthread_create(&id[i], NULL, func, &arg[i]) != 0)
        {
            perror("Can't create a thread.\n");
        }
    }
    for (int i = 0; i < count; i++)
    {
        if (pthread_join(id[i], NULL) != 0)
        {
            perror("Can't wait for thread\n");
        }
    }
    
    gettimeofday(&end, NULL);
    
    long sec = end.tv_sec - start.tv_sec;
    long microsec = end.tv_usec - start.tv_usec;
    if (microsec < 0)
    {
        --sec;
        microsec += 1000000;
    }
    long elapsed = sec*1000000 + microsec;
    printf("time: %ld ms\n", elapsed);
    
    return 0;
}
