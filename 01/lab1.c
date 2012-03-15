/* Modified demo code from Dr. Murphy's 415 website */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

/* Changed to BUFSIZE to 11 */
#define BUFSIZE 11

main() {
    char buffer[BUFSIZE];
    char filename[BUFSIZE];
    int read_fd;
    int write_fd;
    int copy_fd;
    int retval;
    int numread;
    int fileread;
    int fileval;
    int bytes_copied = 0;

    printf("Welcome to the TEE Copy Program by JP Villanueva!\n");

    printf("Enter the name of the file to copy from: ");
    scanf("%s", &buffer[0]);

    /* Read from a file */
    if((read_fd = open(buffer, O_RDONLY)) < 0) {
        perror("Bad read file open");
        exit(-1);
    }

    printf("Enter the name of the file to copy to: ");
    scanf("%s", &filename[0]);

    /* Create a file and write to it */
    if((write_fd = open(filename, O_CREAT | O_WRONLY)) < 0) {
        perror("Bad write file open");
        exit(-1);
    }

    do {
        /* Read from a file */
        if((numread = read(read_fd, buffer, BUFSIZE)) < 0) {
            perror("Bad file read");
            exit(-1);
        }

        /* Debugging output*/

        printf("This is what is currently in the read buffer: %s\n", buffer);

        /* Write to a file */
        if((retval = write(write_fd, buffer, numread)) < 0) {
            perror("Bad file write");
            exit(-1);
        }

        /* Debugging output*/
        printf("This is what is currently in the write buffer: %s\n", buffer);


        bytes_copied += retval;
    } while(numread > 0);

    /* Close file descriptors */
    close(write_fd);
    close(read_fd);

    /* Read from a file */
    if((copy_fd = open(filename, O_RDONLY)) < 0) {
        perror("Bad read file open");
        exit(-1);
    }

    do {
        /* Read from a file */
        if((fileread = read(copy_fd, buffer, BUFSIZE)) < 0) {
            perror("Bad file read");
            exit(-1);
        }

        /* Write to a STDOUT */
        if((fileval = write(1, buffer, fileread)) < 0) {
            perror("Bad file write");
            exit(-1);
        }
    } while(fileread > 0);

    printf("File copy successful, %d bytes copied.\n", bytes_copied);

    /* Close file descriptor */
    close(copy_fd);
}
