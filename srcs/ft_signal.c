#include "../ft_ping.h"

extern bool pinging;
extern bool sending;

static void signal_handler(int signo)
{
    if (signo == SIGINT)
        pinging = false;
    if (signo == SIGALRM)
        sending = true;
}

// Handle any SINGINT received
void ft_signal()
{
    struct sigaction act;

    memset(&act, 0, sizeof act);
    act.sa_handler = signal_handler;
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGALRM, &act, NULL);
}