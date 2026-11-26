
#include <server.h>


void start_server() {
    int server_sock = inet_init("127.0.0.1", 5060);
    fd_set set;
    int maxfd;
    int cli_sock;
    int cli_sock_arr[1024];
    socklen_t addrlen;
    struct sockaddr_in cli_addr;

    for (int i = 0; i < 1024; i++) {
        cli_sock_arr[i] = -1;
    }
    char cli_ip[16];

    while (1)
    {
        FD_ZERO(&set);
        FD_SET(server_sock, &set);
        maxfd = server_sock;

        for (int i = 0; i < 1024; i++)
        {
            if (cli_sock_arr[i] != -1)
            {
                FD_SET(cli_sock_arr[i], &set);
                if (cli_sock_arr[i] > maxfd)
                {
                    maxfd = cli_sock_arr[i];
                }
            }
        }

        int readynum = select(maxfd + 1, &set, NULL, NULL, NULL);
        if (readynum < 0)
        {
            perror("select fail");
            close(server_sock);
            exit(0);
        }
        if (readynum == 0)
        {
            continue;
        }

        if (FD_ISSET(server_sock, &set))
        {
            addrlen = sizeof(cli_addr);
            cli_sock = accept(server_sock, (struct sockaddr*)&cli_addr, &addrlen);
            if (cli_sock < 0)
            {
                perror("accept");
                continue;
            }
            inet_ntop(AF_INET, &cli_addr.sin_addr, cli_ip, sizeof(cli_ip));
            printf("new client: %s, port:%d\n", cli_ip, ntohs(cli_addr.sin_port));

            int idx;
            for (idx = 0; idx < 1024; idx++)
            {
                if (cli_sock_arr[idx] == -1)
                {
                    cli_sock_arr[idx] = cli_sock;
                    break;
                }
            }
            if (idx >= 1024)
            {
                printf("client full!\n");
                close(cli_sock);
            }
            else
            {
                first_res(cli_sock, cli_ip);
            }
        }

        for (int i = 0; i < 1024; i++)
        {
            int fd = cli_sock_arr[i];
            if (fd == -1) continue;

            if (FD_ISSET(fd, &set))
            {
                char buf[1024] = { 0 };
                int ret = recv(fd, buf, sizeof(buf) - 1, 0);
                if (ret > 0)
                {
                    printf("recv from client %d: %s\n", fd, buf);
                    send(fd, buf, ret, 0);
                }
                else if (ret == 0)
                {
                    printf("client %d closed\n", fd);
                    close(fd);
                    cli_sock_arr[i] = -1;
                }
                else
                {
                    perror("recv");
                    close(fd);
                    cli_sock_arr[i] = -1;
                }
            }
        }
    }
}

int main() {
    start_server();
    return 0;
}