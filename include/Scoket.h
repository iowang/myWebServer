#pragma once
#include "common.h"
#include <string>
class Socket
{
private:
    int fd_;

public:
    DISALLOW_COPY_AND_MOVE(Socket);
    Socket();
    ~Socket();
    // 要有bind，listen函数,客户端还需要Connection函数
    RC Bind(const char *ip, uint16_t port) const;
    RC Listen() const;
    RC Connection(const char *ip, uint16_t port) const;
    // 需要能创建，获取fd
    RC Create();
    void set_fd(int fd);
    int fd() const;
    std::string get_addr() const;
    // 设置非阻塞，并判断
    RC SetNonBlocking() const;
    bool IsNonBlocking() const;
    // 缓冲区字节数
    size_t RecvBufSize() const;
};