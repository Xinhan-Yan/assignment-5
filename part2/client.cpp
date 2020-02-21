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
    send(sock, argv[1],strlen(argv[1]),0);
    char i;
    string ipaddr;
    std::cout << "Enter Valid IP Address To Send: ";
    getline(std::cin, ipaddr,std::cin.widen('\n'));
    string slash("\\");
    ipaddr.append(slash);
    send(sock, &ipaddr,ipaddr.length(),0);
    string message;
    std::cout << "Enter Message To Send: ";
    getline(std::cin, message);

    return 0;
}