#include <poll.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/poll.h>

#define MAX_CMDS 16

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <format> <cmd1> [cmd2...]\n", argv[0]);
        return 1;
    }

    char *fmt = argv[1];
    const int ncmds = argc - 2;
    if (ncmds > MAX_CMDS) {
        fprintf(stderr, "Too many commands (max %d)\n", MAX_CMDS);
        return 1;
    }

    FILE *fps[ncmds];
    char *cmds[ncmds];
    struct pollfd pfds[ncmds];
    char bufs[ncmds][1024];

    for (int i = 0; i < ncmds; i++) {
        cmds[i] = argv[i + 2];
        fps[i] = popen(cmds[i], "r");
        if (!fps[i]) {
            perror("popen");
            return 1;
        }
        pfds[i] = (struct pollfd){.fd = fileno(fps[i]), .events = POLLIN};
    }

    while (true) {
        int ret = poll(pfds, ncmds, -1);
        if (ret < 0) {
            perror("poll");
            break;
        }

        for (int i = 0; i < ncmds; i++) {
            if (pfds[i].revents & POLLIN) {
                fgets(bufs[i], sizeof(bufs[i]), fps[i]);
                bufs[i][strcspn(bufs[i], "\n")] = '\0';
            }
            if (pfds[i].revents & POLLHUP) {
                pfds[i].fd = -1;
            }
        }

        int cmdi = 0;
        for (char *format = fmt; *format != '\0'; ++format) {
            if (*format == '%') {
                ++format;
                if (*format == 's' && cmdi < ncmds) {
                    fputs(bufs[cmdi++], stdout);
                }
            } else {
                putc(*format, stdout);
            }
        }
        fputc('\n', stdout);
        fflush(stdout);
    }

    return 0;
}
