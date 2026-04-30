#include "../ft_ping.h"

void get_ip_address(t_host_info *host_info)
{
    struct addrinfo hints = {
                        .ai_family = AF_INET,
                        .ai_socktype = SOCK_RAW,
                        .ai_protocol = IPPROTO_ICMP},
                    *result;

    // Resolve the DNS to get its IP address, and check if it exists
    if (getaddrinfo(host_info->name, NULL, &hints, &result))
        print_error_message(5, NULL);

    host_info->socket_address = *(struct sockaddr_in *)result->ai_addr;
    inet_ntop(AF_INET, &host_info->socket_address.sin_addr, host_info->ip, INET_ADDRSTRLEN);

    // Clear the memory space that was allocated for the IP linked list
    freeaddrinfo(result);
}

int ft_socket(t_host_info *host_info)
{
    // Check if the hostname given exists
    get_ip_address(host_info);

    // Create a raw socket to send data
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    int ttl = 64;
    if (sockfd < 0)
        print_error_message(6, NULL);
    // Set the Time-to-live for the socket to be sent
    if (setsockopt(sockfd, IPPROTO_ICMP, IP_TTL, &ttl, sizeof(ttl)) == -1)
    {
        close(sockfd);
        print_error_message(6, NULL);
    }

    return (sockfd);
}