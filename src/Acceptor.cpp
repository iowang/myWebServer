#include "include/Acceptor.h"
#include <fcntl.h>
Acceptor::Acceptor(EventLoop *loop)
{
    socket_ = std::make_unique<Socket>();
    assert(socket_->Create() == RC_SUCCESS);
    assert(socket_->Bind("127.0.0.1", 1234) == RC_SUCCESS);
    assert(socket_->Listen() == RC_SUCCESS);

    channel_ = std::make_unique<Channel>(socket_->fd(), loop);
    std::function<void()> cb = std::bind(&Acceptor::AcceptConnection, this);
    channel_->set_read_callback(cb);
    channel_->EnableRead();
}

Acceptor::~Acceptor() {}

RC Acceptor::AcceptConnection() const
{
    int clnfd = -1;
    if (socket_->Accept(clnfd) != RC_SUCCESS)
    {
        return RC_ACCEPTOR_ERROR;
    }
    fcntl(clnfd, F_SETFL, fcntl(clnfd, F_GETFL) | O_NONBLOCK);
    if (new_connection_callback)
    {
        new_connection_callback(clnfd);
    }
    return RC_SUCCESS;
}

void Acceptor::set_new_connection_callback(std::function<void(int)> cb)
{
    new_connection_callback = cb;
}