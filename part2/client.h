#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "string.h"
#include <string>
#include "array.h"
#include <arpa/inet.h>


class Node {
public:
    struct sockaddr_in address;
    int sock;
    StrArray* ip_addresses;
    IntArray* file_descriptors;

    Node(const char* ip_address,int port) {
        this->address = create_address(ip_address,port);
        ip_addresses = new StrArray();
        file_descriptors = new IntArray();
    }

    struct sockaddr_in create_address(const char* ip_address,int port) {
        struct sockaddr_in address;
        char str[INET_ADDRSTRLEN];
        address.sin_family = AF_INET;
        inet_pton(AF_INET,ip_address,&(address.sin_addr));
        address.sin_port = htons( port );
        return address;
    }

    int create_socket() {
        return socket(AF_INET, SOCK_STREAM, 0);
    }

    //
    int connection(int sockfd,struct sockaddr_in addr) {
        return connect(sockfd,(struct sockaddr *)&addr,
                    sizeof addr);
    }

    int setsocketopt(int fd, int opt) {
        return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt));
    }

    int bind1(int fd,sockaddr_in address) {
        return bind(fd, (struct sockaddr *)&address,
             sizeof(address));
    }

    int accept1(int fd, struct sockaddr_in address,int addrlen) {
        return accept(fd, (struct sockaddr *)&address,
               (socklen_t*)&addrlen );
    }

    bool add_ip(String* ip) {
        return ip_addresses->add(ip);
    }

    bool add_file_descriptor(int ip) {
        return file_descriptors->add(ip);
    }
};