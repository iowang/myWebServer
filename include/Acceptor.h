#pragma once
#include <memory>
#include "common.h"
#include <functional>

class Acceptor
{
private:
    // 首先要有Socket这个类
    std::unique_ptr<Socket> socket_;
    // 要有处理事件的Channel
    std::unique_ptr<Channel> channel_;
    // 要有处理连接事件的回调函数(使用服务器的connection)
    std::function<void(int)> new_connection_callback;

public:
    DISALLOW_COPY_AND_MOVE(Acceptor);
    Acceptor(EventLoop* loop);
    ~Acceptor();
    void set_new_connection_callback(std::function<void(int)> const &callback);
    RC AcceptConnection() const;
};