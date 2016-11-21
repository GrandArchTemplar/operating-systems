#include<fcntl.h>
#include<sys/stat.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
/*
 * This is C-analog of CP (Unix term command) with cutted functional(with time functional'll be added)
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
        stat(instance_path, &tmp);
        const int mode = tmp.st_mode;
        const int input = open(instance_path, O_RDONLY);
        const int output = open(copy_path, O_CREAT | O_WRONLY, mode);
        const int BUFFER_SIZE = 16384;
        char *buffer = (char*) malloc(BUFFER_SIZE);
        int r = -1;
        int t;
        while(r)
        {
            r = read(input, buffer, BUFFER_SIZE);
            t = write(output, buffer, r);
        }
        free(buffer);
        return 0;
    }
    return 1;
}