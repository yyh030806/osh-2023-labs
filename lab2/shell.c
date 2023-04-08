#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX 1000
#define MAX_NUM 100
int excute(char* args[100])//执行单条命令
{
         //内部命令
        if(strcmp(args[0], "pwd")==0)//pwd
        {
            char* wd=getcwd(NULL, 0);
            if(wd!=NULL) printf("%s\n",wd);
           
        }
        else if(strcmp(args[0], "cd")==0)//cd
        {
            int cd;
            if(args[1]!=NULL)
            {
               cd=chdir(args[1]);
               if(cd==-1) printf("No such file or directory\n");
            }
            else chdir("/home");

        }
       else{
         //外部命令 
         int i,redirect=3;
         for(i=0;args[i]!=NULL;i++)
         {
          if(strcmp(args[i],">")==0)
           {
            redirect=0;
            args[i]=NULL;
            break;
           }
           if(strcmp(args[i],"<")==0)
           {
            redirect=1;
            args[i]=NULL;
            break;
           }
           if(strcmp(args[i],">>")==0)
           {
            redirect=2;
            args[i]=NULL;
            break;
           }
         }
         if(redirect==0)
         {
          if(fork()==0)
           { 
            int fd;
            int x=dup(1); 
            fd=open(args[i+1],O_WRONLY | O_CREAT,S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH);
            dup2(fd,1);
            execvp(args[0],args);
            close(fd);
            dup2(x,1);
           }
          }
         else if(redirect==1)
         {
          if(fork()==0)
          {
           int fd;
           int x=dup(0);
           fd = open(args[i+1],O_RDONLY);
           dup2(fd,0);
           execvp(args[0],args);
           close(fd);
           dup2(x,0);
          }
         }
         else if(redirect==2)
         {
          if(fork()==0)
          {
           int fd;
           int x=dup(1);
           fd=open(args[i+1],O_WRONLY | O_APPEND ,S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH);
           dup2(fd,1);
           execvp(args[0],args);
           close(fd);
           dup2(x,1);
          }
         }
         else
         {
          if(fork()==0)
          execvp(args[0],args);
         }
        }
       return 0;
}
int main()
{
    while(1)
    {
        //接收命令并进行分割
        char cmd[MAX];
        char* args[MAX_NUM];
        char *str=fgets(cmd,sizeof(cmd),stdin);
        if(str==0) continue;
        cmd[strlen(cmd)-1]='\0';
        args[0]=strtok(cmd," ");
        int i=1;
        while(args[i++]=strtok(NULL," "));
        if(strcmp(args[0], "exit")==0) return 0;//exit命令
        int j=0;
        int k,l;
        char* arg[MAX_NUM][MAX_NUM];
        for(k=0;k<MAX_NUM;k++)
        {
         for(l=0;l<MAX_NUM;l++)
          {
           arg[k][l]=NULL;
          }
        }
        k=0;
        l=0;
        while(j<i-1)
        {
         if(strcmp(args[j], "|")==0) 
         
          {
           j++;
           k++;
           l=0;
          }
          arg[k][l]=(char *)malloc(sizeof(strlen(args[j])));
          strcpy(arg[k][l],args[j]);
          j++;
          l++;
        }
        //单命令
        if(k==0) 
        {
         excute(arg[0]);
        }
        else//管道命令
        {
         int sfd[2];
         sfd[1]=dup(1);
         sfd[0]=dup(0);
         int m;
         int lastread;
         while(m<=k)
         {
           
           int fd[2];
           pipe(fd);
           pid_t pid=fork();
           if(pid==0)
           {
           if(m>0) 
           {
             dup2(lastread,0);
           }
           if(m<k) 
           {
             dup2(fd[1],1);
           }
           excute(arg[m]);
           }
           if(m>0) close(lastread);
           if(m<k) lastread=fd[0];
           close(fd[1]);
           m++;
         }
         dup2(sfd[1],1);
         dup2(sfd[0],0);
        }
      }
   return 0;
}
