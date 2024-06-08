//Copy with Pipe
#include"header.h"

int main(int argc,char* argv[])
{
    int mypipe[2];
    if (pipe(mypipe)==-1)
    {
        printf("Error:Pipe creation failed.\n");
        exit(-1);
    }
    FILE *src;
    src=fopen(argv[1],"r");
    if (src==NULL)
    {
        printf("Error:Could not open file '%s'.\n",argv[1]);
        exit(-1);
    }
    FILE *dest;
    dest=fopen(argv[2],"w+");
    if (dest==NULL)
    {
        printf("Error:Could not open file '%s'.\n",argv[2]);
        fclose(src);
        exit(-1);
    }
    int BUFFER_SIZE=131072;
    pid_t pid=fork();
    if (pid==0)
    {
        char buffer[BUFFER_SIZE];
        close(mypipe[0]);
        size_t input=fread(&buffer,sizeof(char),BUFFER_SIZE,src);
        while (input>0)
        {
            write(mypipe[1],&buffer,sizeof(char)*input);
            input=fread(&buffer,sizeof(char),BUFFER_SIZE,src);
        }
        close(mypipe[1]);
        exit(0);
    }
    else
    {
    // wait(NULL);
        close(mypipe[1]);
        char buffer[BUFFER_SIZE];
        ssize_t output=read(mypipe[0],&buffer,sizeof(char)*BUFFER_SIZE);
        while (output>0)
        {
            fwrite(&buffer,sizeof(char),output,dest);
            output=read(mypipe[0],&buffer,sizeof(char)*BUFFER_SIZE);
        }
        close(mypipe[0]);
        wait(NULL);
    }
    fclose(dest);
    fclose(src);
    
    return 0;
}