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

/*================================*/
/*=====    DATA STRUCTURE    =====*/
/*================================*/

typedef struct s_flags
{
	bool verbose;
	bool help;
} t_flags;

/*================================*/
/*==========    PARSE    =========*/
/*================================*/

void ft_parser(int ac, char **av, char **hostname, t_flags *flags);

/*================================*/
/*==========    SOCKET    ========*/
/*================================*/

void ft_socket(char *hostname);

/*================================*/
/*=========    HELPERS    ========*/
/*================================*/

void print_error_message(int error_code, char *argument);
void display_help();