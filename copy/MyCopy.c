//Usual Copy
#include"header.h"


void copy(FILE *src,FILE *dest,int BUFFER_SIZE)
{
    char buffer[BUFFER_SIZE];
    size_t input=fread(&buffer,sizeof(char),BUFFER_SIZE,src);
    while (input>0)
    {
        fwrite(&buffer,sizeof(char),input,dest);
        input=fread(&buffer,sizeof(char),BUFFER_SIZE,src);
    }
}

int main(int argc,char* argv[])
{
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
    copy(src,dest,BUFFER_SIZE);
  //  printf("Average time taken to copy the file is %f ms\n",sum/20);
    fclose(dest);
    fclose(src);
    return 0;
}