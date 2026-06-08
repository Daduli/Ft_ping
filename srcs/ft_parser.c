#include "../ft_ping.h"

/*
 * Resolves the DNS and stores the IP address
 */
void get_ip_address(t_host_info *host_info, t_packet_info *packet_info)
{
	struct addrinfo hints = {
						.ai_family = AF_INET,
						.ai_socktype = 0,
						.ai_protocol = 0},
					*result;

	// Resolve the DNS, and check if it exists
	if (getaddrinfo(host_info->name, NULL, &hints, &result))
		print_error_message(5, NULL);

	// If it exists, stores the IP address
	struct sockaddr_in *addr = (struct sockaddr_in *)result->ai_addr;
	inet_ntop(AF_INET, &addr->sin_addr, host_info->ip, INET_ADDRSTRLEN);

	packet_info->socket_address = result->ai_addr;
	packet_info->socket_length = result->ai_addrlen;

	// Clear the memory space that was allocated for the IP linked list
	freeaddrinfo(result);
}

/*
 * From an argument, get its type as:
 * 'S' = One dash flag, 'D' = Two dashes flag, 'H' = Host operand
 */
char get_argument_type(char *argument)
{
	if (argument[0] == '-')
	{
		if (argument[1] == '-')
			return ('D');
		else
			return ('S');
	}
	else
		return ('H');
}

/*
 * For single dash flag, parse the argument in order to find all the flags.
 * Supported flags:
 * -v	verbose output
 * -?	help list
 */
void get_single_dash_flag(char *argument, t_flags *flags)
{
	// i = 1 to skip the '-' at the beginning of the string
	for (int i = 1; argument[i]; i++)
	{
		switch (argument[i])
		{
		case 'v':
			flags->verbose = true;
			break;
		case '?':
			flags->help = true;
			break;
		case 'q':
			flags->quiet = true;
			break;
		default:
			print_error_message(1, argument + i);
			break;
		}
	}
}

/*
 * For double dash flag, parse the argument in order to the flag.
 * Supported flags:
 * --verbose	verbose output
 * --help		help list
 */
void get_double_dash_flag(char *argument, t_flags *flags)
{
	if (!strcmp(argument, "verbose"))
		flags->verbose = true;
	else if (!strcmp(argument, "help"))
		flags->help = true;
	else if (!strcmp(argument, "quiet"))
		flags->quiet = true;
	else
		print_error_message(2, argument);

	printf("Quiet: %d\n", flags->quiet);
}

void ft_parser(int ac, char **av, t_host_info *host_info, t_flags *flags, t_packet_info *packet_info)
{
	int host_count = 0;

	for (int i = 1; i < ac; i++)
	{
		char type = get_argument_type(av[i]);
		if (type == 'H')
		{
			host_info->name = av[i];
			host_count++;
		}
		else if (type == 'S')
			get_single_dash_flag(av[i], flags);
		else if (type == 'D')
			get_double_dash_flag(av[i] + 2, flags);
	}

	if (flags->help)
		display_help();
	else if (host_count < 1)
		print_error_message(3, NULL);
	else if (host_count > 1)
		print_error_message(4, NULL);

	// Check if the hostname exist, if so, get its ip
	get_ip_address(host_info, packet_info);
}