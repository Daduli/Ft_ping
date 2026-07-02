#include "../ft_ping.h"

/*
 * Compares a flag with an argument
 */
bool compare_flag(char *argument, char *flag)
{
	int i = 0;
	int length = strlen(flag);

	while (i < length)
	{
		if (argument[i] != flag[i])
			return (1);
		i++;
	}
	if (argument[i])
		return (1);
	return (0);
}

bool is_number(char *argument)
{
	for (int i = 0; argument[i]; i++)
	{
		if (i == 0 && argument[i] == '-')
			continue;
		if (!isdigit(argument[i]))
			return (0);
	}
	return (1);
}

char *get_flag_type(t_flags *flags)
{
	if (flags->count)
		return ("c");
	if (flags->size)
		return ("s");
	if (flags->interval)
		return ("i");
	return (NULL);
}

void save_flag_argument(char *argument, t_flags *flags, t_packet_info *packet_info)
{
	if (!is_number(argument))
		print_error_message(10, argument, 0);

	int value = atoi(argument);
	if (flags->time_to_live)
	{
		if (value <= 0)
			print_error_message(11, argument, 0);
		if (value > 255)
			print_error_message(12, argument, 0);
		packet_info->ttl = value;
		flags->time_to_live = false;
	}
	if (flags->count && value > 0)
	{
		packet_info->count = value;
		flags->count = false;
	}
	if (flags->size)
	{
		if (value < 0)
			print_error_message(11, argument, 0);
		if (value >= 65400)
			print_error_message(12, argument, 0);
		packet_info->size = value;
		flags->size = false;
	}
	if (flags->interval)
	{
		if (value == 0)
			print_error_message(11, argument, 0);
		packet_info->interval = atoi(argument);
		flags->interval = false;
	}
}

/*
 * Resolves the DNS and stores the IP address
 */
void get_ip_address(t_host_info *host_info, t_packet_info *packet_info)
{
	struct addrinfo hints = {
						.ai_family = AF_INET,
						.ai_socktype = SOCK_RAW,
						.ai_protocol = IPPROTO_ICMP},
					*result;

	// Resolve the DNS, and check if it exists
	if (getaddrinfo(host_info->name, NULL, &hints, &result))
		print_error_message(5, NULL, 0);

	// If it exists, stores the IP address
	struct sockaddr_in *addr = (struct sockaddr_in *)result->ai_addr;
	inet_ntop(AF_INET, &(addr->sin_addr), host_info->ip, INET_ADDRSTRLEN);

	packet_info->socket_address.sin_family = AF_INET;
	packet_info->socket_address.sin_addr = addr->sin_addr;

	// Clear the memory space that was allocated for the IP linked list
	freeaddrinfo(result);
}

/*
 * Get an argument type as:
 * 'S' = One dash flag
 * 'D' = Two dashes flag
 * 'H' = Host operand
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
 * Parses the argument for single dashed flags
 */
void get_single_dash_flag(char *argument, t_flags *flags, bool *is_flag_argument)
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
			display_help();
			break;
		case 'q':
			flags->quiet = true;
			break;
		case 'c':
			if (argument[i + 1])
				print_error_message(7, &argument[i], 0);
			*is_flag_argument = true;
			flags->count = true;
			break;
		case 's':
			if (argument[i + 1])
				print_error_message(7, &argument[i], 0);
			*is_flag_argument = true;
			flags->size = true;
			break;
		case 'i':
			if (argument[i + 1])
				print_error_message(7, &argument[i], 0);
			*is_flag_argument = true;
			flags->interval = true;
			break;
		default:
			print_error_message(1, argument + i, 0);
			break;
		}
	}
}

/*
 * Parse the argument for double dashed flag.
 */
void get_double_dash_flag(char *argument, t_flags *flags, bool *is_argument_flag)
{
	if (!strcmp(argument, "verbose"))
		flags->verbose = true;
	else if (!strcmp(argument, "help"))
		display_help();
	else if (!strcmp(argument, "quiet"))
		flags->quiet = true;
	else if (!compare_flag(argument, "ttl"))
	{
		*is_argument_flag = true;
		flags->time_to_live = true;
	}
	else if (!compare_flag(argument, "count"))
	{
		*is_argument_flag = true;
		flags->count = true;
	}
	else if (!compare_flag(argument, "size"))
	{
		*is_argument_flag = true;
		flags->size = true;
	}
	else if (!compare_flag(argument, "interval"))
	{
		*is_argument_flag = true;
		flags->interval = true;
	}
	else
		print_error_message(2, argument, 0);
}

/* Parses the command line arguments */
void ft_parser(int ac, char **av, t_host_info *host_info, t_flags *flags, t_packet_info *packet_info)
{
	int host_count = 0;
	bool is_argument_type = false;

	for (int i = 1; i < ac; i++)
	{
		char type = get_argument_type(av[i]);
		if (type == 'H')
		{
			host_info->name = av[i];
			host_count++;
		}
		else if (type == 'S')
			get_single_dash_flag(av[i], flags, &is_argument_type);
		else if (type == 'D')
			get_double_dash_flag(av[i] + 2, flags, &is_argument_type);
		if (is_argument_type)
		{
			if (!av[i + 1])
			{
				if (type == 'S')
					print_error_message(9, get_flag_type(flags), 0);
				if (type == 'D')
					print_error_message(6, av[i], 0);
			}
			else
			{
				save_flag_argument(av[i + 1], flags, packet_info);
				i++;
			}
		}
	}

	if (host_count < 1)
		print_error_message(3, NULL, 0);
	if (host_count > 1)
		print_error_message(4, NULL, 0);

	get_ip_address(host_info, packet_info);
}