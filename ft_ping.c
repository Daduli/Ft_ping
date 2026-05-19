#include "ft_ping.h"

int main(int ac, char **av)
{
	// For IPv4, default ping packet is 56 bytes of data (28 bytes for header, excluded)
	int sockfd;
	t_host_info host_info = {};
	t_packet_info packet_info = {.ttl = 64, .sequence = 0};
	t_flags flags = {};
	bool sending = true;

	ft_parser(ac, av, &host_info, &flags);
	sockfd = ft_socket(&host_info, &packet_info);
	ft_signal();

	print_ping_start(&host_info, flags.verbose);

	while (sending)
	{
		ft_send_packet(sockfd, &packet_info);
		ft_receive_packet(sockfd, &packet_info, host_info.ip);
		sleep(1);
	}
}