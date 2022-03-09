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

int main()
{
    char* fname1;
    char* fname2;
    
    fname1 = malloc(MAX_BUF);
    if (read(0, fname1, MAX_BUF) < 0)
    {
        perror("ERROR\n");
        return 1;
    }
    int l1 = strlen(fname1);
    fname1[l1 - 1] = '\0';
    fflush(stdout);
    
    int f1 = open(fname1, O_CREAT|O_RDWR|O_APPEND, S_IRWXU);
    if (f1 < 0)
    {
        perror("Problems with file\n");
	return 1;
    }
    
    fname2 = malloc(MAX_BUF);
    if (read(0, fname2, MAX_BUF) < 0)
    {
        close(f1);
        perror("ERROR\n");
        return 1;
    }
    int l2 = strlen(fname2);
    fname2[l2 - 1] = '\0';
    fflush(stdout);
    if (strcmp(fname1, fname2) == 0)
    {
        close(f1);
        perror("The files have the same names\n");
	return 1;
    }  
      
    int f2 = open(fname2, O_CREAT|O_RDWR|O_APPEND, S_IRWXU);
    if (f2 < 0)
    {
        close(f1);
        perror("Problems with file\n");
        return 1;
    }
    
    int tmp1 = open("tmp_name1", O_CREAT|O_RDWR|O_APPEND, S_IRWXU);
    if (tmp1 < 0)
    {
        close(f1);
        close(f2);
        perror("Problems with file\n");
        return 1;
    }
    
    int tmp2 = open("tmp_name2", O_CREAT|O_RDWR|O_APPEND, S_IRWXU);
    if (tmp2 < 0)
    {
        close(f1);
        close(f2);
        remove("tmp_name1");
        perror("Problems with file\n");
        return 1;
    }
    
    char c = ' ';
    while (c != EOF)
    {
        char* str;
        str = malloc(MAX_BUF);
        int i = 0;
        c = ' ';
        bool is_eof = false;
        while (c != '\n')
        {
            if (read(0, &c, sizeof(char)) < 1)
            {
                is_eof = true;
                break;
            }
            str[i] = c;
            i++;
        }
        int l = strlen(str);
        str[l] = '\0';
        if (l-1 > 10)
        {
            write(tmp2, str, sizeof(char) * strlen(str));
        }
        else
        {
            write(tmp1, str, sizeof(char) * strlen(str));
        }
        if (is_eof)
        {
            break;
        }
    }
    
    int pid1 = fork();
    if (pid1 == -1)
    {
        close(f1);
        close(f2);
        remove("tmp_name1");
        remove("tmp_name2");
        perror("Fork error\n");
        return 1;
    }
    if (pid1 == 0)
    {
        close(tmp1);
        if (dup2(f1, 1) < 0)
        {
            close(f1);
            close(f2);
            remove("tmp_name1");
            remove("tmp_name2");
            perror("Can't dup child.out to stdout");
            return 2;
        }
        if (execl("child.out", "tmp_name1", "", NULL) == -1)
        {
            close(f1);
            close(f2);
            remove("tmp_name1");
            remove("tmp_name2");
            perror("Execl child problem");
            return 2;
        }
    }
    
    int pid2 = fork();
    if (pid2 == -1)
    {
        close(f1);
        close(f2);
        remove("tmp_name1");
        remove("tmp_name2");
        perror("Fork error\n");
        return 1;
    }  
    if (pid2 == 0)
    {
        close(tmp2);
        if (dup2(f2, 1) < 0)
        {
            close(f1);
            close(f2);
            remove("tmp_name1");
            remove("tmp_name2");
            perror("Can't dup child.out to stdout");
            return 2;
        } 
        if (execl("child.out", "tmp_name2", "", NULL) == -1)
        {
            close(f1);
            close(f2);
            remove("tmp_name1");
            remove("tmp_name2");
            perror("Execl child problem");
            return 2;
        } 
    } 
    
    close(f1);
    close(f2);
    return 0;
}
