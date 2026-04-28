#include "../ft_ping.h"

static void signal_handler(int signo)
{
    if (signo == SIGINT)
        exit(0);
}

void ft_signal()
{
    struct sigaction act;

    act.sa_handler = signal_handler;
    sigaction(SIGINT, &act, NULL);
}