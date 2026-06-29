#include "ft_ping.h"

volatile bool pinging = true;
volatile bool sending = true;

int main(int ac, char **av)
{
	int sockfd;
	t_host_info host_info = {};
	t_packet_info packet_info = {
		.ttl = 64,
		.sequence = 0,
		.count = 0,
		.size = 56, // For IPv4, default ping packet is 56 bytes of data (28 bytes for header, excluded)
		.interval = 1,
	};
	t_flags flags = {};
	t_ping_stat stats = {
		.nb_sent = 0,
		.nb_received = 0,
		.nb_received_success = 0,
		.max_time = -INFINITY,
		.min_time = INFINITY,
		.avg_time = 0.0,
		.square_avg_time = 0.0,
		.percentage_lost = 0,
		.stddev = 0,
	};
	int i = 0;

	// Root permission is mandatory because of raw socket
	if (getuid())
		print_error_message(8, NULL, 0);

	// Parse the arguments to get the hostname and the flags, if any
	ft_parser(ac, av, &host_info, &flags, &packet_info);

	printf("Count: %d\n", packet_info.count);

	// Create the socket
	sockfd = ft_socket(&packet_info);

	// Setup the signals handling
	ft_signal();

	// Print the starting info
	print_ping_start(&host_info, flags.verbose, packet_info.size);

	// Start the ping loop that will receive back the packets sent
	while (pinging)
	{
		if (sending)
		{
			ft_send_packet(sockfd, &packet_info, &stats);
			sending = false;
			if (stats.nb_sent != packet_info.count)
				alarm(packet_info.interval);
		}
		ft_receive_packet(sockfd, &packet_info, &stats, flags, host_info.ip);
		if (packet_info.count > 0 && stats.nb_received == packet_info.count)
			pinging = false;
	}

	// Calculate all the stats needed to display
	calculate_stats(&stats);

	print_ping_end(host_info.name, stats);
}