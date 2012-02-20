#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#include <pty.h>

int redirect_loop(int tty_fd, char *output_filename)
{
    int output_fd = open(output_filename, 
                         O_WRONLY | O_CREAT | O_TRUNC,
                         S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    if (output_fd < 0) {
        perror("open");
        return -1;
    }

    char buffer[BUFSIZ];

    ssize_t count = read(tty_fd, buffer, sizeof(buffer));

    while (count > 0) {
        count = write(output_fd, buffer, count);
        if (count < 0) {
            perror("write");
            return -1;
        }
        count = read(tty_fd, buffer, sizeof(buffer));
    }

    close(output_fd);

    return 0;
}

int waitchild(int pid) {
    int status;

    pid_t ret = waitpid(pid, &status, 0);

    printf("child exited with status %d\n",
            WEXITSTATUS(status));

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, 
                "Usage:\n\t%s OUTPUT_FILE cmd [args..]\n",
                argv[0]);
        exit(1);
    }

    int tty_fd;
    pid_t pid;

    pid = forkpty(&tty_fd, NULL, NULL, NULL);

    if (pid == -1) {
        /* ERROR */
        perror("forkpty");
        exit(2);
    } 
    else if (pid > 0) {
        /* PARENT */
        redirect_loop(tty_fd, argv[1]);
        waitchild(pid);
    }
    else {
        /* CHILD */
        execvp(argv[2], argv + 2);
        perror("execvp");
        exit(3);
    }

    return 0;
}
