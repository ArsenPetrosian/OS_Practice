#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char** argv)
{
    if(argc < 3)
    {
        printf("%s\n", "Invalid amount of arguments.");
        return 1;
    }

    int num_of_pipes = argc - 2;
    int pipefd[num_of_pipes][2];
    for(int i = 0; i < num_of_pipes; ++i)
    {
        if(pipe(pipefd[i]) < 0)
        {
            perror("Failed to create pipe with number ");
            printf("%d.\n", i + 1);
            return 1;
        }
    }

    for(int i = 0; i < num_of_pipes; ++i)
    {
        close(pipefd[i][0]);
        close(pipefd[i][1]);
    }

    int num_of_processes = argc - 1;
    pid_t pid[num_of_processes];

    for(int i = 0; i < num_of_processes; ++i)
    {
        pid[i] = fork();
        if(pid[i] < 0)
        {
            printf("%s%d.\n", "Failed to fork child with number ", i + 1);
            return 2;
        }
        if(pid[i] == 0)
        {
            if(i == 0)
            {
                for(int j = 1; j < argc; ++j)
                {
                    close(pipefd[j][0]);
                    close(pipefd[j][1]);
                }
                close(pipefd[i][0]);
                dup2(pipefd[i][1], STDOUT_FILENO);
                close(pipefd[i][1]);
            }
            else if(i == num_of_processes - 1)
            {
                for(int j = 0; j < num_of_pipes - 1; ++j)
                {
                    close(pipefd[j][0]);
                    close(pipefd[j][1]);
                }
                close(pipefd[i][1]);
                dup2(pipefd[i][0], STDIN_FILENO);
                close(pipefd[i][0]);
            }
            else
            {
                for(int j = 0; j < num_of_pipes; ++j)
                {
                    if(j == i - 1 || j == i)
                        continue;
                    close(pipefd[j][0]);
                    close(pipefd[j][1]);
                }
                close(pipefd[i - 1][1]);
                close(pipefd[i][0]);
                dup2(pipefd[i - 1][0], STDIN_FILENO);
                close(pipefd[i - 1][0]);
                dup2(pipefd[i][1], STDOUT_FILENO);
                close(pipefd[i][1]);
            }
            if(execlp(argv[i + 1], argv[i + 1], NULL) < 0)
            {
                printf("%s%d.\n", "Failed to execute process with number ", i + 1);
                return i + 3;
            }
        }
    }

    for(int i = 0; i < num_of_processes; ++i)
        waitpid(pid[i], NULL, 0);
    return 0;
}