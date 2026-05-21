#include "ft_ping.h"

volatile bool pinging = true;

int main(int ac, char **av)
{
	// For IPv4, default ping packet is 56 bytes of data (28 bytes for header, excluded)
	int sockfd;
	t_host_info host_info = {};
	t_packet_info packet_info = {.ttl = 64, .sequence = 0, .count = 1};
	t_flags flags = {};

	// Parse the arguments to get the hostname and the flags, if any
	ft_parser(ac, av, &host_info, &flags, &packet_info);

	// Create the socket
	sockfd = ft_socket(&packet_info);

	// Setup the signals handling
	ft_signal();

	// Print the starting info
	print_ping_start(&host_info, flags.verbose);

	// Start the ping loop that will receive back the packets sent
	while (pinging)
	{
		ft_send_packet(sockfd, &packet_info);
		ft_receive_packet(sockfd, &packet_info, host_info.ip);
		sleep(1); // Wait X seconds between every ping
	}

	printf("%d Ping interrupted, ending...\n", pinging);
}