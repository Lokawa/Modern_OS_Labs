#include "header.h"

int parseLine(char *line,char *command[],int pipe_pos[],int *pipe_count)
{
    char *p;
    int count=0;
    char *pos;
    if ((pos=strchr(line, '\n')) != NULL) *pos = '\0';
    if ((pos=strchr(line, '\r')) != NULL) *pos = '\0';
    char *save=NULL;
    p=strtok_r(line," ",&save);
    while (p)
    {
        if (p && strcmp(p,"|")==0)
        {
            pipe_pos[(*pipe_count)]=count;
            (*pipe_count)++;
            
        }
        command[count]=p;
        count++;
        p=strtok_r(NULL," ",&save);
    }
    return count;
}

void SIGCHLD_handler(int sig)
{
    int status;
    while (waitpid(-1, &status, WNOHANG) > 0);
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(2);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    if (portno <= 0 || portno > 65535) {
        fprintf(stderr, "ERROR, invalid port number\n");
        exit(3);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    printf("Waiting for client to connect\n");

    //muiltiple client

    //handle zombie process
    struct sigaction sa;
    sa.sa_handler = SIGCHLD_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

   // int client_count=0;
    while (1)
    {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("ERROR on accept");
        }
     //   client_count++;
        printf("New Client Connected.\n");

        //fork a new process to handle the client
        int clientpid=fork();
        if (clientpid==0) 
        {
            close(sockfd);
            n = write(newsockfd, "Welcome to my shell!\n", 21);
            //shell building
            char *pwd=getcwd(NULL,0);
            char *old=NULL;
            char *cyan="\033[0;36m";
            char *reset="\033[0m";
            while (1)
            {
                bzero(buffer, 256);
                n=  write(newsockfd, cyan, strlen(cyan));
                n=  write(newsockfd, pwd, strlen(pwd));
                n=  write(newsockfd, "> ", 2);
                n=  write(newsockfd, reset, strlen(reset));
                n = read(newsockfd, buffer, 255);
                if (n < 0) {
                    perror("ERROR reading from socket");
                };
               // printf("New Command Received\n");
                //tokenize the command
                char *command[100]={NULL};
                int pipe_pos[100]={0};
                int pipe_count=0;
                int command_count=parseLine(buffer,command,pipe_pos,&pipe_count);
                command[command_count]=NULL;

                //check if the command is exit
                if (strcmp(command[0], "exit") == 0) {
                    break;
                }

                //check if the command is cd
                if (strcmp(command[0], "cd") == 0) {
                    if (strcmp(command[1],"~")==0)
                    {
                        if (chdir(getenv("HOME"))<0)
                        {
                            dup2(newsockfd,STDERR_FILENO);
                            perror("Error changing directory");
                        } 
                        old=pwd;
                        pwd=getcwd(NULL,0);
                        continue;
                    }
                    else if (strcmp(command[1],"-")==0)
                    {
                        if (old)
                        {
                            chdir(old);
                            old=pwd;
                            pwd=getcwd(NULL,0);
                            continue;
                        }
                        else
                        {
                            dup2(newsockfd,STDERR_FILENO);
                            perror("Error changing directory");
                            continue;
                        }
                    } else if (strcmp(command[1],"..")==0)
                    {
                        if (chdir("..")<0)
                        {
                            dup2(newsockfd,STDERR_FILENO);
                            perror("Error changing directory");
                        }
                        old=pwd;
                        pwd=getcwd(NULL,0);
                        continue;
                    } else if (strcmp(command[1],".")==0) continue;
                    else if (chdir(command[1]) < 0) 
                    {
                        dup2(newsockfd,STDERR_FILENO);
                        perror("Error changing directory");
                    }
                    old=pwd;
                    pwd=getcwd(NULL,0);
                    continue;
                }
                // no pipe needs
                if (pipe_count==0) 
                {
                    int pid=fork();
                    if (pid==0)
                    {
                        dup2(newsockfd,STDOUT_FILENO);
                        dup2(newsockfd,STDERR_FILENO);
                        if (execvp(command[0],command)<0)
                        {
                            perror("Error executing command");
                            exit(0);
                        }
                    }
                    else 
                    {
                        int r;
                        wait(&r);
                    }
                }
                else //pipe shell/shell.c
                {
                    if (pipe_pos[0]==0 || pipe_pos[pipe_count-1]==command_count-1)
                    {
                        dup2(newsockfd,STDERR_FILENO);
                        dup2(newsockfd,STDOUT_FILENO);
                        fprintf(stderr,"Error: Invalid pipe command\n");
                        continue;
                    }
                    pipe_pos[pipe_count]=command_count;
                    int num=pipe_count+1;

                    //phrase the command
                    char *commands[100][100]={NULL};
                    for (int j=0;j<pipe_pos[0];j++)
                    {
                        commands[0][j]=command[j];
                    }
                    for (int i=1;i<num;i++)
                    {
                        for (int j=0;j<pipe_pos[i]-pipe_pos[i-1]-1;j++)
                        {
                            commands[i][j]=command[pipe_pos[i-1]+j+1];
                        }
                        commands[i][pipe_pos[i]-pipe_pos[i-1]-1]=NULL;
                    }
                    //create pipe
                    int pipes[100][2];
                    for (int i=0;i<pipe_count;i++)
                    {
                        if (pipe(pipes[i])<0)
                        {
                            perror("Error creating pipe");
                            exit(0);
                        }
                    }

                    //fork and execute
                    for (int i=0;i<num;i++)
                    {
                        int pid=fork();
                        if (pid==0)
                        {
                            //redirect input and output
                            if (i==pipe_count)
                            {
                                dup2(newsockfd,STDOUT_FILENO);
                                dup2(newsockfd,STDERR_FILENO);
                            }
                            if (i!=0)
                            {
                            // close(0);
                                dup2(pipes[i-1][0],STDIN_FILENO);
                            // close(pipes[i-1][0]);
                            }
                            if (i!=pipe_count)
                            {
                            //  close(1);
                                dup2(pipes[i][1],STDOUT_FILENO);
                            // close(pipes[i][1]);
                            }

                            //close unused pipes
                            for (int j=0;j<pipe_count;j++)
                            {
                                close(pipes[j][0]);
                                close(pipes[j][1]);
                            }

                            if (execvp(commands[i][0],commands[i])<0)
                            {
                                dup2(newsockfd,STDERR_FILENO);
                                perror("Error executing command");
                                printf("Error executing command %s\n",commands[i][0]);
                                exit(0);
                            }
                        }
                    }
                    
                    //close all pipes
                    for (int i=0;i<pipe_count;i++)
                    {
                        close(pipes[i][0]);
                        close(pipes[i][1]);
                    }

                    //wait for all child process
                    int r;
                    for (int i=0;i<num;i++)
                    {
                        wait(&r);
                    }
                }
            }
        //    client_count--;
            printf("Client Disconnected.\n");
            exit(0);
        }
        close(newsockfd);
    }
    
    return 0;
}
