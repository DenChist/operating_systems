#include <iostream>
#include <string>
#include <unistd.h>

int main ()
{
    int pipe_AC[2];
    int pipe_AB[2];
    int pipe_CA[2];
    int pipe_CB[2];

    pipe(pipe_AC);
    pipe(pipe_AB);
    pipe(pipe_CA);
    pipe(pipe_CB);

    pid_t id_C = fork();
    if (id_C == -1)
    {
        std::cout << "Fork error!" << std::endl;
        return -1;
    }
    else if (id_C == 0)
    {
        char AC[32];
        char CA[32];
        char CB[32];

        sprintf(AC, "%d", pipe_AC[0]);
        sprintf(CA, "%d", pipe_CA[1]);
        sprintf(CB, "%d", pipe_CB[1]);
            
        execl("./C.out", AC, CA, CB, (char*)(NULL));
    }
    else
    {
        pid_t id_B = fork();
        if (id_B == -1)
        {
            std::cout << "Fork error!" << std::endl;
            return -1;
        }
        else if (id_B == 0)
        {
            char AB[32];
            char CB[32];
            sprintf(AB, "%d", pipe_AB[0]);
            sprintf(CB, "%d", pipe_CB[0]);
            execl("./B.out", AB, CB, (char*)(NULL));
        }
        else
        {
            std::string Str;
            while (true)
            {
                std::cin >> Str;
                if(!std::cin.good()) break;
                size_t Sended_char_count = Str.size();
                uint8_t confirm;

                write(pipe_AB[1], &Sended_char_count, sizeof(size_t));
                write(pipe_AC[1], &Sended_char_count, sizeof(size_t));
                write(pipe_AC[1], Str.c_str(), Sended_char_count);

                read(pipe_CA[0], &confirm, sizeof(uint8_t));
            }
        }
    }
    return 0;
}
