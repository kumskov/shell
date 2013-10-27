#ifndef __headers_h__ 
#define __headers_h__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define TRUE 1
#define FALSE 0		/* Why didn't I define this earlier? */

#define MAX_ARR_SIZE 128
#define MAX_STRING_LENGTH 100
#define MEM_LENGHT 1<<16 		/* What I assume is our memory length is. Everything over it will give you an error, nothing will be executed */
#define STRING_INIT_LENGTH 100
#define MAX_PROCESS_AMOUNT 10	/* Alright, this is where stuff gets real. This one will allow multiple processes running at a time */

/* Consts and stuff */


typedef struct node /* 2-way list for keeping our arguements */
    {
    char* elem;
    struct node *next;
    struct node *prev;
    } arg;

typedef struct proc /* 2-way list for keeping our arguements */
    {
    char* name;
    pid_t pid;
    int currentState;	/* This way I will be able to move it to foreground or background */
    } process;

process processTable[MAX_PROCESS_AMOUNT];
int processCounter;

#define FG 1 		/* Foreground */
#define BG 2 		/* Background */

char source[MAX_ARR_SIZE];
char execName[MAX_ARR_SIZE];
char arguementString[MAX_ARR_SIZE];
char homeSweetHome[MAX_ARR_SIZE];
char* arguements[MAX_ARR_SIZE];
char* defaultCommands[] = {"exit", "home", "help", "copyright", "cd"};
char* commandHelp[] = {"Stops shell execution", "Shows home directory", "Display help", "Display copyrights", "Change directory"};
const int commandAmount = 5;
int argflag;
int arglenght;
int defflag;

int TERMINAL;	/* Input-output control */
int SHELL_ID;	/* Shell pid to return terminal control	*/

/* Main input procedure */

char* getString(int eoflag); /* Read infinite string from the input, say something if we go over MEM_LENGTH. */



/* Basic procedures to work with the list and other junk */
/* Most of this is outdated - I no longer use lists. Oh well, let those procedures remain as a history. */

void pushToList(arg** target, char* text); /* Will push "text" string to "trg"-list */

void initList(arg** target); /* Create the first node of arguement list. The first node will always be NULL */

void putTextToString(char* string, const char* text); /* Why the hell did I ever think I would need this? */

void cleanList(arg** list); /*Pretty self-explanatory, huh? */

void cleanUp(char** trash, int num); /*Tried to clean string array properly. Failed horribly. */



/* Procedures to work with program name and comments in the line */

int backSlashCheck(char letter, int *counter);  /* Such a sweet construction I decided it deserved its very own function. It checks if the char has backslash and signs about it. */

int getExecName(char* input, char* name, char* args); /* Will get the name of the program to execute, returns 1 if there are also arguements */

void getHashComment(const char* input, char* output); /* Get comments indicated by # and ignore them */

void getArguements(char* input, char* arr[], int *length); /* Alright, now THIS is the most horrible stupid dumb function I HATE so god damn much. Mostly because I can't, for some reason, properly fill char**. The first element is fine, all others will get filled with some junk which appeared out of nowhere. Please don't ask me why I am writing elements to array in THIS horrible way. I couldn't find better. Mallocing new elements doesn't work properly, I tried. */

void printArguements(arg** inputList); /* This is a placeholder since we don't do anything with arguements yet */



/* Executing commands and stuff. Some error notification maybe? */

void ironsInTheFire(char* command, char** args);  /* Properly working executing. Well, it works. And executes stuff. */


#endif
