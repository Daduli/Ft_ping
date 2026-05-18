#include "../ft_ping.h"

void get_ip_address(t_host_info *host_info)
{
    struct addrinfo hints = {
                        .ai_family = AF_INET,
                        .ai_socktype = 0,
                        .ai_protocol = 0},
                    *result;

    // Resolve the DNS to get its IP address, and check if it exists
    if (getaddrinfo(host_info->name, NULL, &hints, &result))
        print_error_message(5, NULL);

    struct sockaddr_in *addr = (struct sockaddr_in *)result->ai_addr;
    inet_ntop(AF_INET, &addr->sin_addr, host_info->ip, INET_ADDRSTRLEN);

    host_info->socket_address = result->ai_addr;
    host_info->socket_length = result->ai_addrlen;

    // Clear the memory space that was allocated for the IP linked list
    freeaddrinfo(result);
}

int ft_socket(t_host_info *host_info)
{
    // Check if the given hostname exists
    get_ip_address(host_info);

    // Create a connectionless socket to send data
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
    int ttl = 64;
    if (sockfd < 0)
        print_error_message(6, NULL);
    // Set the Time-to-live for the socket to be sent
    if (setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) == -1)
    {
        close(sockfd);
        print_error_message(6, NULL);
    }

    return (sockfd);
}