#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        perror("Inappropriate amount of arguments");
        return 1;
    }

    int file1 = open(argv[1], O_RDONLY, 0666);
    if(file1 < 0)
    {
        perror("Can't open the file");
        return 1;
    }

    int file2{};
    if(argc > 2)
    {
        if(argc == 3 || (argc == 4 && strcmp(argv[3], "NO") == 0))
            file2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
        else if(argc == 4 && strcmp(argv[3], "YES") == 0)
            file2 = open(argv[2], O_WRONLY, 0666);
        else
        {
            perror("Inappropriate arguments");
            return 1;
        }
    }
    else
    {
        const char* filename = "Target";
        file2 = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    }

    if(file2 < 0)
    {
        perror("Can't open the file");
        return 1;
    }

    char buffer[1024];
    ssize_t bytes;
    while((bytes = read(file1, buffer, 1024)) != 0)
    {
        lseek(file2, 0, SEEK_END);
        write(file2, buffer, bytes);
    }

    close(file1);
    close(file2);
    return 0;
}