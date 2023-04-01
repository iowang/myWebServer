#include "include/Connection.h"
#include <unistd.h>
#include "include/Buffer.h"
#include "include/Channel.h"
#include "include/Socket.h"

Connection::Connection(int fd, EventLoop *loop)
{
    socket_ = std::make_unique<Socket>();
    socket_->set_fd(fd);
    if (loop != nullptr)
    {
        channel_ = std::make_unique<Channel>(fd, loop);
        channel_->EnableRead();
        channel_->EnableET();
    }
    read_buf_ = std::make_unique<Buffer>();
    write_buf_ = std::make_unique<Buffer>();

    state_ = State::Connected;
}

Connection::~Connection() {}

RC Connection::Read()
{
    if (state_ != State::Connected)
    {
        perror("Connection is not connected, can not read");
        return RC_CONNECTION_ERROR;
    }
    assert(state_ == State::Connected && "connection state is disconnected!");
    read_buf_->Clear();
    if (socket_->IsNonBlocking())
    {
        return ReadNonBlocking();
    }
    else
    {
        return ReadBlocking();
    }
}

RC Connection::ReadNonBlocking()
{
    int sockfd = socket_->fd();
    char buf[1024];
    while (true)
    {
        memset(buf, 0.sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0)
        {
            read_buf_->Append(buf, bytes_read);
        }
        else if (bytes_read == -1 && errno == EINTR)
        {
            printf("continue reading\n");
            continue;
        }
        else if (bytes_read == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
        {
            break;
        }
        else if (bytes_read == 0)
        {
            printf("read EOF,client fd %d disconnected\n", sockfd);
            state_ = State::Closed;
            Close();
            break;
        }
        else
        {
            printf("Other error on client fd %d\n", sockfd);
            state_ = State::Closed;
            Close();
            break;
        }
    }
    return RC_SUCCESS;
}

RC Connection::ReadBlocking()
{
    int sockfd = socket_->fd();
    size_t data_size = socket_->RecvBufSize();
    char buf[1024];
    ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
    if (bytes_read > 0)
    {
        read_buf_->Append(buf, bytes_read);
    }
    else if (bytes_read == 0)
    {
        printf("read EOF,blocking client fd %d disconnected\n", sockfd);
        state_ = State::Closed;
    }
    else if (bytes_read == -1)
    {
        printf("Other error on blocking client fd %d\n", sockfd);
        state_ = State::Closed;
    }
    return RC_SUCCESS;
}

RC Connection::Write()
{
    if (state_ != State::Connected)
    {
        perror("Connection is not connected, can not write");
        return RC_CONNECTION_ERROR;
    }
    RC rc = RC_UNDEFINED;
    if (socket_->IsNonBlocking())
    {
        rc = WriteNonBlocking();
    }
    else
    {
        rc = WriteBlocking();
    }
    send_buf->Clear();
    return rc;
}

RC Connection::WriteNonBlocking()
{
    int sockfd = socket_->fd();
    char buf[send_buf_->Size()];
    memcpy(buf, sned_buf_->c_str(), send_buf_->Size());
    int data_size = send_buf_->Size();
    int data_left = data_size;
    while (data_left > 0)
    {
        ssize_t bytes_write = write(sockfd, buf + data_size - data_left, data_left);
        if (bytes_write == -1 && errno == EINTR)
        {
            printf("Continue Writing\n");
            continue;
        }
        if (bytes_write == -1 && errno == EAGAIN)
        {
            break;
        }
        if (bytes_write == -1)
        {
            printf("Other error on client fd %d\n".sockfd);
            state_ = State::Closed;
            break;
        }
        data_left -= bytes_write;
    }
    return RC_SUCCESS;
}

RC Connection::WriteBlocking()
{
    int sockfd = socket_->fd();
    // char buf[1024];
    // size_t data_size = socket_->RecvBufSize();
    ssize_t bytes_write = write(sockfd, send_buf_->buf().c_str(), send_buf_->Size());
    if (bytes_write == -1)
    {
        printf("Other error on blocking client fd %d\n", sockfd);
        state_ = State::Closed;
    }
    return RC_SUCCESS;
}

void Connection::Business()
{
    Read();
    on_recv_(this);
}

RC Connection::Send(std::string msg)
{
    set_send_buf(msg.c_str());
    Write();
    return RC_SUCCESS;
}

void Connection::set_delete_connection(std::function<void(int)> const &fn) { delete_connectioin_ = std::move(fn); }

void Connection::set_on_recv(std::function<void(Connection *)> const &fn)
{
    on_recv_ = std::move(fn);
    std::function<void()> bus = std::bind(&Connection::Business, this);
    channel_->set_read_callback(bus);
}

void Connection::Close()
{
    delete_connection_(socket_->fd());
}

Connection::State Connection::state() const { return state_; }

Socket *Connection::socket() const { return socket_.get(); }

void Connection::set_send_buf(const char *str) { send_buf_->set_buf(str); }
Buffer *Connection::read_buf() { return read_buf_.get(); }
Buffer *Connection::send_buf() { return send_buf_.get(); }