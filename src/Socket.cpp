#include "Socket.h"
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
Socket::Socket() : fd_(-1) {}

Socket::~Socket()
{
    if (fd_ ! = -1)
    {
        close(fd_);
        fd_ = -1;
    }
}

RC Socket::Bind(const char *ip, uint16_t port) const
{
    assert(fd_ != -1);
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
    if (bind(fd_, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("Failed to bind socket");
        return RC_SOCKET_ERROR;
    }
    return RC_SUCCESS;
}

RC Socket::Listen() const
{
    assert(fd_ != -1);
    if (listen(fd_, SOMAXCONN) == -1)
    {
        perror("Failed to listen socket");
        return RC_SOCKET_ERROR;
    }
    return RC_SUCCESS;
}

RC Socket::Connect(const char *ip, uint16_t port) const
{

}