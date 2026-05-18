#include "../ft_ping.h"

/*
 * Calculate the checksum for the ICMP header (Environment is 32 bits)
 */
unsigned short calculate_checksum(uint16_t *icmp, size_t bytes)
{
    // Sum must start at 0
    int sum = 0;

    // Get the sum of the ICMP header + data, 16 bits at a time
    while (bytes > 1)
    {
        sum += *icmp++;
        bytes -= 2;
    }

    // Add leftover byte, if there's any
    if (bytes == 1)
        sum += *(unsigned short *)icmp;

    // Fold 32 bits sum to 16 bits
    sum = (sum & 0xffff) + (sum >> 16);
    sum += (sum >> 16);

    // Perform one's complement on the sum
    return (~sum);
}

void ft_send_packet(int sockfd, t_host_info *host_info)
{
    t_ping_packet send_packet;
    int sequence_number = 0;

    // Fill the ICMP header with the information needed for the echo request
    send_packet.header.type = ICMP_ECHO;
    send_packet.header.code = 0;
    send_packet.header.un.echo.id = getpid();
    send_packet.header.un.echo.sequence = sequence_number++;
    send_packet.header.checksum = calculate_checksum((uint16_t *)&send_packet.header, sizeof(send_packet.header));

    // Send the packet for an echo request
    if (sendto(sockfd, &send_packet, sizeof(send_packet), 0, host_info->socket_address, host_info->socket_length) == -1)
        print_error_message(6, NULL);
}

void ft_receive_packet(int sockfd, t_host_info *host_info)
{
    t_ping_packet recv_packet;
    int tmp;

    printf("Receiving...\n");
    if ((tmp = recvfrom(sockfd, &recv_packet, sizeof(recv_packet), 0, host_info->socket_address, &host_info->socket_length)) == -1)
        print_error_message(6, NULL);

    printf("Received from: %s\n", host_info->ip);

    printf("Response type: %d \n", recv_packet.header.type);
}