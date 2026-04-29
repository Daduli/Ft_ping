#pragma once

/*================================*/
/*==========    LIBC    ==========*/
/*================================*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

/*================================*/
/*=====    DATA STRUCTURE    =====*/
/*================================*/

typedef struct s_flags
{
	bool verbose;
	bool help;
} t_flags;

typedef struct s_host
{
	char *name;
	char ip[INET_ADDRSTRLEN];
	int data_length;
} t_host;

/*================================*/
/*==========    PARSE    =========*/
/*================================*/

void ft_parser(int ac, char **av, t_host *host, t_flags *flags);

/*================================*/
/*==========    SOCKET    ========*/
/*================================*/

void ft_socket(t_host *host);

/*================================*/
/*==========    SIGNAL    ========*/
/*================================*/

void ft_signal();

/*================================*/
/*=========    HELPERS    ========*/
/*================================*/

void print_error_message(int error_code, char *argument);
void display_help();