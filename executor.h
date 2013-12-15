#ifndef __executor_h__
#define __executor_h__

/* Main procedure to launch new process. Because newProcess() is too mainstream. */

void ironsInTheFire(char *cmd[], char* file, int mode, struct _IO_FILE *descriptor)   
    {
        pid_t pid;
        pid=fork();
 
        if (pid)
        {
            setpgid(pid, pid);                                                                      
            procList=addNewProcess(pid, *(cmd), file, (int) mode); 
            process* proc=getProcessByPID(pid);                            
            if (mode == FG) sendProcessToForeground(proc);                                       
            if (mode == BG) sendProcessToBackground(proc);   
        } 
        else
        {
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
            signal(SIGTSTP, SIG_DFL);
            signal(SIGCHLD, &childHandler);
            signal(SIGTTIN, SIG_DFL);
            setpgrp();
            if (mode==FG) tcsetpgrp(TERMINAL, getpid());
            if (mode==BG) printf("Process No%d with PID <%d> was created.\n", ++processCounter, (int) getpid());

            execute(cmd, file, mode, descriptor);

            exit(1);
        }           
    }

void waitForProcess(process* proc)
    {
        int terminate;
        waitpid(proc->pid, &terminate, WNOHANG);
        procList=deleteProcess(proc);                                   
    }

void execute(char *cmd[], char *file, int mode, struct _IO_FILE *descriptor)
    {
        int commandDesc;
        if (descriptor==STDIN) 
        {
            commandDesc=open(file, O_RDONLY, 0666);                                     
            dup2(commandDesc, STDIN_FILENO);
            close(commandDesc);
        }

        if (descriptor == STDOUT) 
        {
            commandDesc=open(file, O_CREAT|O_TRUNC|O_WRONLY, 0666);  // open (create) the file truncating it at 0, for write only
            dup2(commandDesc, STDOUT_FILENO);
            close(commandDesc);
        }
        if (execvp(cmd[0], cmd)==-1) log_err("\nCould not execute <%s>", *cmd);
    }

/* Pre-defined commands */

void helpMe()   /* Print help */
    {
    int j;
    for (j=0;j<commandAmount;j++)
        {
        printf("[%s]: %s\n", defaultCommands[j], commandHelp[j]);
        }
    }

void findMe()   /* Get current directory */
    {
    char* env;
    env=getenv("HOME");
    printf("You are currently in: %s\n", env);
    }

void killMe()   /* Terminate shell execution */
    {
    printf("Terminating...\n");
    exit(0);
    }

void changeDir()    /* Change directory */
    {
        /* if (arguements[1]=="..")
        {
            dotflag=1;
        } */
        if (arguements[1] == NULL) 
        {
            chdir(getenv("HOME"));                                                 
        } 
        else 
        {
            if (chdir(arguements[1])==-1) printf(" %s: Dir doesnt exits\n", arguements[1]);
        }
    }

int Command_handler(char** split,char* shell)
    {
    int i = 0, executed = 0;
    
    signal(SIGINT,SIG_IGN);

    while (split[i] != NULL)
        {
        if (strcmp(split[i],"$EUID") == 0)  /*$EUID*/      
            {
            if (i == 0)
                {
                printf("EUID is: %d\n",geteuid());
                executed = 1;
                }
            free(split[i]);
            split[i] = (char *) malloc(sizeof(char)*10);
            sprintf(split[i],"%d",geteuid());
            }   

        else if (strcmp(split[i],"$USER") == 0) /*$USER*/ 
            {
            if (i == 0)
                {
                printf("Your login is: %s\n",getenv("LOGNAME"));
                executed = 1;
                }
            free(split[i]);
            split[i] = (char *) calloc(strlen(getenv("LOGNAME"))+1,sizeof(char));
            strncpy(split[i],getenv("LOGNAME"),strlen(getenv("LOGNAME")));
            }

        else if (strcmp(split[i],"$HOME") == 0) /*$HOME*/
            {
            if (i == 0)
                {
                printf("Your home directory is: %s\n",getenv("HOME"));
                executed = 1;
                }
            free(split[i]);
            split[i] = (char *) calloc(strlen(getenv("HOME"))+1,sizeof(char));
            strncpy(split[i],getenv("HOME"),strlen(getenv("HOME")));
            }

        else if (strcmp(split[i],"$SHELL") == 0)    /*$SHELL*/
            {
            if (i == 0)
                {
                printf("Shell is: %s\n",shell);
                executed = 1;
                }
            free(split[i]);
            split[i] = (char *) calloc(strlen(shell)+1,sizeof(char));
            strncpy(split[i],shell,strlen(shell));
            }

        i++;
        }

    
    i = 0;
    if (executed == 0)
        {

        if (strcmp(split[i],"cd") == 0) /*cd*/
            {
            if (split[i+1] != NULL)
                {
                printf("You're going to directory %s\n",split[i+1]);
                if (chdir(split[i+1]) != 0)
                    perror("Unreachable parameter of cd");
                }
            else
                {
                printf("You're going to home directory (%s).\n",getenv("HOME"));
                chdir(getenv("HOME"));
                }
            }

        else if (strcmp(split[i],"pwd") == 0)   /*pwd*/
            {
            char* cur_dir = getcwd(NULL,0);
            printf("Your current directory is: %s\n",cur_dir);
            free(cur_dir);
            }

        else if (strcmp(split[i],"exit") == 0)  /*exit*/
            {
            printf("I'm going to exit.\n");
            return 1;
            }

        else    /*Process*/
            {
            if (fork() == 0)    /*Son*/
                {
                signal(SIGINT,SIG_DFL);
                printf("Starting process with PID = %d\n",getpid());
                execvp(split[i],split);
                printf("There is no such command.\n");
                exit(0);
                }
            else    /*Parent*/
                {
                }
            }
        }
    
    wait(NULL);
    return 0;       
    }

int checkDefaultCommands()  /* Check for pre-defined command usage. Almost done */
    {
    int chk, f=0, i;
    for (i=0;i<commandAmount;i++)
        {
        chk=strcmp(arguements[0], defaultCommands[i]);
        /* printf("%s - %s: %d\n", arguements[0], defaultCommands[i], chk); */
        if (!chk) 
            {
            switch(i)
                {
                case 0:
                    {
                    killMe();
                    f=1;
                    break;
                    }
                case 1:
                    {
                    findMe();
                    f=1;
                    break;
                    }
                case 2:
                    {
                    helpMe();
                    f=1;
                    break;
                    }
                case 3:
                    {
                    printf("---\n");
                    f=1;
                    break;
                    }
                case 4:
                    {
                    changeDir();
                    f=1;
                    break;
                    }
                case 5:
                    {
                    printf("Work in progress. Sorry, mate.\n");
                    f=1;
                    break;
                    }
                case 6:
                    {
                    ironsInTheFire(arguements+2, *(arguements+1), FG, STDIN);
                    f=1;
                    break;
                    }
                case 7:
                    {
                    ironsInTheFire(arguements+2, *(arguements+1), FG, STDOUT);
                    f=1;
                    break;
                    }   
                case 8:
                    {
                    printf("Work in progress. Sorry, mate.\n");
                    f=1;
                    break;
                    }   
                default:
                    {
                    break;
                    }

                }
            }
        }

    return f;

    }

void startWorking()         /* Yup. The creation of a procedure for 1 line of code was totally nessesary */
    {
        if(!checkDefaultCommands()) {} ironsInTheFire(arguements, "standart_output", FG, 0);
        sleep(1);

    }

#endif
