#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int read_reply(int sockfd)
{
    char buf[1024];
    int code = 0;
    size_t numbytes;

    if ((numbytes = recv(sockfd, buf, sizeof(buf), 0)) == -1)
    {
        perror("recv");
        return 1;
    }

    if (numbytes > 0)
    {
        buf[numbytes] = '\0';
        code = atoi(buf);
    }

    return code;
}

int connect_to_server(const char *ip, int port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("[-]Socket error");
        return 1;
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);
    int result = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (result < 0)
    {
        close(sock);
        perror("Failed to connect");
        return 1;
    }
    printf("Successfully connected!\n");
    return sock;
}

int main(int argc, char **argv)
{
    char ip[16];
    printf("Enter IP address to scan ports\n"); // 163.20.104.59
    scanf("%s", ip);
    int ports[] = {25, 53, 80, 110, 143, 443, 445, 8080};
    int size = sizeof(ports) / sizeof(ports[0]);
    int code;
    for (int i = 0; i < size; i++)
    {
        printf("Port %d\n", ports[i]);
        int sock = connect_to_server(ip, ports[i]);
        if (sock != 1)
        {
            code = read_reply(sock);
            printf("Port %d\n%d\n", ports[i], code);
        }
    }
    return 0;
}
