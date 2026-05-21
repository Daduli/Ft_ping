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
#include <netinet/ip_icmp.h>
#include <time.h>

#define SIZE 56

/*================================*/
/*=====    DATA STRUCTURE    =====*/
/*================================*/

typedef struct s_flags
{
	bool verbose;
	bool help;
} t_flags;

typedef struct s_host_info
{
	char *name;
	char ip[INET_ADDRSTRLEN];
} t_host_info;

typedef struct s_packet_info
{
	struct sockaddr *socket_address;
	socklen_t socket_length;
	int sequence;
	int ttl;
	struct timespec start_time;
	struct timespec end_time;
	int count;
} t_packet_info;

typedef struct s_ping_packet
{
	struct icmphdr header;
	char *data;
	ssize_t size;
} t_ping_packet;

/*================================*/
/*==========    PARSE    =========*/
/*================================*/

void ft_parser(int ac, char **av, t_host_info *host_info, t_flags *flags, t_packet_info *packet_info);

/*================================*/
/*==========    SOCKET    ========*/
/*================================*/

int ft_socket(t_packet_info *packet_info);

/*================================*/
/*==========    SIGNAL    ========*/
/*================================*/

void ft_signal();

/*================================*/
/*==========    PACKET    ========*/
/*================================*/

void ft_send_packet(int sockfd, t_packet_info *packet_info);
void ft_receive_packet(int sockfd, t_packet_info *packet_info, char *ip);

/*================================*/
/*=========    HELPERS    ========*/
/*================================*/

void print_error_message(int error_code, char *argument);
void display_help();
void print_ping_start(t_host_info *host, bool verbose);
void print_ping_loop(t_ping_packet recv_packet, char *host_ip, int ttl, float time);