#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
        if(fork()==0) //外部命令 
        {
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
        //if(str==0) continue;
        cmd[strlen(cmd)-1]='\0';
        args[0]=strtok(cmd," ");
        int i=1;
        while(args[i++]=strtok(NULL," "));
        if(strcmp(args[0], "exit")==0) return 0;//exit命令
        int j=0;
        int k=0,l=0;
        char* arg[MAX_NUM][MAX_NUM];
        j=0;
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
