#include "include/TcpServer.h"
#include "include/Acceptor.h"
#include "include/ThreadPool.h"

TcpServer::TcpServer()
{
    main_reactor_ = std::make_unique<EventLoop>();
    acceptor_ = std::make_unique<Acceptor>();
    std::function<void(int)> cb = std::bind(&TcpServer::NewConnection, this, std::placeholders::_1);
    acceptor_->set_new_connection_callback(cb);

    unsigned int size = std::thread::hardware_conncurrency();
}