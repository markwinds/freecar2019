#include "common.h"
#include "include.h"

void analysisCommand1(char *argv[])
{
}

void analysisCommand2(char *argv[])
{
    if (!strcmp("get", argv[0]))
    {
        if (!strcmp("long", argv[1]))
        {
            printf("successful\n");
        }
    }
}

void analysisCommand3(char *argv[])
{
}

void analysisCommand()
{
    int argc = 0;
    char *argv[10];
    char command_buff[1024];
    strcpy(command_buff, uart4_buff);
    char *buf = command_buff;
    while ((argv[argc] = strtok(buf, " ")) != NULL)
    {
        argc++;
        buf = NULL;
    }
    switch (argc)
    {
    case 1: //命令参数个数为1
        analysisCommand1(argv);
        break;
    case 2: //命令参数个数为2
        analysisCommand2(argv);
        break;
    case 3: //命令参数个数为3
        analysisCommand3(argv);
        break;
    default:
        break;
    }
}
