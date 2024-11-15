#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    printf("starting...\n");
    int fd;
    unsigned long int write_cnt;
    bool append = true;
    
    if ((argc < 2) || (strcmp(argv[1], "--help") == 0))
        usageErr("%s file num-bytes [x]\n", argv[0]);

    if ((argc > 3) && (strcmp(argv[3], "x") == 0))
        {
            fd = open(argv[1], O_WRONLY|O_CREAT); 
            printf("No append\n");
            append = false; 
        } else {
            fd = open(argv[1], O_WRONLY|O_APPEND|O_CREAT);
        }

    errno = 0;
    char *endptr;
    write_cnt = strtoul(argv[2], &endptr, 10);

    if (errno != 0) {
        perror("strtoul");
        return 1;
    }
    if (*endptr != '\0') {
        fprintf(stderr, "Invalid input: non-numeric characters detected\n");
        return 1;
    }    

    char nullbyte = '\0';
    for (int i = 0; i < write_cnt; i++)
    {
        if (!append)
        {
            lseek(fd, 0, SEEK_END); // lseek to end if file is not opened with append
        }

        write(fd, &nullbyte, 1); // Write one 0 byte to the file
    }
        
    printf("Writing %lu bytes\n", write_cnt);

    return 0;
}
