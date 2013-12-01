#ifndef __utils_h__
#define __utils_h__

/* Old junk to work with lists. No longer needed since I don't use lists anymore. */

void pushToList(arg** target, char* text) /* Will push "text" string to "trg"-list */
    {
    /* int j=0, n=1; */
    arg* safeCopy=*target;
    arg* newNode;
    newNode=(arg*) malloc(sizeof(arg));

    while (safeCopy->next!=NULL)
        {
        safeCopy=safeCopy->next;
        }

    newNode->elem=text;

    /*
    while (text[j]!='\0')
            {
            newNode->elem[j]=text[j];
            j++;
            n++;
            printf("done\n");
            newNode->elem=(char*) realloc(newNode->elem, n*sizeof(char));
            }
    */

    debug("Arguement in push %s (temporal output)\n", newNode->elem);
    /* newNode->elem[j]='\0';  */

    safeCopy->next=newNode;
    newNode->prev=safeCopy;
    newNode->next=NULL;
    }

void initList(arg** target) /* Create the first node of arguement list. The first node will always be NULL */
    {
    (*target)=(arg*) malloc(sizeof(arg));
    /* (*target)->elem=(char*) malloc(sizeof(char)); */
    (*target)->elem='\0';
    (*target)->next=NULL;
    (*target)->prev=NULL;
    }   

void cleanList(arg** list)
    {
    arg* tail=*list;
    arg* head=*list;
    while (head->next!=NULL)
        {
        head=head->next;
        free(tail);
        tail=head;
        }
    free(head);
    free(tail);
    }

/* Now this is more interesting... */

void init()
    {

        SHELL_ID = getpid();        
        TERMINAL = STDIN_FILENO;     
        SHELL_PG = getpgrp();                                  

        if (isatty(TERMINAL)) 
        {                    
            while (tcgetpgrp(TERMINAL)!=(SHELL_PG=getpgrp())) kill(SHELL_ID, SIGTTIN);                   
            signal(SIGQUIT, SIG_IGN);
            signal(SIGTTOU, SIG_IGN);
            signal(SIGTTIN, SIG_IGN);
            signal(SIGTSTP, SIG_IGN);
            signal(SIGINT, SIG_IGN);
            signal(SIGCHLD, &childHandler);

            setpgid(SHELL_ID, SHELL_ID); 

            SHELL_PG=getpgrp(); 
            if (SHELL_ID!=SHELL_PG) 
            {
                log_err("Shell isn't process group leader. Not gonna work under these conditions.");
                exit(-1);
            }
            tcsetpgrp(TERMINAL, SHELL_PG);
            tcgetattr(TERMINAL, &TERMINAL_MODE);
            currentDir=(char*) calloc(1024, sizeof(char));
        }
         else 
        {
            printf("ISATTY returned false. Sorry, mate, not going to work today.\n");
            exit(-1);
        }
    }

int checkProcessLimit()     /* Check if we have too many processes running */
    {
    if (processCounter>=MAX_PROCESS_AMOUNT) 
        {
        log_err("Maximum amount of parallel processes has been launched. Could not create another one.");
        return 0;
        } else
            {
            debug("There are %d processes at the moment. Creating a new one...", processCounter);
            return 1;
            }
    }

void sendProcessToForeground(process* proc)
    {
        proc->status=FG;                                                
        tcsetpgrp(TERMINAL, proc->pid);                                 
        waitFroProcess(proc);                                                                              
        tcsetpgrp(TERMINAL, SHELL_PG);                              
    }

void sendProcessToBackground(process* proc)
    {
        /*
        That's a huge load o' nothing to see here.
        Like - really. This is only for part 4. I'll do it later. And see if it works later.
        */
        if (proc==NULL) return; 
        tcsetpgrp(TERMINAL, SHELL_PG);                            
    }

process* addNewProcess (pid_t pid, char* name, char* descriptor, int status)
    {
            process *newProc=malloc(sizeof(process));
            newProc->name=(char*) malloc(sizeof(name));
            newProc->name=name;
            newProc->pid=pid;
            newProc->status=status;
            newProc->descriptor=(char*) malloc(sizeof(descriptor));
            newProc->descriptor=descriptor;
            newProc->next=NULL;

            if (procList==NULL) 
            {
                processCounter++;
                newProc->cnt=numActiveJobs;
                return newProc;
            } 
            else 
            {
                process *temp=procList;

                while (temp->next!=NULL) temp=temp->next;

                newProc->cnt=temp->cnt+1;
                temp->next=newProc;
                processCounter++;

                return procList;
            }
    }

int changeProcessStatus(pid_t pid, int status)
    {
        process *proc=getProcessByPID(pid);
        if (proc==NULL) return FALSE;
        else 
        {
            proc->status=status;
            return TRUE;
        }
    }

process* deleteProcess(process* proc)
    {
        if (jobsList == NULL) return NULL;

        process* currentProcess;
        process* beforeCurrentProcess;

        currentProcess=procList->next;
        beforeProcess=procList;

        if (beforeProcess->pid==proc->pid) 
        {
            beforeProcess=beforeProcess->next;
            processCounter--;
            return currentProcess;
        }

        while (currentProcess!=NULL) 
        {
            if (currentProcess->pid==proc->pid) 
            {
                processCounter--;
                beforeProcess->next=currentProcess->next;
            }
            beforeProcess=currentProcess;
            currentProcess=currentProcess->next;
        }
        return procList;
    }

process* getProcessByPID(pid_t pid)
    {
        process* proc=procList;
        while (proc!=NULL) 
        {
            if (proc->pid==pid) return proc;
            else proc=proc->next;
        }          
        return NULL;
    }

void childHandler(int p)
    {
        pid_t pid;
        int terminationStatus;
        pid=waitpid(WAIT_ANY, &terminationStatus, WUNTRACED|WNOHANG); // intercept the process that sends the signal
        if (pid) 
        {                                                                       
            process* proc = getProcessByPID(pid);                      
            if (proc==NULL) return;
            if (WIFEXITED(terminationStatus)) 
            {                                                      
                printf("\n[%d]: <%s> finished succesfully\n", proc->id, proc->name); 
                procList=deleteProcess(proc);                                                    
            }
            else 
            if (WIFSIGNALED(terminationStatus)) 
            {                                  
                printf("\n[%d]: <%s> was killed\n", proc->id, proc->name); 
                procList=deleteProcess(proc);                                                     
            } 
            else 
                if (WIFSTOPPED(terminationStatus))  /* Sigstp */
                {                                                                                                 
                    tcsetpgrp(TERMINAL, proc->pid);                   
                    printf("\n[%d]: %s was stopped\n", processCounter, proc->name); 
                } 
                else 
                {
                    if (proc->status==BG) 
                    {                   
                        procList=deleteProcess(proc);
                    }
                }
            tcsetpgrp(TERMINAL, SHELL_PG);
        }
    }

#endif
