#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX 1000
#define MAX_NUM 100
int main()
{
    while(1)
    {
        printf("$ ");
        char cmd[MAX];
        char* args[MAX_NUM];
        char *str=fgets(cmd,sizeof(cmd),stdin);
        if(str==0) continue;
        cmd[strlen(cmd)-1]='\0';
        args[0]=strtok(cmd," ");
        int i=1;
        while(args[i++]=strtok(NULL," "));
        if(strcmp(args[0], "exit")==0)
        {
            return 0;
        }
        else if(strcmp(args[0], "pwd")==0)
        {
            char* wd=getcwd(NULL, 0);
            if(wd!=NULL) printf("%s\n",wd);
            continue;
        }
        else if(strcmp(args[0], "cd")==0)
        {
            int cd;
            if(args[1]!=NULL)
            {
               cd=chdir(args[1]);
               if(cd==-1) printf("No such file or directory\n");
            }
            else chdir("/home");
            continue;
        }
        pid_t id=fork();
        if(id==0) execvp(args[0],args);
    }
    return 0;
}
