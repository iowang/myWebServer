#pragma once
#include "common.h"
#include <functional>
#include <unordered_map>
#include <vector>
#include <memory>

class TcpServer
{
private:
    // 要有一个acceptor类,用于处理连接
    std::unique_ptr<Acceptor> acceptor_;
    // 要有一个Connection类来处理其余的逻辑
    std::unordered_map<int, std::unique_ptr<Connection>> connections_;
    // 线程池需要Server来维护，而非其他类来维护
    std::unique_ptr<ThreadPool> thread_pool_;
    // 要有一个mainReactor
    std::unique_ptr<EventLoop> main_reactor_;
    // 要有一系列的从Reactor
    std::vector<std::unique_ptr<EventLoop>> sub_reactors_;
    // 要有处理连接的回调函数
    std::function<void(Connection *)> on_connect_;
    // 要有处理读取数据的回调函数
    std::function<void(Connection *)> on_recv_;

public:
    // 禁止拷贝和移动
    DISALLOW_COPY_AND_MOVE(TcpServer);
    TcpServer();
    ~TcpServer();
    // 服务器要启动
    void start();
    RC NewConnection(int fd);
    RC DeleteConnection(int fd);

    void onConnect(std::function<void(Connection *)> fn);
    void onRecv(std::function<void(Connection *)> fn);
};