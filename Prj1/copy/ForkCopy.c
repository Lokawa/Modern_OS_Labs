//Copy with Fork
#include "header.h"
int main(int argc,char* argv[])
{
    pid_t pid=fork();
    if (pid==0)
    {
     //   printf("Child process created.\n");
        execl("./MyCopy",argv[0],argv[1],argv[2],NULL);
    }
    else
    {
        //printf("Parent process.\n");
        wait(NULL);
    }
    return 0;
}