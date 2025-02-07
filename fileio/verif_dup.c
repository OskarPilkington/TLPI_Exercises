#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"


int
main(int argc, char *argv[])
{
    char file_name[8] = "tmp_file";
    int fd1 = open(file_name, O_WRONLY|O_CREAT, S_IWUSR|S_IRUSR);
    int fd2 = dup(fd1);

    // Move fd1 to new location
    off_t offset_fd1 = lseek(fd1, 10, SEEK_SET);
    off_t offset_fd2 = lseek(fd2, 0, SEEK_CUR);

    printf("offset fd1: %ld, offset fd2: %ld", offset_fd1, offset_fd2);
    return 0;
}
