#include <iostream>
#include <unistd.h>

int main (int argc, char* argv[])
{
    int pipe_AB = atoi(argv[0]);
    int pipe_CB = atoi(argv[1]);

    size_t Sended_char_count;
    size_t Received_char_count;

    while (read(pipe_AB, &Sended_char_count, sizeof(size_t)) > 0)
    {
        std::cout << "B: char count sended from program A = " << Sended_char_count << std::endl;
        read(pipe_CB, &Received_char_count, sizeof(size_t));
        std::cout << "B: char count received by program C = " << Received_char_count << std::endl;
        std::cout << std::endl;
    }
}
