#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#include <pty.h>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, 
                "Usage:\n\t%s command [args..]\n",
                argv[0]);
        return 1;
    }

    int tty_fd;
    pid_t pid = forkpty(&tty_fd, NULL, NULL, NULL);

    if (pid == -1) {
        /* ERROR */
        perror("forkpty");
        return 2;
    } 
    else if (pid > 0) {
        /* PARENT */
        char buffer[BUFSIZ];
        ssize_t count = read(tty_fd, buffer, sizeof(buffer));

        while (count > 0) {
            write(STDOUT_FILENO, buffer, count);
            count = read(tty_fd, buffer, sizeof(buffer));
        }

        int status;
        pid_t ret = waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    }
    else {
        /* CHILD */
        execvp(argv[1], argv + 1);
        perror("execvp");
        return 3;
    }

    return 0;
}
