#include "ft_ping.h"

int main(int ac, char **av)
{
	t_host host = {};
	t_flags flags = {};

	ft_parser(ac, av, &host, &flags);
	ft_socket(&host);
	ft_signal();

	while (1)
	{
		continue;
	}
}