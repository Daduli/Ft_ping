#include "../ft_ping.h"

/*
Takes an error code and print the related message.

Code 1: unrecognized single dashed flag
Code 2: unrecognized double dashed flag
Code 3: no host
Code 4: more than one host
Code 5: Host not found
*/
void print_error_message(int error_code, char *argument)
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

	default:
		break;
	}
}

void display_help()
{
	printf("Usage: ./ft_ping [OPTIONS...] HOST ...\nSend ICMP ECHO_REQUEST packets to network hosts.\n\n Options:\n  -v, --verbose		verbose output\n  -?, --help		give this help list\n");
	exit(0);
}