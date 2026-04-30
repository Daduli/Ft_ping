#include "ft_ping.h"

int main(int ac, char **av)
{
	// For IPv4, default ping packet is 56 bytes of data (28 bytes for header, exluded)
	int sockfd, data_size = 56;
	t_host_info host_info = {};
	t_flags flags = {};
	bool sending = true;

	ft_parser(ac, av, &host_info, &flags);
	sockfd = ft_socket(&host_info);
	ft_signal();

	print_ping_start(&host_info, data_size);

	// while (1)
	// {
	if (sending)
	{
		ft_send_packet(sockfd, data_size, &host_info);
		usleep(2);
		ft_receive_packet(sockfd);
	}
	// }
}