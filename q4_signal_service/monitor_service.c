#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static void handle_sigint(int sig) {
    (void)sig;
    printf("Monitor Service shutting down safely.\n");
    exit(0);
}

static void handle_sigusr1(int sig) {
    (void)sig;
    printf("System status requested by administrator.\n");
}

static void handle_sigterm(int sig) {
    (void)sig;
    printf("Emergency shutdown signal received.\n");
    exit(0);
}

static void register_handler(int signum, void (*handler)(int)) {
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(signum, &sa, NULL);
}

int main(void) {
    register_handler(SIGINT, handle_sigint);
    register_handler(SIGUSR1, handle_sigusr1);
    register_handler(SIGTERM, handle_sigterm);

    printf("Monitor Service started. PID: %d\n", getpid());

    while (1) {
        printf("[Monitor Service] System running normally...\n");
        sleep(5);
    }

    return 0;
}
