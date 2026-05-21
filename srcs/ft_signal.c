#include "../ft_ping.h"

extern bool pinging;

static void signal_handler(int signo)
{
    if (signo == SIGINT)
        pinging = false;
}

// Handle any SINGINT received
void ft_signal()
{
    struct sigaction act;

    act.sa_handler = signal_handler;
    sigaction(SIGINT, &act, NULL);
}