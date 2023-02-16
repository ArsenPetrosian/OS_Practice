#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

typedef struct msg_buffer
{
    msg_buffer(long type, char* text): msg_type(type)
    {
        strcpy(msg_text, text);
    }
    msg_buffer(): msg_type(0)
    {
        char text[2] = " ";
        strcpy(msg_text, text);
    }
    long msg_type;
    char msg_text[100];
} message_t;

bool flag = false;
void my_handler(int signo)
{
    flag = true;
}

int main()
{
    signal(SIGINT, my_handler);
    key_t key_ping = ftok("ping", 65);
    key_t key_pong = ftok("pong", 65);

    int msgid_ping = msgget(key_ping, 0666 | IPC_CREAT);
    int msgid_pong = msgget(key_pong, 0666 | IPC_CREAT);
    if(msgid_ping == -1 || msgid_pong == -1)
    {
        perror("Unable to get message queue.\n");
        return 1;
    }

    char ping[100] = "ping";
    message_t msg_ping(1, ping);
    message_t msg_pong;

    if(msgsnd(msgid_ping, &msg_ping, sizeof(msg_ping), 0) == -1)
    {
        perror("Unable to send a message.\n");
        return 1;
    }
    printf("%s\n", "Ping!");

    while(true)
    {
        int bytes_count = msgrcv(msgid_pong, &msg_pong, sizeof(message_t), 2, 0);
        if(bytes_count == -1)
        {
            perror("Uanble to receive a message.\n");
            return 1;
        }
        else if(bytes_count > 0)
        {
            char pong[5] = "pong";
            char lose[5] = "lose";
            if(flag)
            {
                char lose[5] = "lose";
                message_t msg_lose(1, lose);
                if(msgsnd(msgid_ping, &msg_lose, sizeof(msg_lose), 0) == -1)
                {
                    perror("Unable to send a message.\n");
                    return 1;
                }
                break;
            }
            if(strcmp(msg_pong.msg_text, pong) == 0)
            {
                if(msgsnd(msgid_ping, &msg_ping, sizeof(msg_ping), 0) == -1)
                {
                    perror("Unable to send a message.\n");
                    return 1;
                }
                printf("%s\n", "Ping!");
                sleep(1);
            }
            else if(strcmp(msg_pong.msg_text, lose) == 0)
            {
                printf("%s\n", "I won!");
                msgctl(msgid_ping, IPC_RMID, NULL);
                msgctl(msgid_pong, IPC_RMID, NULL);
                break;
            }
        }
    }
    return 0;
}