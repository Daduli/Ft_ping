#include "../ft_ping.h"

/******************************************************************
 * From an argument, get its type as:                             *
 * 'S' = One dash flag, 'D' = Two dashes flag, 'H' = Host operand *
 ******************************************************************/
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
	{
		return ('H');
	}
}

/****************************************************************************
 * For single dash flag, parse the argument in order to find all the flags. *
 * Supported flags:                                                         *
 * -v	verbose output                                                      *
 * -?	help list                                                           *
 ****************************************************************************/

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
		default:
			printf("ft_ping: invalid option -- '%c'\nTry 'ping -?' for more information.\n", argument[i]);
			exit(1);
			break;
		}
	}
}

/****************************************************************************
 * For double dash flag, parse the argument in order to the flag.           *
 * Supported flags:                                                         *
 * --verbose	verbose output                                              *
 * --help		help list                                                   *
 ****************************************************************************/
void get_double_dash_flag(char *argument, t_flags *flags)
{
	if (!strcmp(argument, "verbose"))
		flags->verbose = true;
	else if (!strcmp(argument, "help"))
		flags->help = true;
	else
	{
		printf("ft_ping: unrecognized option '--%s'\nTry './ft_ping -?' for more information.\n", argument);
	}
}

void ft_parser(int ac, char **av, char **hostname, t_flags *flags)
{
	int host_count = 0;

	for (int i = 1; i < ac; i++)
	{
		printf("av: %s\n", av[i]);
		char type = get_argument_type(av[i]);
		if (type == 'H')
		{
			*hostname = av[i];
			host_count++;
		}
		else if (type == 'S')
			get_single_dash_flag(av[i], flags);
		else if (type == 'D')
			get_double_dash_flag(av[i] + 2, flags);
	}

	printf("Verbose: %d, Help: %d\n", flags->verbose, flags->help);

	if (host_count != 1)
	{
		print_error("ft_ping: missing host operand\nTry './ft_ping -?' for more information.");
	}
}