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
		return ('H');
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
			print_error_message(1, argument + i);
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
		print_error_message(2, argument);
}

void ft_parser(int ac, char **av, t_host_info *host_info, t_flags *flags)
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
}