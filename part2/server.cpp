// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include "client.h"

#define PORT 8080
#define MAXLEN 100

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char const *argv[])
{
    char buffer[1024] = {0};
    char *hello = "Hello from server";
    char s[INET6_ADDRSTRLEN];
    int master_socket , addrlen , new_socket , client_socket[30] ,
            max_clients = 30 , activity, i , valread , sd,max_sd;

    Node* c = new Node(argv[1],PORT);
    addrlen = sizeof(c->address);
    int opt = 1;

    // Creating socket file descriptor
    if ((master_socket = c->create_socket()) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (c->setsocketopt(master_socket,opt))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (c->bind1(master_socket,c->address)<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(master_socket, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    //set of socket descriptors
    fd_set readfds;
    while(1) {
        //clear the socket set
        FD_ZERO(&readfds);

        //add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        //add child sockets to set
        for (i = 0; i < max_clients; i++) {
            //socket descriptor
            sd = client_socket[i];

            //if valid socket descriptor then add to read list
            if (sd > 0)
                FD_SET(sd, &readfds);

            //highest file descriptor number, need it for the select function
            if (sd > max_sd)
                max_sd = sd;
        }

        //wait for an activity on one of the sockets , timeout is NULL ,
        //so wait indefinitely
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR)) {
            printf("select error");
        }
        int len = 0;
        new_socket = c->accept1(master_socket, c->address, addrlen);
        printf("ACCEPTED\n");
        if (new_socket < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        char buf[MAXLEN];
        int numbytes = recv(new_socket, buf, MAXLEN - 1, 0);
        buf[numbytes] = '\0';
        if (c->ip_addresses->indexOf(new String(buf)) == -1) {
            c->add_ip(new String(buf));
            c->add_file_descriptor(new_socket);

        } else {
            printf("GOT HERE\n");
            c->set_ip(new String(buf),c->ip_addresses->indexOf(new String(buf)));
            c->set_file_descriptor(new_socket,c->file_descriptors->indexOf(new_socket));
        }
        for (int i = 0; i < c->ip_addresses->size();i++) {
            len += c->ip_addresses->get(i)->size() + 1;
        }
        int size_ = len;
        char* cstr_ = new char[size_ + 1]; // ensure that we copy the terminator
        for (int i = 0; i < c->ip_addresses->size();i++) {
            strcat(cstr_, (const char*)c->ip_addresses->get(i)->cstr_);
            strcat(cstr_," ");
        }
        printf("%s\n",cstr_);
        new_socket = c->file_descriptors->get(c->ip_addresses->indexOf(new String(buf)));
        printf("%i",new_socket);
        printf("GOT HERE\n");
        send(new_socket, cstr_,len,0);
        printf("%i\n",c->ip_addresses->size());
    }
    return 0;
}
