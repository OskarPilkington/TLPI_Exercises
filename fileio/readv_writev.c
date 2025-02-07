/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 5-2 */

/* t_readv.c

   Demonstrate the use of the readv() system call to perform "gather I/O".

   (This program is merely intended to provide a code snippet for the book;
   unless you construct a suitably formatted input file, it can't be
   usefully executed.)
*/
#include <sys/stat.h>
#include <sys/uio.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    int fd, fd_out;
    struct iovec iov[3];
#define ARR_1_LEN 5
#define ARR_2_LEN 5
#define ARR_3_LEN 5
    char array_1[ARR_1_LEN];
    char array_2[ARR_2_LEN];
    char array_3[ARR_3_LEN];

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s file\n", argv[0]);

    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
        errExit("open");

    fd_out = open("out_file", O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
    
    iov[0].iov_base = array_1;
    iov[0].iov_len = ARR_1_LEN;

    iov[1].iov_base = array_2;
    iov[1].iov_len = ARR_2_LEN;

    iov[2].iov_base = array_3;
    iov[2].iov_len = ARR_3_LEN;

    readv(fd, iov, 3);

    printf("Arr 1: %.5s\nArr 2: %.5s\nArr3: %.5s\n", array_1, array_2, array_3);

    writev(fd_out, iov, 3);

    return 0;
}
