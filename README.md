Shell
=====

!Simple Unix-like Shell written in C. This is a part of the task that I was given at MSU.

Contents
=====
* headers.h includes classification and description of functions
* strutils.h contains some functions to work with strings. They are pretty basic, but I may need to add somethinng there, so let it be.
* utils.h is mostly old junk to work with lists, I no longer use them. But there are some things I need and use.
* parser.h has all functions to cut string into parts, get comments, arguments, etc. You get the idea.
* executor.h has stuff to execute commands, error alerts (not yet) and pre-defined shell commands ("exit" will stop the execution of shell).
* zadm.h is Zed's Awesome Debug Macros. Copyright by Zed Shaw. You should read his "Learn C the Hard Way".

shell.c has only main().

What currently works
===
* Built-in commands (exit, help, cd, home)
* Launching new processes in background/foreground
* Properly cutting arguments of launched commands (hash stands for comment and is ignored, backslash screens symbols, quotes contain single argument inside.

Updates
===
* 27.10.2013: 
* *Initial upload
* 28.10.2013: 
* *Fixed memory leaks. 
* *Found a couple of 

Copyright
===
This program was written my Mikhail Kumskov from 217 MSU group in 2013. This code and all parts of it are distributed under GPL of current redaction.

