#ifndef __headers_h__ 
#define __headers_h__

#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <termios.h>

#define TRUE 1
#define FALSE 0		/* Why didn't I define this earlier? */

#define MAX_ARR_SIZE 128
#define MAX_ARG_AMOUNT 10
#define MAX_ARG_LENGTH 100
#define MAX_STRING_LENGTH 100
#define MEM_LENGHT 1<<16 		/* What I assume is our memory length is. Everything over it will give you an error, nothing will be executed */
#define STRING_INIT_LENGTH 100
#define MAX_PROCESS_AMOUNT 10	/* Alright, this is where stuff gets real. This one will allow multiple processes running at a time */


#define WAIT_ANY -1
#define STDIN stdin 
#define STDOUT stdout 

/* Consts and stuff */


typedef struct node /* 2-way list for keeping our arguements */
    {
    char* elem;
    struct node *next;
    struct node *prev;
    } arg;

typedef struct proc 
    {
    int id;
    int cnt;
    char *name;
    pid_t pid;
    char *descriptor;
    struct proc *next;
    int status;	/* This way I will be able to move it to foreground or background */
    } process;

typedef struct ppipe
    {
    char** args;
    int fout;
    int fin;
    int finflag;
    int foutflag;
    int fappflag;
    char* flin;
    char* flout;
    struct ppipe *next;
    } pproc;

#define FG 1        /* Foreground */
#define BG 2        /* Background */

static process* procList = NULL;    /* No processes ATM, nothing to see here. */
static int processCounter;

static pproc* pipeline = NULL;

static char* source;
static char execName[MAX_ARR_SIZE];
static char arguementString[MAX_ARR_SIZE];

static char* currentDir;   
static char* arguements[MAX_ARG_AMOUNT];
static int arguementCnt=0;

static char* defaultConsts[] = {"$SHELL", "$HOME", "$USER", "$EUID"};
static const int constAmount = 4;
static char* defaultCommands[] = {"exit", "pwd", "help", "---", "cd", "kill", ">", "<", "<<"};
static char* commandHelp[] = {"Stops shell execution", "Shows current directory", "Display help", "---", "Change directory", "Kill a process", "Direct process input", "Direct process output", "Append process output to the file"};
static const int commandAmount = 9;
static int argflag;
static int arglenght;
static int defflag;
static int errflag;
static int endflag=0;
static int dotflag=1;
static char* shelldir;

static struct termios TERMINAL_MODE;
static int TERMINAL;	/* Input-output control */
static pid_t SHELL_ID;	/* Shell pid to return terminal control	*/
static pid_t SHELL_PG;

/* Command execution and other junk */

void ironsInTheFire(char *cmd[], char* file, int mode, struct _IO_FILE *descriptor);  

void waitForProcess(process* proc);

void execute(char *cmd[], char *file, int mode, struct _IO_FILE *descrtiptor);

/* Pre-defined commands */

void helpMe();   /* Print help */

void findMe();   /* Get current directory */

void killMe();  /* Terminate shell execution */

void changeDir();    /* Change directory */

int checkDefaultCommands();  /* Check for pre-defined command usage. Almost done */

void startWorking();         /* Yup. The creation of a procedure for 1 line of code was totally nessesary */
   
/* Stuff to work with strings and stuff. This is probably called "Parser" */

int backSlashCheck(char letter, int *counter);   /* Such a sweet construction I decided it deserved its very own function */

int getExecName(char* input, char* name, char* args); /* Will get the name of the program to execute */

void getHashComment(const char* input, char* output); /* Get comments indicated by # and ignore them */

void getArguements(char* input, char** arr, int *length);   /* Get arguements from string and fill the array with them. */

void splitString();

void printArguements(arg** inputList); /* This is a placeholder since we don't do anything with arguements yet */

void cleanCommandArray();

/* Old junk to work with lists. No longer needed since I don't use lists anymore. */

void pushToList(arg** target, char* text); /* Will push "text" string to "trg"-list */

void initList(arg** target); /* Create the first node of arguement list. The first node will always be NULL */

void cleanList(arg** list);

/* Now this is more interesting... */

void init();

int checkProcessLimit();     /* Check if we have too many processes running */

void sendProcessToForeground(process* proc);

void sendProcessToBackground(process* proc);

process* addNewProcess (pid_t pid, char* name, char* descriptor, int status);

int changeProcessStatus(pid_t pid, int status);

process* deleteProcess(process* proc);

process* getProcessByPID(pid_t pid);

void childHandler(int p);

/* More primitive stuff to work with strings. NOT A PARSER */

/* Reading user input */

char* getString(); /* Read infinite string from the input, say something if we go over MEM_LENGTH. */

char* strmerge(char* str1, char* str2);

/* Alright, getString is awfull dumb useless function that is not optimized in any way. Why the hell would I ever want dynamic string? Screw it, I am doing it like perfectly sane people do */

/* Properly reading user input */

void getUserInput(char* output);

/* Oh my god why the hell would I want to create a GOD FORSAKEN FUNCTION FOR SIMPLE a=b where both a and b are char*??? */

void putTextToString(char* string, const char* text);

/* Clearing the array of strings. */

void cleanUp(char** array);

void testarray();

void sayHello();

void inviteUser();

int checkArray();

#endif
