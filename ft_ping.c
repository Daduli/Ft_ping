#include "ft_ping.h"

volatile bool pinging = true;
volatile bool sending = true;

int main(int ac, char **av)
{
	int sockfd;
	t_host_info host_info = {};
	t_packet_info packet_info = {
		.ttl = 64, // Default TTL on Linux
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
	clock_t begin = 0;
	double time_spent = 0.0;

	// Root permission is mandatory because of raw socket
	if (getuid())
		print_error_message(8, NULL, 0);

	ft_parser(ac, av, &host_info, &flags, &packet_info);
	sockfd = ft_socket(&packet_info);
	ft_signal();
	print_ping_start(&host_info, flags.verbose, packet_info.size);

	while (pinging)
	{
		if (sending)
		{
			ft_send_packet(sockfd, &packet_info, &stats);
			sending = false;
			// If the count flag is set, stop the loop after sending the number of packets specified
			if (stats.nb_sent != packet_info.count)
				alarm(packet_info.interval);
			// If count flag is set, start a countdown after sending the last packet
			else
				begin = clock();
		}
		ft_receive_packet(sockfd, &packet_info, &stats, flags, host_info.ip);
		// If the count flag is set, stop the loop after receiving the number of packets sent
		if (packet_info.count > 0 && stats.nb_received == packet_info.count)
			pinging = false;
		// After countdown start, check constantly if 5 seconds have passed since the last packet was sent, if so, stop the loop
		if (begin)
		{
			time_spent = (double)(clock() - begin) / CLOCKS_PER_SEC;
			if (time_spent >= 5.0)
				pinging = false;
		}
	}

	calculate_stats(&stats);
	print_ping_end(host_info.name, stats);
}