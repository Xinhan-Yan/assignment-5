// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include "client.h"

#define PORT 8080
using namespace std;

int main(int argc, char const *argv[])
{
    Node* c = new Node(argv[1],PORT);
    int sock, valread;
    struct sockaddr_in serv_addr = c->create_address("127.0.0.1",PORT);
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = c->create_socket()) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    if (c->connection(sock, serv_addr) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    int val = send(sock, argv[1],strlen(argv[1]),0);
    char buf[1000];
    int numbytes = 1000;
    recv(sock, &buf,1000,0);
    printf("%s\n",buf);
    while (true) {
    char i;
    string ipaddr;
    std::cout << "Enter Valid IP Address To Send: ";
    getline(std::cin, ipaddr,std::cin.widen('\n'));
    string message;
    std::cout << "Enter Message To Send: ";
    getline(std::cin, message);

    int sock1, valread1;
    struct sockaddr_in serv_addr1 = c->create_address((const char *)&ipaddr,PORT);
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    if (listen(sock1, 3) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }

    int new_socket = c->accept1(sock, c->address, sizeof (c->address));
    if ((sock1 = c->create_socket()) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    if (c->connection(sock1, serv_addr1) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
}


    return 0;
}
