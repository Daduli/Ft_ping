#include "../ft_ping.h"

/*
 * Creates a raw socket and sets its TTL parameter
 */
int ft_socket(t_packet_info *packet_info)
{
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0)
        print_error_message(0, NULL, 0);
    // Set the Time-to-live for the socket to be sent
    if (setsockopt(sockfd, IPPROTO_IP, IP_TTL, &packet_info->ttl, sizeof(packet_info->ttl)) == -1)
    {
        close(sockfd);
        print_error_message(0, NULL, 0);
    }

    return (sockfd);
}