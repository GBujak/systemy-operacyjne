#pragma once

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int make_sock();
struct sockaddr_in make_addr(uint16_t port);
void connect_sock(int sock, struct sockaddr_in* addr);
void bind_sock(int sock, struct sockaddr_in* addr);
void listen_sock(int sock, int queue);