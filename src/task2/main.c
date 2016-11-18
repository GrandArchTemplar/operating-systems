#include<fcntl.h>
#include<sys/stat.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
/*
 * return -1 if source doesn't exist and 0 if copy was succesful and return 1 if copied file exists
 */
int file_copy(const char *instance_path, const char *copy_path) 
{
    if (access(instance_path, R_OK)) 
    {
        return -1;
    }
    if (access(copy_path, F_OK)) 
    {
        struct stat tmp;
        const int mode = stat(instance_path, &tmp);
        const int input = open(instance_path, O_RDONLY);
        const int output = open(copy_path, O_CREAT | O_WRONLY, mode);
        const int BUFFER_SIZE = 16384;
        char *buffer = malloc(BUFFER_SIZE);
        int r = -1;
        while(r)
        {
            r = read(input, buffer, BUFFER_SIZE);
            write(output, buffer, r);
        }
        return 0;
    }
    return 1;
}

void run(int argc, char **argv) 
{
    if (argc < 2) 
    {
        printf("%s", "Illegal argument number. Requre at least two arguments!");
        return;
    }
    int r = file_copy(*(argv), *(argv + 1));
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
        printf("%s %s", *(argv + 1), " has already existed!");
    }
}

int main(int argc, char **argv) 
{
    run(argc, argv);
}