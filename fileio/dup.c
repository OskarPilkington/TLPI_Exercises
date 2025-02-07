#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"


int dup(int fd)
{
    int fd_out = fcntl(fd, F_DUPFD);
    return fd_out;
}

int dup2(int fd1, int fd2)
{
    int fd_out;
    // Check if fd 2 is open
    int resp = fcntl(fd2, F_GETFL);
    if (resp == -1) // fd 2 doesn't exist, we can just open the file
    {
        fd_out = fcntl(fd1, F_DUPFD, fd2);
    }else{
        close(fd2);
        fd_out = fcntl(fd1, F_DUPFD, fd2);
    }
    return fd_out;
}

int
main(int argc, char *argv[])
{
    int fd1, fd2;
    
    printf("Initializing...\n");
    char filename[9] = "tmp_file";

    fd1 = open(filename, O_WRONLY|O_CREAT, S_IWUSR|S_IRUSR);
    fd2 = dup(fd1);
//    printf("Made it here!\n");
    printf("dup() test:\n");
    printf("fd1: %d, fd2: %d\n", fd1, fd2);

    int fd3 = dup2(fd1, fd2);
    printf("dup2() test:\n");
    printf("fd3: %d\n", fd3);

    return 0;
}
