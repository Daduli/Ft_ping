#include "../ft_ping.h"

void get_ip_address(t_host *host)
{
    // char tmp[100];
    struct addrinfo hints = {
                        .ai_family = AF_INET,
                        .ai_socktype = SOCK_RAW,
                        .ai_protocol = IPPROTO_ICMP},
                    *result;

    // Resolve the DNS to get its IP address, and check if it exists
    if (getaddrinfo(host->name, NULL, &hints, &result))
        print_error_message(5, NULL);

    struct sockaddr_in ip_address = *(struct sockaddr_in *)result->ai_addr;
    inet_ntop(AF_INET, &ip_address.sin_addr, host->ip, INET_ADDRSTRLEN);
    printf("IP: %s\n", host->ip);

    // Clear the memory space that was allocated for the IP linked list
    freeaddrinfo(result);
}

void ft_socket(t_host *host)
{
    // Check if the hostname given exists
    get_ip_address(host);

    // Create a raw socket to send data
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    int ttl;
    if (sockfd < 0)
        print_error_message(6, NULL);
    // Set the Time-to-live for the socket to be sent
    if (setsockopt(sockfd, IPPROTO_ICMP, IP_TTL, &ttl, 64) == -1)
    {
        close(sockfd);
        print_error_message(6, NULL);
    }
}