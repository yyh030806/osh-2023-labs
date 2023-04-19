#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#define MAX 1000
#define MAX_NUM 100

int excute(char* args[100])//执行单条命令
{
         //内部命令
         int j,bg=0;
        for(j=0;args[j]!=NULL;j++)
        {
         if(strcmp(args[j], "&")==0) 
          {
           bg=1;
           args[j]=NULL;
          }
        }
        //printf("bg is %d",bg);
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
        else if(strcmp(args[0], "wait")==0) //wait
        {
         waitpid(-1,NULL,0);
         return 0;
        }
       else
       {
         //外部命
         pid_t gpid=getpgrp();
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
         pid_t fatherpgid=getpgrp();
         pid_t pid=fork();
         if(redirect==0)
         {
          if(pid==0)
           {//信号处理
            signal(SIGINT,SIG_DFL);
            signal(SIGTTOU,SIG_IGN);
            setpgid(getpid(),getpid());
            //设置子程序为前台进程
            if(!bg) tcsetpgrp(0,getpid());
            else tcsetpgrp(0,fatherpgid);
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
          if(pid==0)
          {
           signal(SIGINT,SIG_DFL);
           signal(SIGTTOU,SIG_IGN);
           setpgid(getpid(),getpid());
           if(!bg) tcsetpgrp(0,getpid());
           else tcsetpgrp(0,fatherpgid);        
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
          if(pid==0)
          {
           signal(SIGINT,SIG_DFL);
           signal(SIGTTOU,SIG_IGN);
           setpgid(getpid(),getpid());
           if(!bg) tcsetpgrp(0,getpid());
           else tcsetpgrp(0,fatherpgid);
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
          if(pid==0)
          {
           signal(SIGINT,SIG_DFL);
           signal(SIGTTOU,SIG_IGN);
           setpgid(getpid(),getpid());
           if(!bg) tcsetpgrp(0,getpid());
           else tcsetpgrp(0,fatherpgid);
           execvp(args[0],args);
           }
          }
          if(bg==0)
         {
          waitpid(-1,NULL,0);
          tcsetpgrp(0,getpgrp());
         }
        }
       return 0;
}
void clear(int sig)
{
 printf("(clear)\n");
}
int main()
{   
    signal(SIGINT,clear);
    signal(SIGTTOU,SIG_IGN);
    while(1)
    {
        
        printf("#");
        //接收命令并进行分割
        char cmd[MAX];
        char* args[MAX_NUM];
        char *str=fgets(cmd,sizeof(cmd),stdin);
        char *p;
        if(str==0) continue;
        cmd[strlen(cmd)-1]='\0';
        args[0]=strtok(cmd," ");
        int i=1;
        while(args[i++]=strtok(NULL," "));
        if(strcmp(args[0], "exit")==0) return 0;//exit命
        /*p=args[i];
        int control=1;
        while(p!=NULL)
         {
          if((*(p)=='^')&&(*(p+1)=='C')) control=0;
         }
        if(control==0) continue;*/
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
        int x=0;
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
