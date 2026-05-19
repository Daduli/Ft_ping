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

void ft_send_packet(int sockfd, t_packet_info *packet_info)
{
    ssize_t data_size = SIZE - sizeof(t_packet_info);
    ssize_t packet_size = sizeof(t_packet_info) + data_size;
    t_ping_packet *send_packet = malloc(packet_size);
    int sequence_number = 0;

    // Store the size of the packet to be sent
    send_packet->size = packet_size;

    // Fill the ICMP header with the information needed for the echo request
    send_packet->header.type = ICMP_ECHO;
    send_packet->header.code = 0;
    send_packet->header.un.echo.id = getpid();
    send_packet->header.un.echo.sequence = packet_info->sequence++;
    send_packet->header.checksum = calculate_checksum((uint16_t *)&send_packet->header, sizeof(send_packet->header));

    // Register the time when the packet was sent
    clock_gettime(CLOCK_REALTIME, &packet_info->start_time);

    int tmp;
    // Send the packet for an echo request
    if ((tmp = sendto(sockfd, send_packet, packet_size, 0, packet_info->socket_address, packet_info->socket_length)) == -1)
        print_error_message(6, NULL);

    free(send_packet);
}

void ft_receive_packet(int sockfd, t_packet_info *packet_info, char *ip)
{
    ssize_t data_size = SIZE - sizeof(t_packet_info);
    ssize_t packet_size = sizeof(t_packet_info) + data_size;
    t_ping_packet *recv_packet = malloc(packet_size);

    int tmp;
    if ((tmp = recvfrom(sockfd, recv_packet, packet_size, 0, packet_info->socket_address, &packet_info->socket_length)) == -1)
        print_error_message(6, NULL);

    // Register the time when the packet was received
    clock_gettime(CLOCK_REALTIME, &packet_info->end_time);

    // Calculate the time (ms) it took from sending to receiving the packet
    packet_info->end_time.tv_sec -= packet_info->start_time.tv_sec;
    packet_info->end_time.tv_nsec -= packet_info->start_time.tv_nsec;
    float time_ms = (float)packet_info->end_time.tv_sec * 1000 + (float)packet_info->end_time.tv_nsec / 1000000;

    print_ping_loop(*recv_packet, ip, packet_info->ttl, time_ms);

    free(recv_packet);
}