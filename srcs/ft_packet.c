#include "../ft_ping.h"

/*
 * Calculate the checksum for the ICMP header (Environment is 32 bits)
 */
unsigned short calculate_checksum(void *icmp, int bytes)
{
    unsigned short *buf = icmp;
    // Sum must start at 0
    unsigned int sum = 0;
    unsigned short result;

    // Get the sum of the ICMP header + data, 16 bits at a time
    while (bytes > 1)
    {
        sum += *buf++;
        bytes -= 2;
    }

    // Add leftover byte, if there's any
    if (bytes == 1)
        sum += *(unsigned char *)buf;

    // Fold 32 bits sum to 16 bits
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);

    // Perform one's complement on the sum
    result = ~sum;
    return (result);
}

void ft_send_packet(int sockfd, t_packet_info *packet_info, t_ping_stat *stats)
{
    t_ping_packet packet;
    size_t packet_size = sizeof(struct icmphdr) + packet_info->size;

    // Fill the ICMP header with the information needed for the echo request
    packet.header.type = ICMP_ECHO;
    packet.header.code = 0;
    packet.header.checksum = 0;
    packet.header.un.echo.id = getpid();
    packet.header.un.echo.sequence = htons(packet_info->sequence++);
    packet.data = malloc(packet_size);

    // Merge the content of the header and data
    memset(packet.data + sizeof(struct icmphdr), '0', packet_info->size);
    memcpy(packet.data, &(packet.header), sizeof(packet.header));

    // Define the checksum to be sent
    ((struct icmphdr *)packet.data)->checksum = calculate_checksum(packet.data, packet_size);

    // Register the time when the packet was sent
    clock_gettime(CLOCK_MONOTONIC, &packet_info->start_time);

    // Send the packet for an echo request
    if ((sendto(sockfd, packet.data, packet_size, 0, (struct sockaddr *)&packet_info->socket_address, sizeof(packet_info->socket_address))) == -1)
        print_error_message(0, NULL, 0);
    stats->nb_sent++;

    free(packet.data);
}

void ft_receive_packet(int sockfd, t_packet_info *packet_info, t_ping_stat *stats, t_flags flags, char *ip)
{
    char buffer[1024];
    struct sockaddr_in response_addr;
    socklen_t addr_len = sizeof(response_addr);
    struct msghdr message;
    struct iovec iov[1];
    ssize_t bytes_read;

    iov[0].iov_base = buffer;
    iov[0].iov_len = sizeof(buffer);
    memset(&message, 0, sizeof(message));
    message.msg_name = &response_addr;
    message.msg_namelen = addr_len;
    message.msg_iov = iov;
    message.msg_iovlen = 1;

    bytes_read = recvmsg(sockfd, &message, MSG_DONTWAIT);

    if (errno != EAGAIN && errno != EWOULDBLOCK && bytes_read == -1)
        print_error_message(0, NULL, 0);
    else if (bytes_read == -1)
        return;

    struct iphdr *ip_hdr = (struct iphdr *)buffer;
    struct icmphdr *icmp_response = (struct icmphdr *)(buffer + ip_hdr->ihl * 4);

    // Check if the response is an echo reply
    if (icmp_response->un.echo.id == getpid())
    {
        stats->nb_received++;
        if (icmp_response->type == ICMP_ECHO)
            return;
        else if (icmp_response->type == ICMP_ECHOREPLY)
            stats->nb_received_success++;
    }
    else if (flags.verbose && (icmp_response->type == ICMP_TIME_EXCEEDED || icmp_response->type == ICMP_DEST_UNREACH))
    {
        print_ping_error(ip, icmp_response->type);
        return;
    }
    else
        return;

    // Register the time when the packet was received
    clock_gettime(CLOCK_MONOTONIC, &packet_info->end_time);

    // Calculate the RTT (Round time trip) for the packet
    packet_info->end_time.tv_sec -= packet_info->start_time.tv_sec;
    packet_info->end_time.tv_nsec -= packet_info->start_time.tv_nsec;
    float time_ms = (float)packet_info->end_time.tv_sec * 1000 + (float)packet_info->end_time.tv_nsec / 1000000;

    if (time_ms > stats->max_time)
        stats->max_time = time_ms;
    if (time_ms < stats->min_time)
        stats->min_time = time_ms;
    stats->avg_time += time_ms;
    stats->square_avg_time += time_ms * time_ms;

    if (!flags.quiet)
        print_ping_loop(ntohs(icmp_response->un.echo.sequence), ip, packet_info->ttl, time_ms, ntohs(ip_hdr->tot_len) - (ip_hdr->ihl * 4));
}