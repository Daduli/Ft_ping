#include "../ft_ping.h"

void get_ip_address(char *hostname)
{
    // char tmp[100];
    struct addrinfo hints = {
                        .ai_family = AF_INET,
                        .ai_socktype = SOCK_RAW,
                        .ai_protocol = IPPROTO_ICMP},
                    *result;

    // Resolve the DNS to get its IP address, and check if it exists
    if (getaddrinfo(hostname, NULL, &hints, &result))
        print_error_message(5, NULL);

    freeaddrinfo(result);

    // struct sockaddr_in ip_addr = *(struct sockaddr_in *)result->ai_addr;
    // inet_ntop(AF_INET, &ip_addr.sin_addr, tmp, INET_ADDRSTRLEN);
    // printf("IP: %s\n", tmp);
}

void ft_socket(char *hostname)
{
    // Check if the hostname given exists
    get_ip_address(hostname);

    // Create a raw socket to send data
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    int ttl;
    if (sockfd < 0)
        print_error_message(6, NULL);
    if (setsockopt(sockfd, IPPROTO_ICMP, IP_TTL, &ttl, 64) == -1)
    {
        close(sockfd);
        print_error_message(6, NULL);
    }
}