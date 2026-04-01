/* app.c */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>


int main(int argc, char** argv)
{
    if (argc < 2) 
    {  
        fprintf(stderr, "Usage: %s /dev/mydevice\n", argv[0]);
        return -1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("open");
        return EXIT_FAILURE;
    }

    int nb_interrupts = 0;
    fd_set rfds;

    printf("Waiting for interrupts on %s...\n", argv[1]);
    printf("Press K1, K2, or K3\n\n");

    while (1) {
        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);

        int ret = select(fd + 1, &rfds, NULL, NULL, NULL);
        if (ret < 0) {
            perror("select");
            break;
        }

        if (FD_ISSET(fd, &rfds)) {
            nb_interrupts++;
            printf("Interrupt detected! Total count: %d\n", nb_interrupts);
        }
    }

    close(fd);
    return EXIT_SUCCESS;
}
