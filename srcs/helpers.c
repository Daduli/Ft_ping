#include "../ft_ping.h"

/*
 * Takes an error code and print the related message.
 *
 * Code 0: internal error
 * Code 1: unrecognized single dashed flag
 * Code 2: unrecognized double dashed flag
 * Code 3: no host
 * Code 4: more than one host
 * Code 5: host not found
 * Code 6: double dash missing argument
 * Code 7: single dashed flag is not the last
 * Code 8: user is not root
 * Code 9: single dash missing argument
 * Code 10: invalid flag argument
 * Code 11: argument value too small
 * code 12: argument value too big
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
		printf("ft_ping: option '%s' requires an argument\n%s", argument, info_message);
		exit(6);
		break;
	case 7:
		printf("ft_ping: option needs to be last -- '%c'\n", argument[0]);
		exit(7);
		break;
	case 8:
		printf("ft_ping: root permission needed to run ft_ping\n");
		exit(8);
		break;
	case 9:
		printf("ft_ping: option requires an argument -- '%s'\n%s", argument, info_message);
		exit(9);
		break;
	case 10:
		printf("ft_ping: invalid value ('%s' near '%s')\n", argument, argument + position);
		exit(10);
	case 11:
		printf("ft_ping: option value too small: %s\n", argument);
		exit(11);
	case 12:
		printf("ft_ping: option value too big: %s\n", argument);
		exit(12);
	default:
		printf("ft_ping: %s\n", strerror(errno));
		exit(0);
		break;
	}
}

/* Displays the message from the -? or --help flags */
void display_help()
{
	printf("Usage: ./ft_ping [OPTIONS...] HOST ...\n"
		   "Send ICMP ECHO_REQUEST packets to network hosts.\n\n"
		   " Options:\n"
		   "  -?, --help			give this help list\n"
		   "  -v, --verbose			verbose output\n"
		   "  -q, --quiet			quiet output\n"
		   "  -c, --count NUMBER		stop after sending NUMBER packets\n"
		   "  -s, --size NUMBER		send NUMBER data octets\n"
		   "  -i, --interval NUMBER		wait NUMBER seconds between sending each packet\n"
		   "      --ttl N			specify N as time-to-live\n");
	exit(0);
}

void print_ping_start(t_host_info *host, bool verbose, int data_size)
{
	if (!verbose)
		printf("PING %s (%s): %d data bytes\n", host->name, host->ip, data_size);
	else
		printf("PING %s (%s): %d data bytes, id 0x%x = %d\n", host->name, host->ip, data_size, getpid(), getpid());
}

/* Prints info for each packet received */
void print_ping_loop(uint16_t sequence, char *host_ip, int ttl, float time, int data_size)
{
	if (data_size >= 8 && data_size < 24)
		printf("%d bytes from %s: icmp_seq=%d ttl=0\n", data_size, host_ip, sequence);
	else
		printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n", data_size, host_ip, sequence, ttl, time);
}

void print_ping_error(char *host_ip, int icmp_type)
{
	if (icmp_type == ICMP_TIME_EXCEEDED)
		printf("From %s: Time to live exceeded\n", host_ip);
	else if (icmp_type == ICMP_DEST_UNREACH)
		printf("From %s: Destination host unreachable\n", host_ip);
}

/* Set up the stats to display */
void calculate_stats(t_ping_stat *stats)
{
	stats->percentage_lost = (1.0 - (float)(stats->nb_received_success) / (float)stats->nb_sent) * 100.0;
	stats->avg_time /= stats->nb_received_success;
	if (stats->nb_received_success == 1)
		stats->stddev = 0.0;
	else
	{
		stats->square_avg_time /= stats->nb_received_success;
		stats->stddev = sqrt(((stats->square_avg_time - (stats->avg_time * stats->avg_time)) * stats->nb_received_success) / (stats->nb_received_success - 1));
	}
}

/* Prints the ping stats after the end of the loop */
void print_ping_end(char *host_name, t_ping_stat stats)
{
	printf("--- %s ping statistics ---\n%d packets transmitted, %d packets received, %d%% packet loss\n",
		   host_name, stats.nb_sent, stats.nb_received_success, stats.percentage_lost);
	if (stats.nb_received)
		printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n", stats.min_time, stats.avg_time, stats.max_time, stats.stddev);
}