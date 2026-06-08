#include "ft_ping.h"

volatile bool pinging = true;

int main(int ac, char **av)
{
	int sockfd;
	t_host_info host_info = {};
	t_packet_info packet_info = {
		.ttl = 64,
		.sequence = 0,
		.count = 1,
	};
	t_flags flags = {};
	t_ping_stat stats = {
		.nb_sent = 0,
		.nb_received = 0,
		.max_time = -INFINITY,
		.min_time = INFINITY,
		.avg_time = 0.0,
		.square_avg_time = 0.0,
		.percentage_lost = 0,
		.stddev = 0,
	};

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
		ft_send_packet(sockfd, &packet_info, &stats);
		ft_receive_packet(sockfd, &packet_info, &stats, flags, host_info.ip);
		sleep(1); // Wait X seconds between every ping
	}

	// Calculate all the stats needed to display
	calculate_stats(&stats);

	print_ping_end(host_info.name, stats);
}