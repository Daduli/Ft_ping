#include "../ft_ping.h"

/*
 * Takes an error code and print the related message.
 *
 * Code 1: unrecognized single dashed flag
 * Code 2: unrecognized double dashed flag
 * Code 3: no host
 * Code 4: more than one host
 * Code 5: host not found
 * Code 6: internal error
 * Code 7: invalid format for number
 * Cide 8: user is not root
 */
void print_error_message(int error_code, char *argument, int position)
{
	char *info_message = "Try './ft_ping --help' or './ft_ping -?' for more information.\n";

	switch (error_code)
	{
	case 1:
		printf("ft_ping: invalid option -- '%c'\n%s", argument[0], info_message);
		exit(1);
		break;
	case 2:
		printf("ft_ping: unrecognized option '--%s'\n%s", argument, info_message);
		exit(2);
		break;
	case 3:
		printf("ft_ping: missing host operand\n%s", info_message);
		exit(3);
		break;
	case 4:
		printf("ft_ping: only one host is needed\n");
		exit(4);
		break;
	case 5:
		printf("ft_ping: unknown host\n");
		exit(5);
		break;
	case 6:
		printf("ft_ping: %s\n", strerror(errno));
		exit(6);
		break;
	case 7:
		printf("ft_ping: invalid value ('%s' near '%s')\n", argument, argument + position);
		exit(7);
		break;
	case 8:
		printf("ft_ping: root permission needed to run ft_ping\n");
		exit(8);
		break;
	default:
		break;
	}
}

/* Function used to print the message from the -? or --help flags */
void display_help()
{
	printf("Usage: ./ft_ping [OPTIONS...] HOST ...\n"
		   "Send ICMP ECHO_REQUEST packets to network hosts.\n\n"
		   " Options:\n"
		   "  -v, --verbose		verbose output\n"
		   "  -?, --help		give this help list\n");
	exit(0);
}

/* Prints the first message at the start of the ping command */
void print_ping_start(t_host_info *host, bool verbose)
{
	if (!verbose)
		printf("PING %s (%s): 56 data bytes\n", host->name, host->ip);
	else
		printf("PING %s (%s): 56 data bytes, id 0x%x = %d\n", host->name, host->ip, getpid(), getpid());
}

/* Prints info for each packet received */
void print_ping_loop(uint16_t sequence, char *host_ip, int ttl, float time)
{
	printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n", SIZE, host_ip, sequence, ttl, time);
}

/* Set up the stats to display */
void calculate_stats(t_ping_stat *stats)
{
	stats->percentage_lost = (1.0 - (float)(stats->nb_received) / (float)stats->nb_sent) * 100.0;
	stats->avg_time /= stats->nb_received;
	stats->square_avg_time /= stats->nb_received;
	stats->stddev = sqrt(((stats->square_avg_time - (stats->avg_time * stats->avg_time)) * stats->nb_received) / (stats->nb_received - 1));
}

/* Prints the ping stats after the end of the loop */
void print_ping_end(char *host_name, t_ping_stat stats)
{
	printf("--- %s ping statistics ---\n%d packets transmitted, %d packets received, %d%% packet loss\n"
		   "round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n",
		   host_name, stats.nb_sent, stats.nb_received, stats.percentage_lost, stats.min_time, stats.avg_time, stats.max_time, stats.stddev);
}