#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int sock_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUF_SIZE];
    int str_len;

    // 1. 创建 UDP Socket
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd == -1) { perror("Socket"); exit(1); }

    // 2. 绑定端口
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    if (bind(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind"); close(sock_fd); exit(1);
    }
    printf("UDP Server listening on port %d...\n", PORT);

    // 3. 循环接收数据 (UDP 不需要 accept)
    while (1) {
        memset(buffer, 0, BUF_SIZE);
        str_len = recvfrom(sock_fd, buffer, BUF_SIZE - 1, 0,
                           (struct sockaddr*)&client_addr, &client_len);
        if (str_len == -1) { perror("recvfrom"); continue; }

        printf("Received %d bytes from %s: %s\n", str_len, inet_ntoa(client_addr.sin_addr), buffer);
    }

    close(sock_fd);
    return 0;
}
