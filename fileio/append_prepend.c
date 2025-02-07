#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
	int fd;
	off_t ret_offset;
		
	if ((argc < 2) | (strcmp(argv[1], "--help") == 0))
		usageErr("%s file \n", argv[0]);
	
	fd = open(argv[1], O_WRONLY | O_APPEND);
//	fd = open(argv[1], O_WRONLY);

	if (fd == -1)
	{	
		printf("Error opening file.");
		close(fd);
	}else{
		ret_offset = lseek(fd, -10, SEEK_CUR);
		printf("the offset after lseek is: %ld", (long) ret_offset);
		write(fd, "123", 3);
		close(fd);
	}
}
