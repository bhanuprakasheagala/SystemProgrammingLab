#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
        const char* filename = "example.txt";
        int fd = open(filename, O_RDWR | O_CREAT, 0666);

        if(fd == -1) {
                perror("Failed to open file");
                return 1;
        }

        // Resize the file to be large enough to hold the text
        if(ftruncate(fd, 100) == -1) {
                perror("Failed to resize file");
                close(fd);
                return 1;
        }

        // Map the file into memory
        char* mapped = mmap(NULL, 100, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if(mapped == MAP_FAILED) {
                perror("mmap failed");
                close(fd);
                return 1;
        }

        // Write to the memory-mapped area
        strncpy(mapped, "This is mapped memory!", 100);

        // Sync the changes to the file
        msync(mapped, 100, MS_SYNC);

        // Unmap the memory and close the file
        munmap(mapped, 100);

        close(fd);

        printf("Memory-mapped file written successfully.\n");

        return 0;
}
