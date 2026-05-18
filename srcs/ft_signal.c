#include "../ft_ping.h"

static void signal_handler(int signo)
{
    if (signo == SIGINT)
        exit(0); // Change this with stopping ping loop

    // Add SIGALRM for continous pinging
}

// Change ft_signal accordingly with what is change/added uptop
void ft_signal()
{
    struct sigaction act;

    act.sa_handler = signal_handler;
    sigaction(SIGINT, &act, NULL);
}