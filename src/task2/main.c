#include<fcntl.h>
#include<sys/stat.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include"copy.h"
/*
 * This is C-analog of CP (Unix term command) with cutted functional(with time functional'll be added)
 */

void simple_mode_run(int argc, char **argv) 
{
    if (argc < 2) 
    {
        printf("%s", "Illegal argument number. Require at least two arguments!");
        return;
    }
    int r = file_copy(*(argv + 1), *(argv + 2));
    if (r == 0) 
    {
        printf("%s", "File copied succesful!");
        return;
    }
    if (r == -1) 
    {
        printf("%s", "Source file doesn't exist or can't be read!");
        return;
    }
    if (r == 1) 
    {
        printf("%s %s", *(argv + 2), " has already existed!");
    }
}

int main(int argc, char **argv) 
{
    simple_mode_run(argc, argv);
    return 0;
}