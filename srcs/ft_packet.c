#include "../ft_ping.h"

/*
 * Calculate the checksum for the ICMP header (Environment is 32 bits)
 */
unsigned short calculate_checksum(unsigned short *icmp, int bytes)
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

void ft_send_packet(int sockfd, int data_size, t_host_info *host_info)
{
    struct icmphdr header;
    int sequence_number = 0;
    unsigned short icmp[sizeof(struct icmphdr) + data_size];

    header.type = ICMP_ECHO;
    header.un.echo.id = getpid();
    header.un.echo.sequence = sequence_number++;
    header.checksum = calculate_checksum(icmp, sizeof(icmp));

    int tmp;
    if ((tmp = sendto(sockfd, icmp, sizeof(icmp), 0, (struct sockaddr *)&host_info->socket_address, sizeof(host_info->socket_address))) == -1)
        print_error_message(6, NULL);

    printf("Sent: %d\n", tmp);
}

void ft_receive_packet(int sockfd)
{
    struct sockaddr_in r_addr;
    struct icmphdr buffer;

    // if (recvfrom(sockfd, &buffer, 255, 0, (struct sockaddr *)&r_addr, sizeof(r_addr)) == -1)
    //     print_error_message(6, NULL);

    printf("Receiving...\n");
    recv(sockfd, &buffer, 255, MSG_DONTWAIT);

    printf("Received: %d\n", buffer.type);
}