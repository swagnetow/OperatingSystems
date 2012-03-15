/* Modified demo code for hw1 from Dr. Murphy's 415 website */
#include <stdio.h>
#include <windows.h>

/* Changed BUFSIZE to 11 */
#define BUFSIZE 11

main()
{
    char buffer[BUFSIZE];
    char filename[BUFSIZE];
    HANDLE read_handle, write_handle, copy_handle;
    HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD nIn, nOut, cIn, cOut;
    int bytes_copied = 0;

    printf("Welcome to the TEE Copy Program by JP Villanueva\n");
    printf("Enter the name of the file to copy from: ");
    scanf("%s", &buffer[0]);

    /* Open file handle.. */
    read_handle = CreateFile(buffer, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

    if(read_handle == INVALID_HANDLE_VALUE) {
        printf("Bad File Open. Error: %x\n", GetLastError());
        return 2;
    }

    printf("Enter the name of the file to copy to: ");
    scanf("%s", &filename[0]);

    /* Open file handle. */
    write_handle = CreateFile(filename, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

    if(write_handle == INVALID_HANDLE_VALUE) {
        printf("Bad File Open. Error: %x\n", GetLastError());
        return 2;
    }

    /* Write to file */
    while(ReadFile(read_handle, buffer, BUFSIZE, &nIn, NULL) && nIn > 0) {
        /* Debugging output
        printf("This is what is currently in the read buffer: %s\n", buffer);
        */

        WriteFile(write_handle, buffer, nIn, &nOut, NULL);

        /* Debugging output
        printf("This is what is currently in the write buffer: %s\n", buffer);
        */

        if(nIn != nOut) {
            printf("Bad write. Error: %x\n", GetLastError());
            return 4;
        }

        bytes_copied += nIn;
    }

    CloseHandle(read_handle);
    CloseHandle(write_handle);

    printf("\n");

    /* Open file handle */
    copy_handle = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

    if(copy_handle == INVALID_HANDLE_VALUE) {
        printf("Bad File Open. Error: %x\n", GetLastError());
        return 2;
    }

    /* Write to STDOUT */
    while(ReadFile(copy_handle, buffer, BUFSIZE, &cIn, NULL) && cIn > 0) {
        WriteFile(stdout_handle, buffer, cIn, &cOut, NULL);

        if(cIn != cOut) {
            printf("Bad STDOUT write. Error: %x\n", GetLastError());
            return 4;
        }
    }

    printf("\nFile copy successful, %d bytes copied.\n", bytes_copied);
}
