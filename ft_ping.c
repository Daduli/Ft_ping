#include "ft_ping.h"

int main(int ac, char **av)
{
	char *hostname = NULL;
	t_flags flags = {};

	ft_parser(ac, av, &hostname, &flags);
	ft_socket(hostname);
}