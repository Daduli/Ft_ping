#include "../ft_ping.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

void get_ip_address(char *hostname)
{
    char tmp[100];
    struct addrinfo hints = {
                        .ai_family = AF_INET,
                        .ai_socktype = SOCK_RAW,
                        .ai_protocol = IPPROTO_ICMP},
                    *result;

    if (getaddrinfo(hostname, NULL, &hints, &result))
        print_error_message(5, NULL);

    struct sockaddr_in ip_addr = *(struct sockaddr_in *)result->ai_addr;
    inet_ntop(AF_INET, &ip_addr.sin_addr, tmp, INET_ADDRSTRLEN);
    printf("IP: %s\n", tmp);
}

void ft_socket(char *hostname)
{
    get_ip_address(hostname);
}