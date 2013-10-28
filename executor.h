#ifndef __executor_h__
#define __executor_h__

/* Main procedure to launch new process. Because newProcess() is too mainstream. */

void ironsInTheFire(char* command, char** args, int mode)   /* Properly working executing. Well, it works. And executes stuff. */
    {
    pid_t pid, terminalReciever;
    int junk;
    
    pid=fork();
    if (pid)    
        {            /* Create new process with called execution mode (background or foreground) */
        if ((!errno) || (errflag==-1))
            {
            debug("I created child with PID %d\n", pid);
            setpgid(pid, pid);                                                        
        
            createNewProcess(command, pid, mode);
            if (mode==FG) sendToForeground(pid);                                             
            if (mode==BG) sendToBackground(pid);

            restoreSignals(); /* Parent returns proper signals */
            
            }

        } else 
            {
            junk=setpgrp();   
            terminalReciever=getpid();

            if (mode==FG) tcsetpgrp(TERMINAL, terminalReciever);    /* Allow child to use terminal */
            if (mode==BG) debug("Created a new child in background with ID of %d", terminalReciever);   /* This is totally ironic - terminalReciever DOESN'T RECIEVE ACCESS TO THE TERMINAL */

            errflag=execvp(command, args);
            exit(1);
            }

    }

/* Stuff to work with processes */

void sendToBackground(pid_t id) /* Take terminal control from process and indicate it in the table. Also this process will not be stopped. */
    {
    int num;
    debug("Process %d will be sent to background", id);
    num=getProcessByID(id);
    if (num!=-1) processTable[num].currentState=BG;
        else killProcess(id);
    tcsetpgrp(TERMINAL, SHELL_ID);
    }

void sendToForeground(pid_t id) /* This process will be in the foreground and will be stopped */
    {
    int num;
    debug("Process %d will be sent to foreground", id);
    num=getProcessByID(id);
    if (num!=-1) 
        {
        processTable[num].currentState=FG;
        debug("I gave %d the terminal", id);                   
        tcsetpgrp(TERMINAL, id);
        } else killProcess(id);
    waitProcess(id);

    tcsetpgrp(TERMINAL, SHELL_ID);
    }

void waitProcess(pid_t id)  /* Terminate the process via waitpid */
    {
    int waiter;
    int status;
    debug("Killing zombie process with id %d with wait", id);
    waiter=waitpid(id, &status, WNOHANG);
    deleteProcess(id);
    debug("Zombie process %d succesfully killed", id);
    }

void killProcess(pid_t id)  /* Terminate the process via SIGKILL */
    {
    debug("Killing process with id %d by sending him SIGKILL", id);
    kill(id, SIGKILL);
    deleteProcess(id);
    debug("Process %d succesfully killed", id);
    }

void createNewProcess(char* name, pid_t id, int state) /* Add new process at the end of processTable */
    {
    if (checkProcessLimit())
        {
        processTable[processCounter].name=name;
        processTable[processCounter].pid=id;
        processTable[processCounter].currentState=state;
        processCounter++;
        }
    }

int getProcessByID(pid_t id)   /* Find process with id and return it's number in processTable */
    {
    int i, flag=0;
    for (i=0;i<processCounter;i++)
        {   
        if (processTable[i].pid==id)
            {
            debug("Found process with id %d, being %d in the process list", id, i);
            flag=1;
            return i;
            break;
            }
        }
    if (!flag) 
        {
        printf("No process was found with id %d\n", id);
        return -1;
        }
    }

void deleteProcess(pid_t id)   /* Delete process from execution list */
    {
    int i, j, flag=0;
    for (i=0;i<processCounter;i++)
        {   
        if (processTable[i].pid==id)
            {
            debug("Deleting %d process, being %d in the process list", id, i);
            processCounter--;  
            for (j=i;j<processCounter;j++)
                {   
                processTable[j].name=processTable[j+1].name;
                processTable[j].pid=processTable[j+1].pid;
                processTable[j].currentState=processTable[j+1].currentState;
                flag=1;
                }
            }
        }
    if (!flag) printf("No process was suspended with id %d\n", id);
    }

void childControl()  /* To be finished  */
    {
    pid_t pid;
    int checker;    /* For WIFEXITED, SIGNALED and STOPPED */
    int status;     /* This one is weird since I don't completely understand how it works. But WIFEXITED and others use this &int for child detected by waitpid, so here it is. */

    pid=waitpid(WAIT_ANY, &status, WNOHANG|WUNTRACED );
    if (pid) 
        {                                                                         
        
        /*
        
        Some code should be here, with a lot of majyyks including WIFEXITED, WIFSIGNALED and WIFSTOPPED. This should be run when process gets SIGCHL.

        */

        tcsetpgrp(TERMINAL, SHELL_ID);
        }
    }

/* Pre-defined commands */

void helpMe()   /* Print help */
    {
    int j;
    for (j=0;j<commandAmount;j++)
        {
        printf("%s: %s\n", defaultCommands[j], commandHelp[j]);
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
    int success;
    if (arguements[0]==NULL) chdir(getenv("HOME"));
        else 
            {
            success=chdir(arguements[0]);
            if (success==-1) printf("There is no directory named <%s>\n", arguements[0]);
            }
    }

int checkDefaultCommands()  /* Check for pre-defined command usage. Not yet complete */
    {
    int chk, f=0, i, j;
    for (i=0;i<commandAmount;i++)
        {
        chk=strcmp(execName, defaultCommands[i]);
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
                    printf("This program was created by Kumskov Mikhail, 2013\n");
                    f=1;
                    break;
                    }
                case 4:
                    {
                    changeDir();
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

#endif
