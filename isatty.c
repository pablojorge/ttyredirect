#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (isatty(STDOUT_FILENO)) {
        printf("stdout is a tty\n");
    } else {
        printf("stdout is NOT tty\n");
    }

    return 0;
}
